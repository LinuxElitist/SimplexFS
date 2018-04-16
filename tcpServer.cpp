//
// Created by sandeep on 3/23/18.
//

#include "tcpServer.h"
#include "tcpCommunication.h"
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <algorithm>
#include <netinet/in.h>
#include <iostream>
using namespace std;
using std::vector;

Server::Server(int port, int num_conns)
        : portno(port), num_conns(num_conns)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) > 0)
    {
        // Errors
        printf("Error binding socket\n");
    }

    newsockfds = vector<int>(num_conns, -1);
}

Server::~Server()
{
    servCloseAll();
}

int Server::servListen()
{
    printf("Listening on port %d\n", portno);
    if(listen(sockfd, num_conns))
    {
        printf("ERROR: Listening failed\n");
        return -1;
    }
    servAccept();
    return 0;
}

int Server::servAccept()
{
    socklen_t clilen = sizeof(cli_addr);

    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
    {
        // Errors
        printf("Error accepting new socket\n");
    }

    int cli_num = -1;
    mtx.lock();
    for (size_t i = 0; i < newsockfds.size(); i++)
    {
        if (newsockfds[i] == -1)
        {
            newsockfds[i] = newsockfd;
            cli_num = i;
            break;
        }
    }
    mtx.unlock();
    printf("Accepting connection\n");

    return cli_num;
}

int Server::servRead(int cli_num, char **msg)
{
    int bytes_read = g_read(newsockfds[cli_num], msg);
    if (bytes_read < 0)
    {
        newsockfds[cli_num] = -1;
    }

    return bytes_read;
}

int Server::servWrite(int cli_num, const char *msg, int msg_size)
{
    int bytes_written = g_write(newsockfds[cli_num], msg, msg_size);
    if (bytes_written < 0)
    {
        newsockfds[cli_num] = -1;
    }

    return bytes_written;
}

int Server::servClose(int cli_num)
{
    int ret_code = 0;
    if (close(newsockfds[cli_num]) != 0)
    {
        printf("ERROR closing socket %d\n", newsockfds[cli_num]);
        ret_code = -1;
    }
    else
    {
        newsockfds[cli_num] = -1;
    }

    return ret_code;
}

int Server::servCloseAll()
{
    int ret_code = 0;
    if (close(sockfd) != 0)
    {
        printf("ERROR closing socket %d\n", sockfd);
        ret_code = -1;
    }

    for (size_t i = 0; i < newsockfds.size(); i++)
    {
        if (newsockfds[i] > -1 && close(newsockfds[i]) != 0)
        {
            printf("ERROR closing socket %d\n", newsockfds[i]);
            ret_code = -1;
        }
    }

    return ret_code;
}

int Server::getNumActiveClients() const
{
    int ret = 0;
    mtx.lock();
    for (int i = 0; i < num_conns; i++)
    {
        if (newsockfds[i] != -1) {
            ret++;
        }
    }
    mtx.unlock();
    return ret;
}

int Server::getNumConns() const
{ //gives max_connections allowed
    return num_conns;
}


int main(int argc, char *argv[]){
  if (argc < 3) {
      std::cout << "Usage: ./serverside port max_connections \n";
      exit(1);
  }
  int self_port = stoi(argv[1]);
  int max_conn = stoi(argv[2]);
  char *msg;
  char *msg2;
  //char article_string[MAX_ARTICLE_LENGTH];

  Server serv(self_port, max_conn);
  while (1) {

  serv.servListen();
  serv.servRead(0, &msg);
  serv.servRead(1, &msg2);
  serv.servWrite(0, msg, strlen(msg));
  cout << "#activeClients " << serv.getNumActiveClients() << "\n";
  serv.servWrite(1, msg2, strlen(msg2));
  cout << "#activeClients " << serv.getNumActiveClients() << "\n";
}
  return 0;
}
