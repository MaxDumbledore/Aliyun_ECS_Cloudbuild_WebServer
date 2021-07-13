#include "Reply.h"

Reply::operator std::string() {
    std::string r;
    switch (status) {
        case OK:
            r = "HTTP/1.0 200 OK\r\n";
            break;
        case BAD_REQUEST:
            r = "HTTP/1.0 400 Bad Request\r\n";
            break;
        default:
            break;
    }
    for (auto& header : headers)
        r += header.first + ": " + header.second + "\r\n";
    r += "\r\n";
    r += content;
    return r;
}

Reply::Reply(StatusType status, std::string&& content, ContentType contentType)
    : status(status), content(std::move(content)), contentType(contentType) {
    headers.reserve(2);
    headers.emplace_back("Content-Length",
                         std::to_string(this->content.size()));
    headers.emplace_back("Content-Type", contentType == TEXT_PLAIN
                                             ? "text/plain"
                                             : "application/json");
}

Reply::Reply(StatusType status) : status(status) {}
