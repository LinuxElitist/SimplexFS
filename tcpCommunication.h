//
// Created by sandeep on 3/24/18.
//

#ifndef BULLETINBOARDCONSISTENCY_COMMUNICATION_H
#define BULLETINBOARDCONSISTENCY_COMMUNICATION_H
#pragma once

int g_read(int sockfd, char **msg);
int g_write(int sockfd, const char *msg, int msg_size);

#endif //BULLETINBOARDCONSISTENCY_COMMUNICATION_H
