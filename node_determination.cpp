//
// Created by sandeep on 4/16/18.
//# All individual node infromation shall be in CSV formatted as "hostname,mp,cp
//#   hostname - Name of host where node is located
//#   mp - Main (command) port of that node. Also used in formatting unique
//#       path to file-server root dir. Peers talk amongst each other on
//#       this port.
//#   cp - Command port. Clients (non-peers) send messages to the server on
//#       this port to indicate vairous commands. These are detailed in the
//#       client program.
//#
//# The tracking server information shall be listed under name N0.
//# The tracking server shall also have a cp of 0.
//# Each node's respective environment variable name shall be formatted as
//#   an 'N' followed by a non-negative integer.
//#
//# All node relationships shall be in CSV formatted as lhs,rhs,latency.
//#   lhs and rhs are names of nodes. The number of lhs shall be less than rhs.
//#   No transitive relationships shall be allowed (eg. n0,n0,1000)
//# Each environment variable name indicating a relationship shall begin
//#   with "REL", and end in a non-negative integer.
//#
//
#include "node_determination.h"
#include "peer_info.h"
#include "sxfs.h"
#include <cstdlib>
#include <vector>

using std::vector;
using std::string;
using std::to_string;
using namespace std;

NodeDet::NodeDet() : nodename(""), hostname("") {//, client_port(0), server_port(0) {
}

NodeDet::NodeDet(int number) {
  nodenum = number;
    char *sh = NULL;
    int i = 0;
   nodename = "N" + to_string(number);
    sh = getenv(nodename.c_str());
    if (!sh) {
        nodenum = -1;
        nodename = "";
        hostname = "";
        client_port = 0;
        return;
    }
    vector <string> node_info = str_split(sh, ':');


    hostname = node_info[0];
    client_port = atoi(node_info[1].c_str());

    // Populate latency relationships
    char relainfo[MAXFILENAME];
    snprintf(relainfo, MAXFILENAME, "REL%d", i);
    sh = getenv(relainfo);

    while (sh) {
        vector <string> rel_info = str_split(sh, ',');


       int lat;
       string left,right;

        left =  rel_info[0];
        right = rel_info[1];
        lat = atoi(rel_info[2].c_str());

       if (strcmp(left.c_str(),nodenum.c_str()) == 0) {

         latencies[right] = lat;
       }
       else if (strcmp(right.c_str(),nodenum.c_str()) == 0) {
         latencies[left] = lat;

       }

       i++;
       snprintf(relainfo,64,"REL%d", i);
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
  // NodeDet *node = new NodeDet(getenv(N0));
  //   NodeDet *node = new NodeDet(getenv(N1));
  //   NodeDet *node = new NodeDet(getenv(N2));
  //   NodeDet *node = new NodeDet(getenv(N3));

}
