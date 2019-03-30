cmake_minimum_required(VERSION 3.0.0)
project(Cpp-Server VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard
set(CMAKE_CXX_FLAGS "-Wall")

# include(CTest)
# enable_testing()

include_directories(Cpp-Server PRIVATE include)

add_executable(Cpp-Server 
    src/main.cpp
    src/TCP_Server.cpp
)

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
