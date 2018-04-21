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
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <cstring>

using std::to_string;
using namespace std;

class PeerInfo;

PeerInfo::PeerInfo(int num) {
char *n = NULL;
int i =0 ;
string node_name = "N" + to_string(num);
n = getenv(node_name.c_str());

while(n) {

nodes.push_back(new NodeDet(num));
i++;
node_name = "N" + to_string(i);
n = getenv(node_name.c_str());

}
}
NodeDet PeerInfo::getNode(int num) const {

return *nodes[num];

}

int PeerInfo::getNumNodes() const {

return nodes.size();

}

PeerInfo::PeerInfo(string ip, int port) {

  //Have a check here for invalid ip or not
  this->ip = ip;
  this->port = port;
}

// string getIp() {
//         return ip;
//     }
//
// int getPort() {
//         return this->port;
//     }
//
// string toString() {
//         stringstream ss;
//         ss << getIp() << ":" << getPort();
//         return ss.str();
//     }


//This is apparently fastest hash called djb2_hash so using it
uint32_t HashCode(const uint8_t *str) {
  uint32_t hash = 5381;
  while (uint32_t c = static_cast<uint32_t>(*str++))
  hash = ((hash << 5) + hash) + c;
  return hash;
}

string md5sum(string filename) {
  unsigned char c[MD5_DIGEST_LENGTH];
  unsigned char data[BUFFER_SIZE + 1];
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
  sum << c;
  return sum.str();
}

bool checksum_test(string orig_checksum, string filename) {
  int i;
  string new_sum = md5sum(filename);
  return (strcmp(new_sum.c_str(), orig_checksum.c_str()) == 0);
  //        for (i = 0; i < MD5_DIGEST_LENGTH; i++)
  //            printf("%02x", new_sum[i]);
  //        printf("\n");


}

PeerInfo::PeerInfo(const std::string &name) {


  int fd = open(name.c_str(),O_RDONLY);
  if (fd < 0) {
    return;
  }
  struct stat stat_buf;
  fstat(fd,&stat_buf);
  size = stat_buf.st_size;
  buf  = new unsigned char[size];
  if (read(fd,buf,size) != size) {
    close(fd);

    if (buf) {

      delete[] buf;
      buf = NULL;
    }
    size = 0;
    return;

  }

  //md5sum(name); // Ok ?

}

const unsigned char *PeerInfo::content() const {
  return (const unsigned char *)buf;
}

int PeerInfo::gsize() {
  return size;
}

bool PeerInfo::hasContent() const {

  return buf != NULL;
}
