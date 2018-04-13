const MAXIP = 16;
const MAXCLIENTS = 10;
const MAXFILENAME = 256;
const MAXFILES = 20;

typedef string IP<MAXIP>;
typedef string FILENAME<MAXFILENAME>;

struct node {
    IP ip;
    int port;
};
typedef node node_list<MAXCLIENTS>;

typedef FILENAME client_file_list<MAXFILES>;
struct client_details {
    node client_node;
    client_file_list c_file_list;
};
typedef client_details client_list<MAXCLIENTS>;

program SIMPLE_XFS {
    version SIMPLE_VERSION {
        node_list FILE_FIND(string) = 1;
        int UPDATE_LIST(IP, int, client_file_list) = 2;
    } = 1 ;
} = 0x22221122;