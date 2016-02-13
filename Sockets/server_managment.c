#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdio.h>      /* for printf() and fprintf() */
#include <string.h>     /* for memset() */

FILE *fp;

int tcp_socket()
{
  int servSock = 0; 
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    printf("socket() failed");
  else
    printf("%s\n", "El socket TCP fue creado correctamente\n");
  return servSock;
}

int tcp_bind(unsigned short echoServPort, int servSock)
{
  struct sockaddr_in echoServAddr;
  /* Construct local address structure */
  memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
  echoServAddr.sin_family = AF_INET;                /* Internet address family */
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
  echoServAddr.sin_port = htons(echoServPort);      /* Local port */
  
  /* Bind to the local address */
  int r = bind(servSock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr));
  
  if(r < 0) printf("%s\n", "Ocurrio un error realizando la conexion al puerto");
  else printf("%s\n", "Conexion al puerto realizada satisfactoriamente");
  return r;
}

void open_log(char* nameLog)
{
  fp = fopen(nameLog, "w");
}

void close_log(char* nameLog)
{
  fclose(fp);
}

void write_log(char* cliente, char* actividad, char* mensaje)
{
  char buffer[128];
  if(fp == NULL) DieWithError("ERROR FATAL: Ocurrio un error abriendo el archivo");
  fputs(buffer, fp);
  fclose(fp);
}
