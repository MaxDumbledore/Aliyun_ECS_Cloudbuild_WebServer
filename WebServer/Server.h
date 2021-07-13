#ifndef WEBSERVER_SERVER_H
#define WEBSERVER_SERVER_H

#include <asio.hpp>
#include "ConnectionManager.h"

class Server {
   public:
    Server(const std::string &address, int port);

   private:
    asio::io_context ioContext;
    asio::ip::tcp::acceptor tcpAcceptor;

    ConnectionManager connectionManager;

    void doAccept();
};

#endif