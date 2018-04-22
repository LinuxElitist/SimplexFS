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
#include <set>
#include <map>
#include <iostream>
#include "node_determination.h"
using namespace std;

class PeerInfo {

#define BUFFER_SIZE 1024
public:
    string ip;
    int port;
    string name;
    unsigned char *buf;
    int size;
    int num;
std::vector<NodeDet> nodes;

//using namespace std;
// string getIp();
// int getPort();
// string toString ();
PeerInfo(int num);
NodeDet getNode(int num) const;
int getNumNodes() const;
PeerInfo(string ip, int port);
PeerInfo(const std::string &name);
const unsigned char *content() const;
int gsize();
bool hasContent() const;
//void md5sum(const char * const filename, unsigned char *sum);

};
#endif //SIMPLEXFS_PEER_INFO_H
