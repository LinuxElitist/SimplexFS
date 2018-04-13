const MAXIP = 16;
const MAXCLIENTS = 10;
const MAXFILENAME = 256;
const MAXFILES = 20;

typedef string IP<MAXIP>;
typedef string FILENAME<MAXFILENAME>;
typedef string CHECKSUM<MD5_DIGEST_LENGTH>;

struct node {
    IP ip;
    int port;
    files_in_client client_file_list;
};
typedef node client_list<MAXCLIENTS>;

struct file_details {
    FILENAME filename;
    CHECKSUM checksum;
};
typedef file_details files_in_client<MAXFILES>;

struct file_in_server{
    FILENAME filename;
    client_list file_specific_client_list;
};
typedef file_in_server server_file_list<MAXFILES>;

program SIMPLE_XFS {
    version SIMPLE_VERSION {
        client_list FILE_FIND(string) = 1;
        int GET_LOAD(string,int) = 2;
        string DOWNLOAD(string) = 3;
        files_in_client UPDATE_LIST() = 4;
        int JOIN_CLIENT(IP, int) = 5;
    } = 1 ;
} = 0x22221122;