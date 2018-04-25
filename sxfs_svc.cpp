/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "sxfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <iostream>
#include <string>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static node_list *
_file_find_1 (char * *argp, struct svc_req *rqstp)
{
	return (file_find_1_svc(*argp, rqstp));
}

static int *
_update_list_1 (update_list_1_argument *argp, struct svc_req *rqstp)
{
	return (update_list_1_svc(argp->arg1, argp->arg2, argp->arg3, argp->arg4, rqstp));
}

static int *
_remove_client_1 (remove_client_1_argument *argp, struct svc_req *rqstp)
{
	return (remove_client_1_svc(argp->arg1, argp->arg2, rqstp));
}

static void
simple_xfs_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		char *file_find_1_arg;
		update_list_1_argument update_list_1_arg;
		remove_client_1_argument remove_client_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case FILE_FIND:
		_xdr_argument = (xdrproc_t) xdr_wrapstring;
		_xdr_result = (xdrproc_t) xdr_node_list;
		local = (char *(*)(char *, struct svc_req *)) _file_find_1;
		break;

	case UPDATE_LIST:
		_xdr_argument = (xdrproc_t) xdr_update_list_1_argument;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) _update_list_1;
		break;

	case REMOVE_CLIENT:
		_xdr_argument = (xdrproc_t) xdr_remove_client_1_argument;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) _remove_client_1;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "Usage: ./serverside server_port\n";
		exit(1);
	}

	int server_port = std::stoi(argv[1]);

	register SVCXPRT *transp;

	pmap_unset (SIMPLE_XFS, SIMPLE_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, SIMPLE_XFS, SIMPLE_VERSION, simple_xfs_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (SIMPLE_XFS, SIMPLE_VERSION, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, SIMPLE_XFS, SIMPLE_VERSION, simple_xfs_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (SIMPLE_XFS, SIMPLE_VERSION, tcp).");
		exit(1);
	}

    std::thread ping_thread = std::thread(ping);
    std::thread s_fault_check_thread = std::thread(s_fault_check, server_port);

    s_fault_check_thread.detach();
    ping_thread.detach();
	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
