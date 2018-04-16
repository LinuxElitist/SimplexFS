//
// Created by sandeep on 3/23/18.
//

#include "tcpClient.h"
#include <iostream>
#include "tcpCommunication.h"
#include <cstdio>
#include <unistd.h>
#include <string.h>

using namespace std;

Client::Client(const char *host, int port)  {
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

int Client::clntOpen() {
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

int Client::clntWrite(const char *msg, int msg_size) {
    return g_write(sockfd, msg, msg_size);
}

int Client::clntRead(char **msg)  {
    return g_read(sockfd, msg);
}

int Client::clntClose()  {
    if(close(sockfd) != 0) {
        printf("ERROR closing client socket %d\n", sockfd);
    }

    return 0;
}

int main(int argc, char *argv[]){
  if (argc < 4) {
      std::cout << "Usage: ./clientside server_ip client_port msg\n";
      exit(1);
  }
  char *serv_ip = (char *) argv[1];
  int self_port = stoi(argv[2]);
  char *msg = (char *) argv[3];

  char *return_msg;
  //char article_string[MAX_ARTICLE_LENGTH];

  Client conn(serv_ip, self_port);
  conn.clntOpen();
  conn.clntWrite(msg, strlen(msg));
  cout << "writing " << msg << "\n";
  conn.clntRead(&return_msg);
  cout << "read " << return_msg << "\n";
  conn.clntClose();
  return 0;
}
