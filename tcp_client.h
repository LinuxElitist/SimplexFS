//
// Created by sandeep on 3/23/18.
//

#pragma once
#include <netdb.h>

class TcpClient
{
public:
    TcpClient(const char *hostname, int port);
    int clntOpen();
    int clntWrite(const char *msg, int msg_size);
    int clntRead(char **msg);
    int clntClose();

private:
    int sockfd, portno;
    hostent *server;
    sockaddr_in serv_addr;
};
