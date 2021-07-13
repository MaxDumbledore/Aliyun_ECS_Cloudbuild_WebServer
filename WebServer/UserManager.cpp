#include "UserManager.h"

UserManager::UserManager() {
    userFile = fopen("./user.txt", "r");
    if (userFile != nullptr) {
        char a[205], b[205], c[205], d[205], e[205], f[205];
        while (fscanf(userFile, "%s %s %s %s %s %s", a, b, c, d, e, f) == 6) {
            json j;
            j["username"] = a;
            j["firstName"] = b;
            j["lastName"] = c;
            j["email"] = d;
            j["password"] = e;
            j["phone"] = f;
            nameToInfo.emplace(a, std::move(j));
        }
        fclose(userFile);
    }
}

bool UserManager::insert(json&& user) {
    std::string name = user["username"];
    if (nameToItem.count(name))
        return false;

    userFile = fopen("./user.txt", "a");
    fprintf(userFile, "%s %s %s %s %s %s\n",
            name.data(),
            user["firstName"].get<std::string>().data(),
            user["lastName"].get<std::string>().data(),
            user["email"].get<std::string>().data(),
            user["password"].get<std::string>().data(),
            user["phone"].get<std::string>().data());
    fclose(userFile);

    nameToItem.emplace(user["username"], (Item){false, -1});
    nameToInfo.emplace(user["username"], std::move(user));
    return true;
}

std::pair<bool, std::string> UserManager::login(const std::string& username,
                                                const std::string password) {
    auto& info = nameToInfo[username];
    if (info["password"] != password)
        return {false, ""};

    auto& item = nameToItem[username];
    if (!item.online)
        item.online = true;
    return {true, tokenGenerator.generate(username)};
}

std::pair<bool, json> UserManager::queryUserInfo(const std::string& username) {
    if (!nameToItem.count(username))
        return {false, json()};
    auto r = nameToInfo[username];
    r.erase("username");
    r.erase("password");
    return {true, r};
}

bool UserManager::checkLoggedIn(const std::string& token) {
    auto decoded = tokenGenerator.decode(token);
    auto name = decoded.get_payload_claims()["username"].as_string();
    if (!tokenGenerator.verify(decoded) || !nameToInfo.count(name))
        return false;
    return nameToItem[name].online;
}

void UserManager::userEnterRoom(const std::string& name, int roomId) {
    nameToItem[name].currentRoom = roomId;
}

std::string UserManager::getNameFromToken(const std::string& token) {
    return tokenGenerator.decode(token)
        .get_payload_claims()["username"]
        .as_string();
}

void UserManager::userLeaveRoom(const std::string& name) {
    nameToItem[name].currentRoom = -1;
}

int UserManager::getRoomIdFromUser(const std::string& name) {
    return nameToItem[name].currentRoom;
}
