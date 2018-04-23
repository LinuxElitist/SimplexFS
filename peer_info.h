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

public:
    string ip;
    int port;
    string name;
    unsigned char *buf;
    int size;
    int num;
    std::vector <NodeDet> nodes;

    PeerInfo(int num);

    NodeDet getNode(int num) const;

    int getNumNodes() const;

    PeerInfo(const std::string &name);
};

#endif //SIMPLEXFS_PEER_INFO_H
