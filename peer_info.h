//
// Created by gupta329 on 4/13/18.
//

#ifndef SIMPLEXFS_PEER_INFO_H
#define SIMPLEXFS_PEER_INFO_H

#include "sxfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <openssl/md5.h>

#define BUFFER_SIZE 1024


vector<pair<string, int> > clientList;
void outputClientList();
client_list buildClientList();

int joinClient(string ip, int port);

void md5sum(const char * const filename, unsigned char *sum);

#endif //SIMPLEXFS_PEER_INFO_H
