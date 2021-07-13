#ifndef WEBSERVER_USERMANAGER_H
#define WEBSERVER_USERMANAGER_H

#include <cstdio>
#include <nlohmann/json.hpp>
#include "TokenGenerator.h"

using namespace nlohmann;

class UserManager {
   public:
    UserManager();

    bool insert(json&& user);

    std::pair<bool, std::string> login(const std::string& username,
                                       const std::string password);

    std::pair<bool, json> queryUserInfo(const std::string& username);

    bool checkLoggedIn(const std::string& token);

    void userEnterRoom(const std::string& name, int roomId);

    void userLeaveRoom(const std::string& name);

    int getRoomIdFromUser(const std::string& name);

    std::string getNameFromToken(const std::string& token);

   private:
    struct Item {
        bool online;
        int currentRoom;
    };

    std::map<std::string, json> nameToInfo;
    std::map<std::string, Item> nameToItem;

    TokenGenerator tokenGenerator;

    FILE* userFile;
};

#endif