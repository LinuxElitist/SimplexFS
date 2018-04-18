const MAXIP = 16;
const MAXCLIENTS = 3;
const MAXFILENAME = 256;
const MAXFILES = 30;
const MAXFILELIST = 7680; /*256*30*/

typedef string IP<MAXIP>;
typedef string client_file_list<MAXFILELIST>;

struct node {
    IP ip;
    int port;
};
typedef node node_list<MAXCLIENTS>;

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