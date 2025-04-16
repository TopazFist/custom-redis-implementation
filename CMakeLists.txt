cmake_minimum_required(VERSION 3.10)
project(redis_clone)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Build server
add_executable(server server.cpp helper.cpp)

# Build client
add_executable(client client.cpp helper.cpp)
