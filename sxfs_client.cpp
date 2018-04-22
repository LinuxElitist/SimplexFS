/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "sxfs.h"
#include "string.h"
#include "stdlib.h"
#include <unistd.h>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <thread>
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include "tcp_client.h"
#include "tcp_server.h"
#include "tcp_communication.h"
#include "peer_info.h"
#include "node_determination.h"
#include <mutex>
#include <fstream>

#define FS_ROOT "./5105_node_files"

using namespace std;
using std::thread;

class Client;

class Client {

public:
    //thread related
    std::thread tcp_thread;
    std::thread heartbeat_thread;
    std::thread update_thread;
    bool update_flag = false;
    bool heartbeat_flag = false;
    bool tcp_flag = false;
    void update_thread_func();
    void heartbeat();
    void tcp_thread_func();

    //class fields
    CLIENT *clnt;
    char *self_ip;
    int self_port;
    int client_number;
    int num_active_clients;
    client_file_list self_file_list;
    node_list *peers_with_file;
    map<int, pair<char *, int>> peer_load; //(load,pair<ip,port>)
    TcpClient *tcp_clnt; //create self as tcp client and serv doe peer operations
    TcpServer *tcp_serv;

    //class methods
    void file_find(char *filename);
    int get_load(char * peer_ip, int peer_port);
    void download(char *filename);
    void populate_file_list();
    void update_list();
    void remove_client();
    int ping();
    void download_file_helper();

    static bool compare_first(const std::pair<int, pair<char *, int> > &lhs, const std::pair<int, pair<char *, int> > &rhs);
    static bool compare_equal(const std::pair<int, pair<char *, int> > &lhs, const std::pair<int, pair<char *, int> > &rhs);

    Client(char *ip, char *host, int port) {
        self_ip = ip;
        self_port = port;
        clnt = clnt_create(host, SIMPLE_XFS, SIMPLE_VERSION, "udp");
        if (clnt == NULL) {
            clnt_pcreateerror(host);
            exit(1);
        }
        update_list();
        std::cout << ".....Completed client creation.....\n";
        tcp_serv = new TcpServer(self_port, MAXCLIENTS);

        heartbeat_flag = true;
        heartbeat_thread = thread(&Client::heartbeat, this);
        update_flag = true;
        update_thread = thread(&Client::update_thread_func,this);
        tcp_flag = true;
        tcp_thread = thread(&Client::tcp_thread_func, this);
    }

    ~Client() {
        remove_client();
        if (tcp_thread.joinable()) {
            tcp_thread.join();
        }
        if (update_thread.joinable()) {
            update_thread.join();
        }
        if (heartbeat_thread.joinable()) {
            heartbeat_thread.join();
        }
        if (clnt)
            clnt_destroy(clnt);
    }
};

bool Client::compare_first(const std::pair<int, pair<char *, int> > &lhs,
                           const std::pair<int, pair<char *, int> > &rhs) {
    return lhs.first < rhs.first;
}

bool Client::compare_equal(const std::pair<int, pair<char *, int> > &lhs,
                           const std::pair<int, pair<char *, int> > &rhs) {
    return lhs.first == rhs.first;
}

void Client::download_file_helper() {
    char **file_to_download;
    tcp_serv->servRead(client_number, file_to_download);
//    cout << "File to download: " << *file_to_download << endl;
    ifstream peer_file(*file_to_download, ifstream::binary);
    streampos size;
    char *peer_file_contents;
    if (peer_file.is_open()) {
        peer_file.seekg(0, peer_file.end);
        size = peer_file.tellg();
        peer_file.seekg(0, peer_file.beg);
        peer_file_contents = new char[size];
        peer_file.read(peer_file_contents, size);
        peer_file.close();
        peer_file_contents[size] = '\0';
//        cout << "write: " << peer_file_contents << endl;
        if (tcp_serv->servWrite(client_number, peer_file_contents, size) != size) {
            cout << "Unable to transfer full file" << endl;
        }
        delete[] peer_file_contents;
    } else {
        cout << "Unable to open " << file_to_download << " So can not transfer the file contents" << endl;
    }
}

void Client::heartbeat() {
    while(heartbeat_flag) {
        sleep(5);
        ping();
    }
}

void Client::update_thread_func() {
    while(update_flag) {
        sleep(60);
        update_list();
    }
}

void Client::tcp_thread_func() {
    tcp_serv->servListen();
    while(tcp_flag){
        client_number = tcp_serv->servAccept();
        num_active_clients = tcp_serv->getNumActiveClients();
        if(strcmp((tcp_serv->download_flag),"true")==0){
            strcpy(tcp_serv->download_flag, "false");
            download_file_helper();
        }
        tcp_serv->servClose(client_number);
    }
}

void Client::populate_file_list() {
    char temp_list[MAXFILELIST];
    temp_list[0] = '\0';
    int client_num_files = 0;
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(FS_ROOT)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (strncmp(entry->d_name, ".", 1) != 0) { //ignoring this directory , parent directory and hidden files
                client_num_files++;
                strcat(temp_list, entry->d_name);
                strcat(temp_list, " "); //appending space for distinguishing filenames in the list
            }
        }
    }
    closedir(dir);
    cout << "populating file list on client side: " << temp_list << endl;
    self_file_list = (client_file_list) temp_list;
}

void Client::file_find(char *filename) {
    peers_with_file = file_find_1(filename, clnt);
    if (peers_with_file == (node_list *) NULL) {
        clnt_perror(clnt, "call failed");
    } else {
        cout << "Node_list for " << filename << " is:\n";
        for (int i = 0; i < peers_with_file->node_list_len; i++) {
            cout << (peers_with_file->node_list_val + i)->ip << ":" << (peers_with_file->node_list_val + i)->port << endl;
        }
        cout << "\n";
    }
}

int Client::get_load(char * peer_ip, int peer_port) {
    char *temp_load;
    int load = 0;
    if ((strcmp(self_ip, peer_ip) != 0) || (self_port != peer_port)) {
        tcp_clnt = new TcpClient(peer_ip,peer_port);
        tcp_clnt->clntOpen();
        tcp_clnt->clntRead(&temp_load);
        string send_download_flag = "false";
        tcp_clnt->clntWrite(send_download_flag.c_str(),send_download_flag.length());
        tcp_clnt->clntClose();
        free(tcp_clnt);
        load = atoi(temp_load);
//        cout << "read load " << load << "\n";
    } else {
        //create self as tcp client and servr does peer operations
        load = num_active_clients;
//        cout << "read self load " << load << "\n";
    }
    return load;
}

void Client::download(char *filename) {
    file_find(filename);
    std::vector<string> minEqualLoad;
    string temp_nodename= "";
    if (peers_with_file->node_list_len == 0) {
        cout << "File does not exist" << endl;
    } else {
        char *dest_ip;
        int dest_port;

        int temp_load;
        map < int, pair < char *, int >>::iterator load_itr = peer_load.begin();
        for (int i = 0; i < peers_with_file->node_list_len; i++) {
            temp_load = get_load((peers_with_file->node_list_val+i)->ip, (peers_with_file->node_list_val+i)->port);
            peer_load.insert(load_itr, std::pair < int, pair < char * , int >>
                                                (temp_load, make_pair((peers_with_file->node_list_val+i)->ip,
                                                                      (peers_with_file->node_list_val+i)->port)));
            load_itr++;
        }
        load_itr = min_element(peer_load.begin(), peer_load.end(), this->compare_first);
        int min_load = load_itr->first;

        //TODO: what if similar loads
//        map< int, pair < char *, int >>::iterator find_itr;
//        find_itr = peer_load.find(min_load);
//        temp_nodename = find_itr->second.first;
//        temp_nodename.append(to_string(find_itr->second.second));
//        minEqualLoad.push_back(temp_nodename);
//        find_itr++;
//        while(find_itr != peer_load.end()) {
//            if (find_itr == peer_load.find(min_load)){
//                temp_nodename = find_itr->second.first;
//                temp_nodename.append(to_string(find_itr->second.second));
//                minEqualLoad.push_back(temp_nodename);
//                find_itr++;
//            }
//        }
//        temp_nodename = self_ip;
//        temp_nodename.append(to_string(self_port));

        dest_ip = load_itr->second.first;
        dest_port = load_itr->second.second;
        char *clnt_file_contents;
        string file_to_download = FS_ROOT;
        string send_download_flag = "true";
        file_to_download.append("/");
        file_to_download.append(filename);
        //if ip and port same as self, i.e. act like a server
        if((strcmp(self_ip,dest_ip)!=0) || (self_port != dest_port)){
            tcp_clnt = new TcpClient(dest_ip,dest_port);
            char *temp;
            tcp_clnt->clntOpen();
            tcp_clnt->clntRead(&temp);
            tcp_clnt->clntWrite(send_download_flag.c_str(),send_download_flag.length());

            tcp_clnt->clntWrite(file_to_download.c_str(),file_to_download.length());
            tcp_clnt->clntRead(&clnt_file_contents);
//            cout << "reading contents: " << clnt_file_contents << endl;
            send_download_flag = "false";
            tcp_clnt->clntWrite(send_download_flag.c_str(),send_download_flag.length());
            tcp_clnt->clntClose();
            // create a file and write these contents

            ofstream client_file(file_to_download.c_str());
            if(client_file.is_open()){ //if able to open the file
                client_file << clnt_file_contents ;
//                cout << "content received: " << clnt_file_contents << endl;
                client_file.close();
                //TODO: update list to be called if checksum returned success
                update_list();
            }
            else{
                cout << "Unable to create " << filename << endl;
            }
        }
        else{
            ifstream fptr(file_to_download);
//            cout << "name file: " << file_to_download <<endl;
            if(fptr.good()==0){
                cout << "File already exists" << endl; //not overwriting as not considering consistency issues
            }
        }

        //TODO: implement latency in sending
        //calculate checksum of downloaded file
        //compare with original file and output success if checksum matches
    }
}

void Client::update_list() {
    populate_file_list();
    auto result_4 = update_list_1(self_ip, self_port, self_file_list, clnt);
    if (*result_4 == -1) {
        clnt_perror(clnt, "call failed");
    }
}

void Client::remove_client(){
    auto result_5 = remove_client_1(self_ip, self_port, clnt);
    if (*result_5 == -1) {
        clnt_perror(clnt, "call failed");
    }
    tcp_flag = false;
    update_flag = false;
    heartbeat_flag = false;
}

int Client::ping() {
    auto output = ping_1(clnt);
    if (output == NULL) {
        clnt_perror(clnt, "Cannot ping server");
        return 1;
    } //only print to the user if there is failure

    //if peer crashed
    //TODO: remove client from file_specific_client_list and then call update_list
    return *output;
}

//TODO: scenario of a client leaving and then joining back cz we need checksum too

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "Usage: ./clientside client_ip server_ip client_port\n";
        exit(1);
    }
    char *client_ip = (char *) argv[1];
    char *serv_ip = (char *) argv[2];
    int self_port = stoi(argv[3]);

    Client conn(client_ip, serv_ip, self_port);
    char func[1];
    int func_number;
    char search_filename[MAXFILENAME];
    while (1) {
        std::cout << "Please enter what function you want to perform [1-5]:\n"
                  << "Function description\n1 file_find\n2 download\n3 get_load\n4 update_list\n5 remove_client\n";
        std::cin >> func;
        try {
            func_number = stoi(func);
        }
        catch (std::exception &e) {
            cout << "ERROR:  Please limit operation values from 1-4 " << endl;
            continue;
        }
        switch (func_number) {
            case 1:
                std::cout << "Please enter the filename to be searched:\n";
                std::cin.get();
                std::cin.getline(search_filename, MAXFILENAME);
                conn.file_find(search_filename);
                break;
            case 2:
                std::cout << "Please enter the filename to be downloaded:\n";
                std::cin.get();
                std::cin.getline(search_filename, MAXFILENAME);
                conn.download(search_filename);
                break;
            case 3:
                char dest_ip[MAXIP];
                int dest_port;
                char temp_port[4];
                std::cout << "Please enter hostname:\n";
                std::cin >> dest_ip;
                std::cout << "Please enter hostname:\n";
                std::cin >> temp_port;
                try {
                    dest_port = stoi(temp_port);
                }
                catch (std::exception &e) {
                    cout << "ERROR:  invalid port number " << endl;
                    continue;
                }
                conn.get_load(dest_ip,dest_port);
                break;
            case 4:
                conn.update_list();
                break;
            case 5:
                conn.remove_client();
                exit(0);
                break;
            default:
                std::cout << "Wrong format specified. Please retry \n";
                break;
        }
    }
}
