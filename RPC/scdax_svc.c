/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "scdax.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
 #include <getopt.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
scdax_prog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		message encrypt_msg_1_arg;
		message decrypt_msg_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case ENCRYPT_MSG:
		_xdr_argument = (xdrproc_t) xdr_message;
		_xdr_result = (xdrproc_t) xdr_wrapstring;
		local = (char *(*)(char *, struct svc_req *)) encrypt_msg_1_svc;
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
	register SVCXPRT *transp;

	pmap_unset (SCDAX_PROG, SCDAX_VERS);

	char* archivoBitacora;

	/* Verificacion de argumentos */
    if (argc < 3 || argc > 3)
        DieWithError("ERROR: Cantidad de argumentos invalidos.\n"
        "   Introduzca: scdax_svr -b <archivo_bitacora>");

    int option = 0;
    /* Caso en que la cantidad de argumentos es de 9 */
    while((option = getopt(argc, argv,"b:")) != -1) 
    {
        switch (option)
        {
            case 'b':
                archivoBitacora = optarg;
                break;
            case '?':
                DieWithError("ERROR: Argumentos invalidos.\n"
                "   Introduzca: scdax_svr -b <archivo_bitacora>");
                break;
        }
    }

    /* Pasamos el archivo Bitacora */
    archivoBitacoraSVC = malloc(MES_SIZE * sizeof(char));
	archivoBitacoraSVC = archivoBitacora;

	// Generamos el random para el servidor. USAR MEJORAMIENTO DE JOSE LUIS
	srand(time(NULL));
	SERVERID = 33 + rand() % 30;

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, SCDAX_PROG, SCDAX_VERS, scdax_prog_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (SCDAX_PROG, SCDAX_VERS, tcp).");
		exit(1);
	}


	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
