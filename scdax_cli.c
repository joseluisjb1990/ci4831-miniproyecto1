#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>     /* for atoi() and exit() */
#include <string.h>     /* for memset() */
#include <unistd.h>     /* for close() */
#include <getopt.h>
#include <time.h>
#include <ctype.h>


#define MES_SIZE 450
#define FILE_SIZE 401 
void DieWithError(char *errorMessage);  /* Error handling function */
void write_file_process(char* buffer, char* file);


/*
 * Método que construye el mensaje de acuerdo al protocolo, que será enviado al
 * servidor
 * @param encrypt Indica si el mensaje va a ser cifrado o descifrado
 * @param key Clave a utilizar para el cifrado
 * @param address Desplazamiento de cifrado
 * @param mes Mensaje a cifrar o descifrar
 * @param ret_buffer Mensaje construido
 */
void build_message(int encrypt, char* key, char* address, char* mes, char* ret_buffer)
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%Y %H:%M:%S",tlocal);

    if (encrypt == 1) strcpy(ret_buffer, "CIF\n");
    else strcpy(ret_buffer, "DES\n");

    strcat(ret_buffer, "key: ");
    strcat(ret_buffer, key);
    strcat(ret_buffer, "\n");
    strcat(ret_buffer, "address: ");
    if(strcmp(address, "izquierda") == 0) strcat(ret_buffer, "i"); else strcat(ret_buffer, "d");
    strcat(ret_buffer, "\n");
    strcat(ret_buffer, "time: ");
    strcat(ret_buffer, output);
    strcat(ret_buffer, "\n");
    strcat(ret_buffer, "");
    printf("%d\n", (int) strlen(ret_buffer));
    strcat(ret_buffer, mes);
}


/*
 * Metodo que interpreta la respuesta del servidor y lo escribe en el archivo
 * @param response Respuesta del servidor
 * @param fileProcess Nombre del archivo
 */
void parse_response(char* response, char* fileProcess)
{
  char* auxToken = strtok(response, "\n"); 
  int code = strtol(auxToken, NULL, 10);

  switch(code) {
    case 100:
      auxToken = strtok(NULL, "\n"); //Parseamos la linea de la hora y el dia y la descartamos.
      auxToken = strtok(NULL, "\0"); //En auxToken queda el mensaje encriptado
      write_file_process(auxToken, fileProcess);
      break;
    case 200:
      printf("ERROR %d: %s\n", code, "El mensaje enviado por el cliente no sigue las reglas definidas por el protocolo");
      break;
    case 300:
      printf("ERROR %d: %s\n", code, "El mensaje no pudo ser cifrado correctamente");
      break;
    case 301:
      printf("ERROR %d: %s\n", code, "El mensaje no pudo ser descifrado correctamente");
      break;
    case 302:
      printf("ERROR %d: %s\n", code, "El mensaje no pudo ser descifrado correctamente");
      break;
    case 400:
      printf("ERROR %d: %s\n", code, "El mensaje que se intentó descifrar no fue cifrado por el servidor al que se consultó");
      break;
    case 900:
      printf("ERROR %d: %s\n", code, "Ocurrió un error grave en el servidor");
      break;
    default:
      printf("%d\n", code);
  }
}


/*
 * Procedimiento que escribe un archivo
 * @param buffer Mensaje a ser insertado en el archivo
 * @param file Nombre del archivo
 */
void write_file_process(char* buffer, char* file)
{
  FILE *fp = fopen(file, "w");
  if(fp == NULL) DieWithError("ERROR FATAL: Ocurrio un error abriendo el archivo");
  fputs(buffer, fp);
  fclose(fp);
}


/*
 * Metodo que lee un archivo
 * @param buffer Contiene la información leída del archivo
 * @param file Nombre del archivo
 */
void read_file_process(char* buffer, char* file)
{
  FILE *fp = fopen(file, "r");
  char c;
  int i;

  if(fp == NULL) DieWithError("ERROR FATAL: Ocurrio un error abriendo el archivo");

  i = 0;
  while((c = getc(fp)) != EOF)
    buffer[i++] = c;
  
  if(i == 0) DieWithError("ERROR 501: El mensaje que se intenta cifrar está vacío");
  else if (i > 400) DieWithError("ERROR 500: El mensaje que se intenta cifrar es muy grande");
  else buffer[i] = '\0';

  fclose(fp);
}


/*
 * Metodo principal
 * @param argc Tamaño del comando ejecutado
 * @param argv Contiene el comando ejecutado
 */
int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char echoString[FILE_SIZE];    /* String to send to echo server */
    char out_buffer[MES_SIZE]; 
    unsigned int echoStringLen;      /* Length of string to echo */
    int bytesRcvd;   /* Bytes read in single recv() 
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
    while((option = getopt(argc, argv,"i:c:a:f:p:")) != -1) 
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
                if ( (key < 1) || (key > 26) )
                    DieWithError("ERROR: EL VALOR SEGUIDO DE [-c] DEBE ESTAR COMPRENDIDO ENTRE 1 Y 27");
                break;
            case 'a':
                dirCifrado = optarg;     
                 if ( !((strcmp("derecha",dirCifrado)==0) || (strcmp("izquierda",dirCifrado)==0)) )
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


    read_file_process(echoString, nombreArchivoProcesar);

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("ERROR FATAL: Falló la creación del socket");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    int intentos = 0;

    while(intentos <= 3)
      if(intentos == 3)
        DieWithError("ERROR FATAL: No pudo ser realizada la conexion con el servidor");
      else if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
      {
        printf("%s\n%s\n", "Intento fallido de conexion con el servidor", "Intentando de nuevo en 5 segundos");
        intentos++;
        sleep(5);
      } else
        break;

    int mode;
    if(isalpha(echoString[0])) mode = 1;
    else mode = 0;

    build_message(mode, longClave, dirCifrado, echoString, out_buffer);
    echoStringLen = strlen(out_buffer);          /* Determine input length */

    /* Send the string to the server */
    if (send(sock, out_buffer, echoStringLen, 0) != echoStringLen)
        DieWithError("ERROR FATAL: El requerimiento no pudo ser enviado al servidor correctamente");

    if ((bytesRcvd = recv(sock, out_buffer, MES_SIZE - 1, 0)) <= 0)
        DieWithError("ERROR FATAL: La respuesta no pudo ser recibida correctamente por el cliente");

    out_buffer[bytesRcvd] = '\0';  /* Terminate the string! */
    
    parse_response(out_buffer, nombreArchivoProcesar);  /* Print a final linefeed */

    close(sock);
    exit(0);
}
