CXX=g++
CXXFLAGS=-std=c++0x -g -fpermissive
LIBS= -lnsl  -lpthread
RPCGEN_FILE= sxfs.x

SVC=sxfs_svc.cpp
CLI=sxfs_clnt.cpp
XDR=sxfs_xdr.cpp

SVC_SRC= $(XDR) $(SVC) sxfs_server.cpp
CLI_SRC=$(XDR) $(CLI) sxfs_client.cpp

CLIENT=clientside
SERVER=serverside

all: $(CLIENT) $(SERVER)

rpc:
	rpcgen -N $(RPCGEN_FILE)
	rename 's/\.c/.cpp/' sxfs_svc.c sxfs_clnt.c sxfs_xdr.c

$(CLIENT): $(CLI_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(SERVER): $(SVC_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(SVC) $(CLI) $(XDR) $(CLIENT) $(SERVER) sxfs.h