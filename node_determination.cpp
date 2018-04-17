//
// Created by sandeep on 4/16/18.
//
#include "node_determination.h"
#include <cstdlib>
#include <vector>

using std::vector;
using std::string;
using std::to_string;

NodeDet::NodeDet() : nodename(""), hostname(""), client_port(0), server_port(0) {
}

NodeDet::NodeDet(int num) {
    char *sh = NULL;
    int i = 0;
    nodename = "N" + to_string(num);
    sh = getenv(nodename.c_str());
    if (!sh) {
        nodename = "";
        hostname = "";
        client_port = 0;
        server_port = 0;
        return;
    }
    vector <string> node_info = str_split(sh, ',');

    // Node info is host,mp,cp
    hostname = node_info[0];
    client_port = atoi(node_info[1].c_str());
    server_port = atoi(node_info[2].c_str());

    // Populate latency relationships
    char relainfo[64];
    snprintf(relainfo, 64, "REL%d", i);
    sh = getenv(relainfo);
    while (sh) {
        vector <string> rel_info = str_split(sh, ',');
        string other;
        int lat;
        if (rel_info[0] == nodename) {
            other = rel_info[1];
            lat = atoi(rel_info[2].c_str());
            latencies[other] = lat;
        } else if (rel_info[1] == nodename) {
            other = rel_info[0];
            lat = atoi(rel_info[2].c_str());
            latencies[other] = lat;
        }
        i++;
        snprintf(relainfo, 64, "REL%d", i);
        sh = getenv(relainfo);
    }
}

std::vector <string> NodeDet::str_split(const std::string &str, char delimiter) {
    vector <string> strings;
    string tmp = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == delimiter) {
            strings.push_back(tmp);
            tmp = "";
        } else {
            tmp = tmp + str[i];
        }
    }
    strings.push_back(tmp);
    return strings;
}

int test_node_determination() {
    NodeDet *node = new NodeDet(1);
}