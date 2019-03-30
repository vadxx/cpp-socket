#include "TCP_Client.h"

#include <iostream>

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        std::cout << "ERROR, no addr provided" << std::endl;
        return 0;
    }
    else
    {
        if (argc < 3) {
            std::cout << "ERROR, no port provided" << std::endl;
            return 0;
        }
    }

    TCP_Client client(argv[1], argv[2]); // address, port
    client.start();
    return 0;
}
