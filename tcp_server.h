#pragma once
#include <vector>
#include <mutex>
#include <netdb.h>

class TcpServer
{
public:
    TcpServer(int port, int num_conns);
    ~TcpServer();
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