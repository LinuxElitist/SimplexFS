//
// Created by sandeep on 4/12/18.
//
// We can use this class/file for returning ip,port and calculating hash value for the value.

#include <iostream>
#include <sstream>
#include <string>
#include "string.h"
#include <cstdint>
#include <inttypes.h>
#include "peer_info.h"
using namespace std;

class PeerInfo;

class PeerInfo {

private:
    string ip;
    int port;

    PeerInfo(string ip, int port){
        //Have a check here for invalid ip or not
        this->ip = ip;
        this->port = port ;
    }
    string getIp() {
        return ip;
    }
    int getPort() {
        return port;
    }
    string toString() {
        stringstream ss;
        ss << getIp() << ":" << getPort();
        return ss.str();
    }

    //This is apparently fastest hash called djb2_hash so using it
    uint32_t HashCode(const uint8_t *str) {
        uint32_t hash = 5381;
        while (uint32_t c = static_cast<uint32_t>(*str++))
            hash = ((hash << 5) + hash ) + c ;
        return hash;
    }

    string md5sum(string filename) {
        unsigned char c[MD5_DIGEST_LENGTH];
        unsigned char data[BUFFER_SIZE+1];
        MD5_CTX ctx;
        FILE *fp = NULL;
        size_t bytes;

        fp = fopen(filename.c_str(), "r");
        if (!fp) {
            fprintf(stderr, "fopen error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        MD5_Init(&ctx);
        while ((bytes = fread(data, 1, BUFFER_SIZE, fp)) != 0)
            MD5_Update(&ctx, data, bytes);
        MD5_Final(c, &ctx);
        fclose(fp);
        stringstream sum;
        sum << c ;
        return sum.str();
    }

    bool checksum_test(string orig_checksum, string filename)
    {
        int i;
        string new_sum = md5sum(filename);
        return (strcmp(new_sum.c_str(),orig_checksum.c_str())==0);
//        for (i = 0; i < MD5_DIGEST_LENGTH; i++)
//            printf("%02x", new_sum[i]);
//        printf("\n");
    }

};