#include <asio.hpp>
#include <iostream>
#include "Server.h"
#include "TokenGenerator.h"

int main() {
    Server s("127.0.0.1",8080);
    return 0;
}