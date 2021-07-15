#include <asio.hpp>
#include <iostream>
#include "Server.h"
#include "TokenGenerator.h"

int main() {
    Server s("0.0.0.0",8080);
    return 0;
}