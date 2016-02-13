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
void HandleTCPClient(int clntSocket, int id, char* ipcli);   /* TCP client handling function */

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
    while((option = getopt(argc, argv,"l:b:")) != -1) 
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
    
    create_log(archivoBitacora);

    /* Create socket for incoming connections */
    servSock = tcp_socket();

    /* Bind to the local address */
    tcp_bind(echoServPort, servSock);

    /* Mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
    {
      write_entry_log("listen", "ERROR FATAL: El socket no puede escuchar por el puerto especificado", "");
      DieWithError("ERROR FATAL: El socket no puede escuchar por el puerto especificado");
    } else
      write_entry_log("listen", "El servidor esta escuchando en el puerto especificado", "");

    srand(time(NULL));

    int id = 33 + rand() % 30;
    printf("%s\n", "Atendiendo solicitudes");

    for (;;) /* Run forever */
    {
        clntLen = sizeof(echoClntAddr);

        if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0)
        {
          write_entry_log("accept", "ERROR FATAL: Ocurrió un error aceptando peticiones de clientes", "");
          DieWithError("ERROR FATAL: Ocurrió un error aceptando peticiones de clientes");
        } else
        {
          write_entry_log("accept", "Procesando solicitud", inet_ntoa(echoClntAddr.sin_addr));
          HandleTCPClient(clntSock, id, inet_ntoa(echoClntAddr.sin_addr));
        }
    }
}
