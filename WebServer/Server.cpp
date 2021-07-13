#include "Server.h"
#include "Connection.h"

Server::Server(const std::string& address, int port)
    : ioContext(1), tcpAcceptor(ioContext) {
    asio::ip::tcp::resolver resolver(ioContext);
    auto endpoint = static_cast<asio::ip::tcp::endpoint>(
        *resolver.resolve(address, std::to_string(port)).begin());
    tcpAcceptor.open(endpoint.protocol());
    tcpAcceptor.set_option(asio::ip::tcp::acceptor::reuse_address(true));

    tcpAcceptor.bind(endpoint);
    tcpAcceptor.listen();
    doAccept();
    ioContext.run();
}

void Server::doAccept() {
    tcpAcceptor.async_accept(
        [this](const asio::error_code &ec, asio::ip::tcp::socket socket) {
            if (!ec) {
                connectionManager.start(std::make_shared<Connection>(
                    std::move(socket), connectionManager));
            }
            doAccept();
        });
}
