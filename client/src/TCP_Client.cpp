#include "TCP_Client.h"

#include <arpa/inet.h>
#include <unistd.h> // for socket close
#include <netdb.h> // for gethostbyname

#include <iostream>

TCP_Client::TCP_Client(std::string addr, const char* port)
    : addr(addr)
    , sockfd(-1)
    , client_active(false)
{
    this->port = std::stoi(port);
    init();
}

TCP_Client::~TCP_Client()
{
    stop();
    ::close(sockfd);
}

void TCP_Client::init()
{
    std::cout << "Connecting... Addr: " << addr << "  " << "port: " << port << std::endl;

    if(sockfd == -1)
    {
        sockfd = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == -1)
        {
            std::cout << "Could not create socket" << std::endl;
        }
    }
    // if(inet_addr(addr.c_str()) == -1)
    // {
    //     struct hostent *host_ent;
    //     struct in_addr **addr_list;
    //     if ( (host_ent = ::gethostbyname(addr.c_str()) ) == NULL)
    //     {
    //         herror("gethostbyname");
    //         std::cout<<"Failed to resolve hostname\n";
    //         exit(0);
    //     }
    //     addr_list = reinterpret_cast<struct in_addr **>(host_ent->h_addr_list);
    //     for(int i = 0; addr_list[i] != NULL; i++)
    //     {
    //         serv_addr.sin_addr = *addr_list[i];
    //         break;
    //     }
    //     // delete ptr's
    //     host_ent = NULL;
    //     delete host_ent;
    //     addr_list = NULL;
    //     delete addr_list;
    // }
    // else
    // {
    //     serv_addr.sin_addr.s_addr = inet_addr( addr.c_str() );
    // }
    serv_addr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    serv_addr.sin_family = AF_INET;
  	serv_addr.sin_port = htons(port);

    struct sockaddr* s_addr = reinterpret_cast<struct sockaddr*>(&serv_addr);
    if(::connect(sockfd, s_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect failed. Error");
        exit(0);
    }
    // delete ptr
    s_addr = NULL;
    delete s_addr;
    
}

void TCP_Client::start()
{
    client_active = true;
}

void TCP_Client::stop()
{
    client_active = false;
}

Data TCP_Client::recieve()
{
    Data dt;
    return dt;
}