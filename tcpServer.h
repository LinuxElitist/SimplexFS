//
// Created by sandeep on 3/23/18.
//

#ifndef BULLETINBOARDCONSISTENCY_SERVER_H
#define BULLETINBOARDCONSISTENCY_SERVER_H
#pragma once
#include <vector>
#include <mutex>
#include <netdb.h>

class Server
{
public:
    Server(int port, int num_conns);
    ~Server();
    int servListen();
    int servAccept();
    int servRead(int cli_num, char **buffer);
    int servWrite(int cli_num, const char *msg, int msg_size);
    int servClose(int cli_num);
    int servCloseAll();
    int getNumActiveClients() const;
    int getNumConns() const;

    std::vector<int> newsockfds;

private:
    int num_conns;
    int sockfd, portno;
    sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    mutable std::mutex mtx;
};


#endif //BULLETINBOARDCONSISTENCY_SERVER_H
