cmake_minimum_required(VERSION 3.0.0)
project(Cpp-Server VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard
set(CMAKE_CXX_FLAGS "-Wall")

include_directories(Cpp-Server PRIVATE include)

add_executable(Cpp-Server 
    src/main.cpp
    src/TCP_Server.cpp
)
