//
// Created by sandeep on 4/16/18.
//

#ifndef SIMPLEXFS_NODEINFO_H
#define SIMPLEXFS_NODEINFO_H

#pragma once

#include <string>
#include <unordered_map>
#include <vector>
struct NodeDet
{
    /* Data */
    std::string nodename;
    std::string hostname;
    int mp;
    int cp;
    std::unordered_map<std::string, int> latencies;

    /* Methods */
    NodeDet();
    NodeDet(int num);
    std::vector<std::string> str_split(const std::string &str , char delimiter);
};

#endif //SIMPLEXFS_NODEINFO_H
