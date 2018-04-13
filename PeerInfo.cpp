//
// Created by sandeep on 4/12/18.
//
// We can use this class/file for returning ip,port and calculating hash value for the value.

#include <iostream>
#include <string>
#include <stdint>
#include <inttypes.h>

class PeerInfo {

private:
    string ip;
    int port;

    PeerInfo(string ip, int port){
        //Have a check here for invalid ip or not

        this.ip = ip;
        this.port = port ;
    }
    string getIp() {

        return ip;

    }
    int getPort() {

        return port;

    }
    string toString() {
        return ip + ":" + port;


    }

    //This is apparently fastest hash called djb2_hash so using it
    uint32_t HashCode(const uint8_t *str) {

        uint32_t hash = 5381;
        while (uint32_t c = static_cast<uint32_t>(*str++))
            hash = ((hash << 5) + hash ) + c ;
        return hash;


    }

};