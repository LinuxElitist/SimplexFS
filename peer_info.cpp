//
// Created by sandeep on 4/12/18.
//
// We can use this class/file for returning ip,port and calculating hash value for the value.

#include <iostream>
#include <sstream>
#include <string>
#include "string.h"
#include <cstdint>
#include "peer_info.h"
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

using std::to_string;
using namespace std;

class PeerInfo;

PeerInfo::PeerInfo(int num) {
    char *n = NULL;
    int i = 0;
    string node_name = "N" + to_string(num);
    n = getenv(node_name.c_str());

    while (n) {

        nodes.push_back(NodeDet(num));
        i++;
        node_name = "N" + to_string(i);
        n = getenv(node_name.c_str());
    }
}

NodeDet PeerInfo::getNode(int num) const {
    return nodes[num];
}

int PeerInfo::getNumNodes() const {
    return nodes.size();
}

int test_peer_info() {
    PeerInfo(1);
}
