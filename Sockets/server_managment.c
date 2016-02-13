#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


/**
 * Este archivo define todo lo correspondiente a las conexiones
 * cliente servidor
 * @author: Jose Luis Jimenez y Ramon Marquez
 */

char* NAMELOG; // Nombre del archivo bitácora o log
FILE* FP;


/*
 * Método que asigna el nombre del log
 * @param nameLog Nombre del log
 */
void create_log(char* nameLog)
{
  NAMELOG = nameLog;
}


/*
 * Método que abre el archivo log
 * @param 
 */
void open_log()
{
  FP = fopen(NAMELOG, "a");
}


/*
 * Método que cierra el archivo log
 * @param 
 */
void close_log()
{
  fclose(FP);
}


/*
 * Procedimiento que escribe en el log
 * @param message Mensaje escrito en el log
 */
void write_log(char* message)
{
  open_log();
  fputs(message, FP);
  close_log();
}


/*
 * Escribe una entrada en el log
 * @param 
 */
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

  write_log("[cliente ");
  write_log(ipcli);
  write_log("] ");

  write_log(message);
  write_log("\n");
}


/*
 * Conecta los sockets
 * @param 
 */
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


/*
 * Realiza el enlace con el cliente
 * @param 
 */
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
  
  if(r < 0) 
  {
    write_entry_log("conexion", "ERROR FATAL: Ocurrio un error realizando la conexion al puerto", "");
    DieWithError("ERROR FATAL: Ocurrio un error realizando la conexion al puerto");
  }
  else
  {
    write_entry_log("conexion", "Conexion al puerto realizada satisfactoriamente", "");
    printf("%s\n", "Conexion al puerto realizada satisfactoriamente");
  }
  return r;
}