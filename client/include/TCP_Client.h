#pragma once
// Sock_addr_in
#include <netinet/in.h>
#include <sys/socket.h>

#include <string>

struct Data
{
    std::string msg;
};

class TCP_Client
{
  public:
    TCP_Client(std::string addr, const char *port);
    ~TCP_Client();

    void init();

    void start();
    void stop();

    bool send(Data d);
    Data recieve();

  private:
    sockaddr_in serv_addr;
    std::string addr;

    Data s_data;

    int sockfd; // file descriptor
    int port;

    bool client_active;
};
