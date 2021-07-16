#ifndef WEBSERVER_ROOMMANAGER_H
#define WEBSERVER_ROOMMANAGER_H

#include <string>
#include <set>
#include <vector>
#include <cstdio>

struct Message{
    std::string id, text;
    time_t timestamp;
};

struct Room{
    std::string name;
    std::set<std::string> users;
    std::vector<Message> messages;
    FILE *file;
};

class RoomManager {
   public:
    RoomManager();

    int getCount();

    int createNewRoom(const std::string& name);

    bool checkRoomId(int roomId);

    const Room& getRoom(int roomId);

    void addUser(int roomId, const std::string &name);

    void removeUser(int roomId, const std::string &name);

    void addMessage(int roomId, Message &&message);

   private:
    std::vector<Room> rooms;

    FILE *roomFile,*messageFile;
};

#endif