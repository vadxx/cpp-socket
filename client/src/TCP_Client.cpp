#include "TCP_Client.h"

#include <arpa/inet.h>
#include <unistd.h> // for socket close
#include <netdb.h>  // for gethostbyname
#include <thread>
#include <iostream>

TCP_Client::TCP_Client(std::string addr, const char *port)
    : addr(addr), sockfd(-1), client_active(false)
{
    this->port = std::atoi(port);
    init();
}

TCP_Client::~TCP_Client()
{
    stop();
    ::close(sockfd);
}

void TCP_Client::init()
{
    std::cout << "Connecting... Addr: " << addr << "  "
              << "port: " << port << std::endl;

    if (sockfd == -1)
    {
        sockfd = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sockfd == -1)
        {
            std::cout << "Could not create socket" << std::endl;
        }
    }
    if (inet_addr(addr.c_str()) == -1)
    {
        struct hostent *host_ent;
        struct in_addr **addr_list;
        if ((host_ent = ::gethostbyname(addr.c_str())) == NULL)
        {
            herror("gethostbyname");
            std::cout << "Failed to resolve hostname\n";
            exit(0);
        }
        addr_list = reinterpret_cast<struct in_addr **>(host_ent->h_addr_list);
        for (int i = 0; addr_list[i] != NULL; i++)
        {
            serv_addr.sin_addr = *addr_list[i];
            break;
        }
        // delete ptr's
        host_ent = NULL;
        delete host_ent;
        addr_list = NULL;
        delete addr_list;
    }
    else
    {
        serv_addr.sin_addr.s_addr = inet_addr(addr.c_str());
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    struct sockaddr *s_addr = reinterpret_cast<struct sockaddr *>(&serv_addr);
    if (::connect(sockfd, s_addr, sizeof(serv_addr)) < 0)
    {
        perror("connect failed. Error");
        exit(0);
    }
    std::cout << "Connected" << std::endl;
    // delete ptr
    s_addr = NULL;
    delete s_addr;
}

void TCP_Client::start()
{
    client_active = true;

    while (client_active == true)
    {
        Data new_data;
        new_data.msg = "Pong";
        send(new_data);
        Data reciveve = recieve();
        std::cout << "Getted: " << reciveve.msg << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

void TCP_Client::stop()
{
    client_active = false;
}

bool TCP_Client::send(Data d)
{
    if (sockfd != -1)
    {
        std::cout << "Sending..." << std::endl;
        void *curr_d = reinterpret_cast<void *>(&d);
        if (::send(sockfd, curr_d, sizeof(d), 0) < 0)
        {
            std::cout << "Failed Send: data " << curr_d << std::endl;
            return false;
        }
        curr_d = NULL;
    }
    else
    {
        std::cout << "Failed Send: Sockfd problem!" << std::endl;
        return false;
    }
    return true;
}

Data TCP_Client::recieve()
{
    Data dt_rec;
    void *curr_d = reinterpret_cast<void *>(&dt_rec);
    if (::recv(sockfd, curr_d, sizeof(dt_rec), 0) < 0)
    {
        std::cout << "Failed recieve: data " << curr_d << std::endl;
    }
    s_data = dt_rec;
    curr_d = NULL;
    return dt_rec;
}