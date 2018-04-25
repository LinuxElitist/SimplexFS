# SimplexFS
Project for Distributed system course

To compile (RPCGEN has already been used to generate files, so that compilation can be ignored):
```
make clean
make
```

How to use:
````
Server: Launch the RPC server with: ./serverside server_port
Client: Launch the client application with: ./clientside “local ip” “server ip” "client_tcp_port" "client_crash_check_port" "server_crash_check_port"
````
Assumptions: 
````
1) The filenames do not contain any white spaces 
2) All filenames are unique
3) Hidden files are not considered in the filesystem
4) A client’s file list is updated on server side whenever any file is downloaded or every 30 sec
5) Server tries to connect to each client every 5 sec, in order to check if any client crashes
6) Clients try to connect to server via tcp on a separate port to make sure that server did not crash
7) Maximum number of clients id 4. But system is scalable, only that test.sh file will have to be manually upgraded with latencies
8) Files to be transferred are all located at a static mount point inside the project folder
````
Options on client side (find_file | download | get_load | update_list | remove_client) where each has its function as follows -
````
1) find_file will returns the list of nodes which store a file
2) download will download a given file; once downloaded it becomes shareable from that peer
3) get_load returns the load at a given peer requested from another peer
4) update_list provides the list of files stored at a given peer to the server
5) remove_client informs server to remove client's entry from list and disconnects the client from server and peers
````