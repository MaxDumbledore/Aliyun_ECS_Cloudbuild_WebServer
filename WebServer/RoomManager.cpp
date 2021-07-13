#include "RoomManager.h"

RoomManager::RoomManager() {
    roomFile = fopen("./room.txt", "r");
    if (roomFile != nullptr) {
        char s[205];
        while (fscanf(roomFile, "%s", s) == 1)
            rooms.emplace_back((Room){s, {}, {}});
        fclose(roomFile);
    }
    for (int i = 0; i < rooms.size(); i++) {
        messageFile =
            fopen(("./room_" + std::to_string(i) + ".txt").data(), "r");
        if (messageFile == nullptr)
            continue;
        char x[205], y[205];
        unsigned int u;
        while (fscanf(messageFile, "%s %s %u", x, y, &u) == 3)
            rooms[i].messages.emplace_back((Message){x, y, u});
        fclose(messageFile);
    }
}

int RoomManager::createNewRoom(const std::string& name) {
    rooms.emplace_back((Room){name, {}, {}});

    roomFile = fopen("./room.txt", "a");
    fprintf(roomFile, "%s\n", name.data());
    fclose(roomFile);

    return rooms.size() - 1;
}

bool RoomManager::checkRoomId(int roomId) {
    return roomId >= 0 && roomId < rooms.size();
}

const Room& RoomManager::getRoom(int roomId) {
    return rooms[roomId];
}

void RoomManager::addUser(int roomId, const std::string& name) {
    rooms[roomId].users.emplace(name);
}

int RoomManager::getCount() {
    return rooms.size();
}

void RoomManager::removeUser(int roomId, const std::string& name) {
    rooms[roomId].users.erase(name);
}

void RoomManager::addMessage(int roomId, Message&& message) {
    messageFile =
        fopen(("./room_" + std::to_string(roomId) + ".txt").data(), "a");
    fprintf(messageFile, "%s %s %u\n", message.id.data(), message.text.data(),
            (unsigned int)message.timestamp);
    fclose(messageFile);

    rooms[roomId].messages.emplace_back(std::move(message));
}
