//
// Created by sandeep on 4/16/18.
//

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
class NodeDet
{
public:
    /* Data */
    std::string nodename;
    std::string hostname;
//    int client_port;
//    int server_port;
    std::unordered_map<std::string, int> latencies;

    /* Methods */
    NodeDet();
    NodeDet(int num);
    std::vector<std::string> str_split(const std::string &str , char delimiter);
};



