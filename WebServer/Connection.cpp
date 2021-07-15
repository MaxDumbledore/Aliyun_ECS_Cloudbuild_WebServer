#include "Connection.h"
#include <iostream>
#include "ConnectionManager.h"
#include "Reply.h"

#define MAX_BYTES 2048

HandlerManager Connection::handlerManager;

Connection::Connection(asio::ip::tcp::socket socket, ConnectionManager& manager)
    : socket(std::move(socket)), manager(manager) {
    buffer.reserve(MAX_BYTES);
}

void Connection::start() {
    readHeader();
}

void Connection::stop() {
    socket.close();
}

void Connection::readHeader() {
    auto self(shared_from_this());
    buffer.resize(MAX_BYTES);
    socket.async_read_some(
        asio::buffer(buffer),
        [this, self](const asio::error_code& ec, std::size_t bytesTransferred) {
            if (ec) {
                manager.stop(self);
                return;
            }
            HeaderParser::ResultType result;
            for (int i = 0; i < bytesTransferred; i++) {
                result = headerParser.append(buffer[i]);
                if (result == HeaderParser::BAD) {
                    manager.stop(self);
                    return;
                } else if (result == HeaderParser::GOOD) {
                    auto sLen = headerParser.get("Content-Length");
                    int len = sLen.empty() ? 0 : std::stoi(sLen);
                    if (len > 0) {
                        bodyString =
                            buffer.substr(i + 1, bytesTransferred - i - 1);
                        readBody(len - bodyString.size());
                    } else {
                        body.clear();
                        response();
                    }
                    return;
                }
            }
            readHeader();
            // requestParser.parse(buffer);
        });
}

void Connection::readBody(int restByteCount) {
    auto self(shared_from_this());
    buffer.resize(restByteCount);
    asio::async_read(socket, asio::buffer(buffer),
                     [this, self](const asio::error_code& ec, std::size_t) {
                         if (ec) {
                             manager.stop(self);
                             return;
                         }
                         bodyString += buffer;
                         body = json::parse(bodyString);
                         response();
                     });
}

void Connection::response() {
    auto self(shared_from_this());

    for (const auto& i : headerParser.getUriParams())
        body[i.first] = i.second;

    auto auth = headerParser.get("Authorization");
    if (!auth.empty())
        body["Authorization"] = auth;

    auto uriPath = headerParser.getUriPath().substr(1);
    auto pos = uriPath.find('/');
    if (pos != std::string::npos) {
        body["path"] = uriPath.substr(pos + 1, uriPath.size() - pos - 1);
        uriPath.resize(pos);
    }

    try {
        buffer = handlerManager.deal(headerParser.getMethod(),
                                     uriPath)(std::move(body));
    } catch (std::exception& e) {
        std::clog << e.what() << std::endl;
        // manager.stop(self);

        buffer = Reply(Reply::BAD_REQUEST);
    }
    body = json();

    asio::async_write(
        socket, asio::buffer(buffer),
        [this, self](const asio::error_code& ec, std::size_t) {
            if (!ec) {
                asio::error_code ignored;
                socket.shutdown(asio::ip::tcp::socket::shutdown_both, ignored);
            }
            manager.stop(self);
        });
}