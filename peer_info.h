//
// Created by gupta329 on 4/13/18.
//

#ifndef SIMPLEXFS_PEER_INFO_H
#define SIMPLEXFS_PEER_INFO_H

#include "sxfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <openssl/md5.h>
#include <vector>

#define BUFFER_SIZE 1024

using namespace std;
void outputClientList();
client_list buildClientList();

void md5sum(const char * const filename, unsigned char *sum);


#endif //SIMPLEXFS_PEER_INFO_H
