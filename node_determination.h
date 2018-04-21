//
// Created by sandeep on 4/16/18.
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class NodeDet {
    /* Data */
  public:
    std::string nodename;
    std::string hostname;
    std::string nodenum;
    int client_port;
    unordered_map<std::string, int> latencies;
    int number;

    /* Methods */
    NodeDet();
    //NodeDet(string name);
    NodeDet(int number);
    std::vector<std::string> str_split(const std::string &str , char delimiter);
};
