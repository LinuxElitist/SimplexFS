//
// Created by sandeep on 3/23/18.
//

#ifndef BULLETINBOARDCONSISTENCY_CLIENT_H
#define BULLETINBOARDCONSISTENCY_CLIENT_H

#pragma once
#include <netdb.h>

class Client
{
public:
    Client(const char *hostname, int port);
    int clntOpen();
    int clntWrite(const char *msg, int msg_size);
    int clntRead(char **msg);
    int clntClose();

private:
    int sockfd, portno;
    hostent *server;
    sockaddr_in serv_addr;
};
#endif //BULLETINBOARDCONSISTENCY_CLIENT_H
