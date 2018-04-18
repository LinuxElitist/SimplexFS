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


#define BUFFER_SIZE 1024

using namespace std;

void outputClientList();
client_list buildClientList();

void md5sum(const char * const filename, unsigned char *sum);
//class Server{
//public:
//    vector<pair<pair<char *,int>,client_file_list *>> clientList;
//    client_file_list self_f_list;
//    int *update_list_1_svc(IP arg1, int arg2, client_file_list arg3, struct svc_req *rqstp);
//    node_list *file_find_1_svc(char *arg1, struct svc_req *rqstp);
//    Server();
//};
//extern Server *serv_ptr;


#endif //SIMPLEXFS_PEER_INFO_H
