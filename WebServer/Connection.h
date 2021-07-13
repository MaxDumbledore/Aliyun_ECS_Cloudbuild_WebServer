#ifndef WEBSERVER_CONNECTION_H
#define WEBSERVER_CONNECTION_H

#include <asio.hpp>
#include <nlohmann/json.hpp>
#include "HandlerManager.h"
#include "HeaderParser.h"

using namespace nlohmann;

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection> {
   public:
    explicit Connection(asio::ip::tcp::socket socket,
                        ConnectionManager& manager);

    void start();

    void stop();

   private:
    asio::ip::tcp::socket socket;
    ConnectionManager& manager;
    HeaderParser headerParser;

    std::string buffer, bodyString;
    json body;

    static HandlerManager handlerManager;

    void readHeader();

    void readBody(int restByteCount);

    void response();
};

using ConnectionPtr = std::shared_ptr<Connection>;
#endif