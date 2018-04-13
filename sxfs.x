const MAXIP = 16;
const MAXSERVERS = 10;
const MAXFILENAME = 256;
const MAXFILES = 20;

typedef string IP<MAXIP>;
typedef string filename<MAXFILENAME>;
typedef filename file_list<MAXFILES>;

struct node {
    IP ip;
    int port;
};
typedef node server_list<MAXSERVERS>;

program SIMPLE_XFS {
    version SIMPLE_VERSION {
        server_list FileFind(string) = 1;
        int GetLoad(string,int) = 2;
        string Download(string) = 3;
        file_list UpdateList() = 4;
    } = 1 ;
} = 0x22221122;
