cmake_minimum_required(VERSION 3.16)
project(WebServer)

#set(CMAKE_BUILD_TYPE=Release)

set(CMAKE_CXX_STANDARD 17)

include_directories(/home/max/asio-1.18.1/include)

find_package(Threads)

find_package(OpenSSL)

##

include_directories(/home/max/nlohmann_json_cmake_fetchcontent/include)

include_directories(/home/max/jwt-cpp/include)


##

add_executable(WebServer main.cpp Server.cpp Server.h ConnectionManager.cpp ConnectionManager.h HeaderParser.cpp HeaderParser.h Connection.cpp Connection.h HandlerManager.cpp HandlerManager.h Reply.cpp Reply.h RoomManager.cpp RoomManager.h TokenGenerator.cpp TokenGenerator.h UserManager.cpp UserManager.h)

target_link_libraries(WebServer PUBLIC ${CMAKE_THREAD_LIBS_INIT} OpenSSL::Crypto)