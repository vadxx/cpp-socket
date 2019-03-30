#pragma once
// Sock_addr_in
#include <netinet/in.h>
#include <sys/socket.h>

#include <string>
#include <thread>
#include <vector>

struct Data
{
    std::string msg;
};


class TCP_Server
{
public:
    TCP_Server(const char* port);
    ~TCP_Server();

    void init();

    void start();
    void stop();
    void handle(void* data);

    void send();
    Data recieve();

    void clean();

private:
    sockaddr_in serv_addr;
    sockaddr_in client_addr;

    Data s_data;

    int sockfd; // file descriptor
    int newsockfd; // file descriptor
    int count_sock;
    int port;

    std::thread th_recieve_data;
    std::thread th_send_data;
    std::thread th_handle_client;

    bool serv_active;
};