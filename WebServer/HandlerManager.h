#ifndef WEBSERVER_HANDLERMANAGER_H
#define WEBSERVER_HANDLERMANAGER_H

#include <nlohmann/json.hpp>
#include "Reply.h"
#include "RoomManager.h"
#include "TokenGenerator.h"
#include "UserManager.h"

using namespace nlohmann;

using RETURN_TYPE = Reply;
using HANDLER = std::function<RETURN_TYPE(json&& j)>;

class HandlerManager {
   public:
    HandlerManager();

    HANDLER deal(const std::string method, const std::string& uri);

   private:
    std::map<std::string, HANDLER> getHandlers, postHandlers, putHandlers;

    RoomManager roomManager;
    UserManager userManager;
};

#endif