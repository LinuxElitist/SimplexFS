//
// Created by sandeep on 3/24/18.
//

#include "tcpCommunication.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int g_read(int sockfd, char **msg)
{
    int msg_size;
    if (read(sockfd, &msg_size, sizeof(msg_size)) < 0)
    {
        printf("Failure to read message size\n");
        return -1;
    }

    char *buffer = new char[msg_size];
    int total_bytes = 0;
    int cur_bytes = 0;

    while (total_bytes != msg_size)
    {
        cur_bytes = read(sockfd, &buffer[total_bytes], msg_size - total_bytes);
        if (cur_bytes < 0)
        {
            // Errors
            printf("Error reading from socket\n");
            delete[] buffer;
            return -1;
        }
        else
        {
            total_bytes += cur_bytes;
        }
    }

    *msg = buffer;
    return total_bytes;
}

int g_write(int sockfd, const char *msg, int msg_size)
{
    if (write(sockfd, &msg_size, sizeof(msg_size)) < 0)
    {
        printf("Failure to write message size\n");
        return -1;
    }

    int cur_bytes = 0;
    int total_bytes = 0;

    while (total_bytes != msg_size)
    {
        cur_bytes = write(sockfd, &msg[total_bytes], msg_size - total_bytes);
        if (cur_bytes < 0)
        {
            // Errors
            printf("Error writing to socket\n");
            return -1;
        }
        else
        {
            total_bytes += cur_bytes;
        }
    }

    return total_bytes;
}
