#include "TCP_Server.h"

#include <arpa/inet.h>
#include <unistd.h> // for socket close
#include <iostream>
#include <chrono>

TCP_Server::TCP_Server(const char* port)
    : count_sock(5)
    , serv_active(false)
{
    init(port);
    s_data.msg = ("Hi ");
}

TCP_Server::~TCP_Server()
{
    stop();
}

void TCP_Server::init(std::string port_str)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // IPv4
    if (sockfd < 0) {
        std::cout << "ERROR opening socket" << std::endl;
    }
    port = std::stoi(port_str);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);

    struct sockaddr* s_addr = reinterpret_cast<struct sockaddr*>(&serv_addr);
    if (::bind(sockfd, s_addr, sizeof(serv_addr)) < 0)
    {
        std::cout <<"ERROR on binding" << std::endl;
    }

    // delete ptr
    s_addr = NULL;
    delete s_addr;
}


void TCP_Server::start()
{
    serv_active = true;
    listen(sockfd, count_sock);
    std::cout << "Listen ..." << std::endl;
    if (th_recieve_data.joinable())
    {
        th_recieve_data.join();
    }
    th_recieve_data = std::thread(&TCP_Server::recieve, this);

    if (th_send_data.joinable())
    {
        th_send_data.join();
    }
    th_send_data = std::thread(&TCP_Server::send, this);
}

void TCP_Server::stop()
{
    if (th_recieve_data.joinable())
    {
        th_recieve_data.join();
    }
    serv_active = false;
    std::cout << "Stop ..." << std::endl;
    detach();
}

void TCP_Server::handle(void* fd)
{
    std::cout << "Handle data client: " << fd << " ..."<< std::endl;
    int newsockfd = reinterpret_cast<long>(fd);
    Data n_data;
    while(serv_active)
    {
        count_sock = ::recv(newsockfd, reinterpret_cast<void*>(&n_data), sizeof(Data), 0);
        if (count_sock == 0)
        {
            ::close(newsockfd);
            break;
        }
        s_data = n_data;
    }
}

void TCP_Server::send()
{
    std::cout << "Sending ..." << std::endl;
    while(serv_active)
    {
        struct Data sending_struct;
        std::string str = s_data.msg;
        std::cout << "Message: " << str << std::endl;
        sending_struct.msg = "client: " + str + " Hello";
        ::send(newsockfd, &sending_struct, sizeof(sending_struct), 0);
        clean();
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }
}

Data TCP_Server::recieve()
{
    std::cout << "Recieving ..." << std::endl;
    struct Data data;
    while(serv_active)
    {
        socklen_t sock_size  = sizeof(client_addr);
        struct sockaddr* c_addr = reinterpret_cast<struct sockaddr*>(&client_addr);

        newsockfd = accept(sockfd, c_addr, &sock_size);
        if (newsockfd < 0) 
        {
            std::cout << "ERROR on accept" << std::endl;
        }
        data.msg = inet_ntoa(client_addr.sin_addr);

        std::cout << "New Client Addr: " << c_addr << " Port: " << std::endl;
        // pthread_create(&serverThread,NULL,&Task,(void *)newsockfd);
        th_handle_client = std::thread(&TCP_Server::handle, this, reinterpret_cast<void*>(&newsockfd));
        if (th_handle_client.joinable())
        {
            std::cout << "Join handle_client" << std::endl;
            th_handle_client.join();
        }
        // delete ptr
        c_addr = NULL;
        delete c_addr;
    }
    return data;
}

void TCP_Server::clean()
{
    s_data.msg = "";
    s_data.msg.resize(1);
}

void TCP_Server::detach()
{
    ::close(sockfd);
    ::close(newsockfd);
}