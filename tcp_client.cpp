//
// Created by sandeep on 3/23/18.
//

#include "tcp_client.h"
#include <iostream>
#include "tcp_communication.h"
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

TcpClient::TcpClient(const char *host, int port)  {
    char hostname[256];
    strcpy(hostname, host);
    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        // Errors
        printf("Error opening socket\n");
    }

    server = gethostbyname(hostname);
    if (!server) {
        // Errors
        printf("Could not get host %s\n", hostname);
    }
}

int TcpClient::clntOpen() {
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *) &serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        // Errors
        printf("Error connecting to port %d\n", portno);
        return -1;
    }

    return 0;
}

int TcpClient::clntWrite(const char *msg, int msg_size) {
    return g_write(sockfd, msg, msg_size);
}

int TcpClient::clntRead(char **msg)  {
    return g_read(sockfd, msg);
}

int TcpClient::clntClose()  {
    if(close(sockfd) != 0) {
        printf("ERROR closing TcpClient socket %d\n", sockfd);
    }

    return 0;
}

int test_tcp_TcpClient(int argc, char *argv[]){
  if (argc < 4) {
      std::cout << "Usage: ./TcpClientside server_ip TcpClient_port msg\n";
      exit(1);
  }
  char *serv_ip = (char *) argv[1];
  int self_port = stoi(argv[2]);
  char *msg = (char *) argv[3];

  char *return_msg;
  //char article_string[MAX_ARTICLE_LENGTH];

  TcpClient conn(serv_ip, self_port);
  conn.clntOpen();
  conn.clntWrite(msg, strlen(msg));
  cout << "writing " << msg << "\n";
  conn.clntRead(&return_msg);
  cout << "read " << return_msg << "\n";
  conn.clntClose();
  return 0;
}
