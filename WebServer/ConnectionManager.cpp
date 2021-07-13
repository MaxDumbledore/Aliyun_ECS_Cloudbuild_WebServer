#include "ConnectionManager.h"

void ConnectionManager::start(ConnectionPtr c){
    c->start();
    connections.emplace(std::move(c));
}

void ConnectionManager::stop(ConnectionPtr c){
    connections.erase(c);
    c->stop();
}