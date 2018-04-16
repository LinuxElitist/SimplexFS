//
// Created by sandeep on 4/16/18.
//
#include "NodeInfo.h"
//#include "str_split.h"
#include <cstdlib>
#include <vector>
using std::vector;
using std::string;
using std::to_string;

NodeDet::NodeDet() : nodename(""), hostname(""), mp(0), cp(0)
{
}

NodeDet::NodeDet(int num)
{
    char *n = NULL;
    int i = 0;

    nodename = "N" + to_string(num);
    n = getenv(nodename.c_str());

    if (!n)
    {
        nodename = "";
        hostname = "";
        mp = 0;
        cp = 0;
        return;
    }

    vector<string> node_info = str_split(n, ',');

    // Node info is host,mp,cp
    hostname = node_info[0];
    mp = atoi(node_info[1].c_str());
    cp = atoi(node_info[2].c_str());

    // Populate latency relationships
    char rel_name[64];
    snprintf(rel_name, 64, "REL%d", i);
    n = getenv(rel_name);

    while (n)
    {
        vector<string> rel_info = str_split(n, ',');
        string other;
        int lat;

        if (rel_info[0] == nodename)
        {
            other = rel_info[1];
            lat = atoi(rel_info[2].c_str());
            latencies[other] = lat;
        }
        else if (rel_info[1] == nodename)
        {
            other = rel_info[0];
            lat = atoi(rel_info[2].c_str());
            latencies[other] = lat;
        }

        i++;
        snprintf(rel_name, 64, "REL%d", i);
        n = getenv(rel_name);
    }
}

std::vector<string> NodeDet::str_split(const std::string &str , char delimiter) {

    vector<string> strings;

    string tmp  = "";
    for (int i = 0; i< str.length(); i++) {
        if (str[i] == delimiter) {
            strings.push_back(tmp);
            tmp = "";
        }
        else {

            tmp  = tmp + str[i];
        }


    }

    strings.push_back(tmp);
    return strings;
}

