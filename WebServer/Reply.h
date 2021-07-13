#ifndef WEBSERVER_REPLY_H
#define WEBSERVER_REPLY_H

#include <asio.hpp>
#include <string>
#include <vector>

class Reply {
   public:
    enum StatusType { OK = 200, BAD_REQUEST = 400 };

    enum ContentType {TEXT_PLAIN, JSON};

    explicit Reply(StatusType status);

    Reply(StatusType status, std::string &&content, ContentType contentType);

    operator std::string();

   private:
    std::vector<std::pair<std::string, std::string>> headers;
    std::string content;
    StatusType status;
    ContentType contentType;
};

#endif