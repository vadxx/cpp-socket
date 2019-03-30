#include "TCP_Server.h"
#include <iostream>
// #include <vector>

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cout << "ERROR, no port provided" << std::endl;
        return 0;
    }

    TCP_Server serv(argv[1]); // port
    serv.start();
    return 0;
}
