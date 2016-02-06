#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <getopt.h>
#include <time.h>

#define RCVBUFSIZE 32   /* Size of receive buffer */

#define MES_SIZE 2048
void DieWithError(char *errorMessage);  /* Error handling function */

void build_message(int encrypt, char* key, char* address, char* mes, char* ret_buffer)
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%Y %H:%M:%S",tlocal);

    if (encrypt) strcpy(ret_buffer, "CIF\n");
    else strcpy(ret_buffer, "DES\n");

    strcat(ret_buffer, "key: ");
    strcat(ret_buffer, key); //ARREGLAR ESTO
    strcat(ret_buffer, "\n");
    strcat(ret_buffer, "address: ");
    strcat(ret_buffer, address);
    strcat(ret_buffer, "\n");
    strcat(ret_buffer, "time: ");
    strcat(ret_buffer, output);
    strcat(ret_buffer, "\n");
    strcat(ret_buffer, mes);
}

int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char *echoString;                /* String to send to echo server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                        and total bytes read */

    /* Otras variables para el cifrado */
    char *longClave;
    char *dirCifrado;
    char *nombreArchivoProcesar;


    /* Puerto por defecto */
    echoServPort = 20849;

    /* Verificamos argumentos */
    if (argc < 9 || argc > 11)
        DieWithError("ERROR: Cantidad de argumentos invalidos.\n"
        "   Introduzca: scdax_cli -i <dir_ip> [-p <puerto_scdax_svr>] -c <long_clave> -a <dir_cif> -f <archivo_a_procesar> ");


    int option = 0;
    while((option = getopt(argc, argv,"i:c:a:f:p")) != -1) 
    {
        switch (option)
        {
            case 'i':
                servIP = optarg;
                break;
            case 'c':
                longClave = optarg;
                int key;
                if (!(key=atoi(longClave)))
                    DieWithError("ERROR: EL VALOR SEGUIDO DE [-c] DEBE SER UN NUMERO ENTERO");
                if ( (key < 1) || (key > 27) )
                    DieWithError("ERROR: EL VALOR SEGUIDO DE [-c] DEBE ESTAR COMPRENDIDO ENTRE 1 Y 27");
                break;
            case 'a':
                dirCifrado = optarg;     
                if ( (strcmp("derecha",dirCifrado)!=0) || !(strcmp("izquierda",dirCifrado)!=0) )
                    DieWithError("ERROR: EL VALOR SEGUIDO DE [-a] DEBE TOMAR LOS VALORES \"izquierda\" O \"derecha\"");
                break;
            case 'f':
                nombreArchivoProcesar = optarg;
                break;
            case 'p':
                if (!(echoServPort = atoi(optarg)))
                    DieWithError("ERROR: EL VALOR SEGUIDO DE [-p] DEBE SER UN NUMERO DE PUERTO");
                break;
            case '?':
                DieWithError("ERROR: Argumentos invalidos.\n"
                "   Introduzca: scdax_cli -i <dir_ip> [-p <puerto_scdax_svr>] -c <long_clave> -a <dir_cif> -f <archivo_a_procesar> ");
                break;
        }
    }



    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed");

    // EN LA VARIABLE ECHOSTRING DEBERIA TENER EL MENSAJE A CIFRAR
    char out_buffer[MES_SIZE]; 
    // SI DEJO EL ECHOSTRING DA VIOLACION DE SEGMENTO ASI QUE MODIFICAR ESTO
    //build_message(1, longClave, dirCifrado, echoString, out_buffer);
    build_message(1, longClave, dirCifrado, "AQUI VA EL MENSAJE", out_buffer);
    echoStringLen = strlen(out_buffer);          /* Determine input length */

    printf("%s\n", out_buffer);

    /* Send the string to the server */
    if (send(sock, out_buffer, echoStringLen, 0) != echoStringLen)
        DieWithError("send() sent a different number of bytes than expected");

    /* Receive the same string back from the server */
    totalBytesRcvd = 0;
    printf("Received: ");                /* Setup to print the echoed string */
    while (totalBytesRcvd < echoStringLen)
    {
        /* Receive up to the buffer size (minus 1 to leave space for
           a null terminator) bytes from the sender */
        if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
        echoBuffer[bytesRcvd] = '\0';  /* Terminate the string! */
        printf("%s", echoBuffer);      /* Print the echo buffer */
    }

    printf("\n");    /* Print a final linefeed */

    close(sock);
    exit(0);
}

