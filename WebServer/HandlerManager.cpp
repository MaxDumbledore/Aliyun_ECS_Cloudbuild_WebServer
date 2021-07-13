#include "HandlerManager.h"

HandlerManager::HandlerManager() {
    postHandlers["room"] = [&](json&& j) -> RETURN_TYPE {
        auto token = j["Authorization"].get<std::string>().substr(7);
        if (!userManager.checkLoggedIn(token))
            return Reply(Reply::BAD_REQUEST);
        return Reply(Reply::OK,
                     std::to_string(roomManager.createNewRoom(j["name"])),
                     Reply::TEXT_PLAIN);
    };

    putHandlers["room"] = [&](json&& j) -> RETURN_TYPE {
        auto token = j["Authorization"].get<std::string>().substr(7);

        auto t = j["path"].get<std::string>();
        auto roomId = std::stoi(t.substr(0, t.size() - 6));
        if (!roomManager.checkRoomId(roomId) ||
            !userManager.checkLoggedIn(token))
            return Reply(Reply::BAD_REQUEST);
        const auto& username = userManager.getNameFromToken(token);
        userManager.userEnterRoom(username, roomId);
        roomManager.addUser(roomId, username);
        return Reply(Reply::OK);
    };

    putHandlers["roomLeave"] = [&](json&& j) -> RETURN_TYPE {
        auto token = j["Authorization"].get<std::string>().substr(7);
        if (!userManager.checkLoggedIn(token))
            return Reply(Reply::BAD_REQUEST);
        const auto& username = userManager.getNameFromToken(token);
        auto roomId = userManager.getRoomIdFromUser(username);
        if (roomId == -1)
            return Reply(Reply::BAD_REQUEST);
        userManager.userLeaveRoom(username);
        roomManager.removeUser(roomId, username);
        return Reply(Reply::OK);
    };

    getHandlers["room"] = [&](json&& j) -> RETURN_TYPE {
        std::string path = j["path"];
        if (path.find('/') == std::string::npos) {
            auto roomId = std::stoi(path);
            if (!roomManager.checkRoomId(roomId))
                return Reply(Reply::BAD_REQUEST);
            return Reply(Reply::OK,
                         std::string(roomManager.getRoom(roomId).name),
                         Reply::TEXT_PLAIN);
        } else {
            auto roomId = std::stoi(path.substr(0, path.size() - 6));
            if (!roomManager.checkRoomId(roomId))
                return Reply(Reply::BAD_REQUEST);
            json r = json::array();
            for (const auto& i : roomManager.getRoom(roomId).users)
                r.emplace_back(json({{"username", i}}));
            return Reply(Reply::OK, r.dump(), Reply::JSON);
        }
    };

    postHandlers["roomList"] = [&](json&& j) -> RETURN_TYPE {
        int sz = j["pageSize"], start = j["pageIndex"].get<int>() * sz;
        // if (!roomManager.checkRoomId(start))
        //     return Reply(Reply::BAD_REQUEST);
        int end = std::min(start + sz, roomManager.getCount() - 1);

        json r = json::array();
        for (int i = start; i <= end; i++) {
            const auto& name = roomManager.getRoom(i).name;
            r.emplace_back(json({{"name", name}, {"id", std::to_string(i)}}));
        }
        return Reply(Reply::OK, r.dump(), Reply::JSON);
    };

    postHandlers["user"] = [&](json&& j) -> RETURN_TYPE {
        return Reply(userManager.insert(std::move(j)) ? Reply::OK
                                                      : Reply::BAD_REQUEST);
    };

    getHandlers["userLogin"] = [&](json&& j) -> RETURN_TYPE {
        auto res = userManager.login(j["username"], j["password"]);
        if (res.first)
            return Reply(Reply::OK, std::move(res.second), Reply::TEXT_PLAIN);
        return Reply(Reply::BAD_REQUEST);
    };

    getHandlers["user"] = [&](json&& j) -> RETURN_TYPE {
        auto res = userManager.queryUserInfo(j["path"]);
        if (res.first)
            return Reply(Reply::OK, res.second.dump(), Reply::JSON);
        return Reply(Reply::BAD_REQUEST);
    };

    postHandlers["message"] = [&](json&& j) -> RETURN_TYPE {
        auto token = j["Authorization"].get<std::string>().substr(7);
        if (!userManager.checkLoggedIn(token))
            return Reply(Reply::BAD_REQUEST);
        auto name = userManager.getNameFromToken(token);
        auto roomId = userManager.getRoomIdFromUser(name);
        if (roomId == -1)
            return Reply(Reply::BAD_REQUEST);
        if (j["path"] == "send") {
            roomManager.addMessage(roomId,
                                   {j["id"], j["text"], std::time(nullptr)});
            return Reply(Reply::OK);
        } else {
            const auto& room = roomManager.getRoom(roomId);
            int sz = j["pageSize"], index = j["pageIndex"],
                end = room.messages.size() + (index + 1) * sz - 1,
                start = std::max(end - sz + 1, 0);
            // if (end < 0)
            //     return Reply(Reply::BAD_REQUEST);
            json r = json::array();
            for (int i = start; i <= end; i++) {
                const auto& message = room.messages[i];
                r.emplace_back(
                    json({{"id", message.id},
                          {"text", message.text},
                          {"timestamp", std::to_string(message.timestamp)}}));
            }
            return Reply(Reply::OK, r.dump(), Reply::JSON);
        }
    };
}

HANDLER HandlerManager::HandlerManager::deal(const std::string method,
                                             const std::string& uri) {
    if (method == "POST")
        return postHandlers[uri];
    else if (method == "GET")
        return getHandlers[uri];
    return putHandlers[uri];
}
