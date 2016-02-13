#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <getopt.h>


/**
 * Este archivo define todo lo correspondiente al servidor
 * @author: Jose Luis Jimenez y Ramon Marquez
 */


#define MAXPENDING 5    /* Maximum outstanding connection requests */

void DieWithError(char *errorMessage);  /* Error handling function */
void HandleTCPClient(int clntSocket, int id);   /* TCP client handling function */

int main(int argc, char *argv[])
{
    int servSock;                    /* Socket descriptor for server */
    int clntSock;                    /* Socket descriptor for client */
    struct sockaddr_in echoServAddr; /* Local address */
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned short echoServPort;     /* Server port */
    unsigned int clntLen;            /* Length of client address data structure */
    char *archivoBitacora;

    /* Puerto por defecto */
    echoServPort = 20849;

    /* Verificacion de argumentos */
    if (argc < 3 || argc > 5)
        DieWithError("ERROR: Cantidad de argumentos invalidos.\n"
        "   Introduzca: scdax_svr [-l <puerto_scdax_svr>] -b <archivo_bitacora>");

    int option = 0;
    /* Caso en que la cantidad de argumentos es de 9 */
    while((option = getopt(argc, argv,"l:b")) != -1) 
    {
        switch (option)
        {
            case 'b':
                archivoBitacora = optarg;
                break;
            case 'l':
                if (!(echoServPort = atoi(optarg)))
                    DieWithError("ERROR: EL VALOR SEGUIDO DE [-l] DEBE SER UN NUMERO DE PUERTO");
                break;
            case '?':
                DieWithError("ERROR: Argumentos invalidos.\n"
                "   Introduzca: scdax_svr [-l <puerto_scdax_svr>] -b <archivo_bitacora>");
                break;
        }
    }
    
    


    /* Create socket for incoming connections */
    if ((servSock = tcp_socket()) < 0)
        DieWithError("socket() failed");

    /* Bind to the local address */
    if ((tcp_bind(echoServPort, servSock)) < 0)
        DieWithError("bind() failed");

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
        DieWithError("listen() failed");

    srand(time(NULL));

    int id = 33 + rand() % 30;
    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        clntLen = sizeof(echoClntAddr);

        /* Wait for a client to connect */
        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, 
                               &clntLen)) < 0)
            DieWithError("accept() failed");

        /* clntSock is connected to a client! */

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
        
        HandleTCPClient(clntSock, id);
    }
    /* NOT REACHED */
}
