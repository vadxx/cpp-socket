cmake_minimum_required(VERSION 3.0.0)
project(Cpp-Client VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard
set(CMAKE_CXX_FLAGS "-Wall")

include_directories(Cpp-Client PRIVATE include)

add_executable(Cpp-Client
    src/main.cpp
    src/TCP_Client.cpp
)
