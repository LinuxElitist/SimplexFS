# SimplexFS
Project for Distributed system course

To compile:
```
make clean
make rpc
make
```

How to use:
````
Server: Launch the RPC server with: ./serverside
Client: Launch the client application with: ./clientside “local ip” “server ip” "client_udp_port"
````
Assumptions: 
````
The filenames do not contain any white spaces
All filenames are unique
Hidden files are not considered in the filesystem
````
Options on client side (find_file | download | get_load | update_list) where each has its function as follows -

1) find_file will returns the list of nodes which store a file
2) download will download a given file; once downloaded it becomes shareable from that peer
3) get_load returns the load at a given peer requested from another peer
4) update_list provides the list of files stored at a given peer to the server
