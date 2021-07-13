#ifndef WEBSERVER_CONNECTIONMANAGER_H
#define WEBSERVER_CONNECTIONMANAGER_H

#include <set>
#include "Connection.h"

class ConnectionManager {
   public:
    ConnectionManager()=default;

    void start(ConnectionPtr c);

    void stop(ConnectionPtr c);

   private:
    std::set<ConnectionPtr> connections;
};

#endif