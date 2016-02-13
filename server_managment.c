#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char* NAMELOG;
FILE* FP;

void create_log(char* nameLog)
{
  NAMELOG = nameLog;
}

void open_log()
{
  FP = fopen(NAMELOG, "a");
}

void close_log()
{
  fclose(FP);
}

void write_log(char* message)
{
  open_log();
  fputs(message, FP);
  close_log();
}

void write_entry_log(char* activity, char* message, char* ipcli)
{
  time_t tiempo = time(0);
  struct tm *tlocal = localtime(&tiempo);
  char output[128];
  strftime(output,128,"%d/%m/%Y %H:%M:%S",tlocal);
 
  write_log("[");
  write_log(output);
  write_log("]");

  write_log("[");
  write_log(activity);
  write_log("]");

  write_log("[cliente");
  write_log(ipcli);
  write_log("] ");

  write_log(message);
  write_log("\n");
}

int tcp_socket()
{
  int servSock = 0; 
  if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
  {
    write_entry_log("conexion", "ERROR FATAL: No pudo ser creado el socket en el servidor", "");
    DieWithError("ERROR FATAL: No pudo ser creado el socket en el servidor");
  }
  else
  {
    printf("%s\n", "El socket TCP fue creado correctamente");
    write_entry_log("conexion", "El socket TCP fue creado correctamente", "");
  }
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
