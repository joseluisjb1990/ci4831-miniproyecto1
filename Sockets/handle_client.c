#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define RCVBUFSIZE 2450   /* Size of receive buffer */
#define OUTBUFSIZE 2431
#define LETTERSIZE 26
#define BACONSIZE 6


// Estructura utilizada para el mensaje que cifra y/o descifra del servidor
typedef struct {
  int mode;
  int cant;
  char* message;
} header;

// Caracteres utilizados para el cifrado/descrifrado
char* letters = "abcdefghijklmnopqrstuvwxyz";
// Identificador del servidor
char SERVERID;

void DieWithError(char *errorMessage);  /* Error handling function */


/*
 * Método que cifra un caracter
 * @param c Caracter a ser cifrado
 * @param shift Clave utilizada para cifrar
 * @return Caracter cifrado
 */
char encryipt_char(char c, int shift)
{
  if(isspace(c)) return c;
  if(c == '.') return c;
  for(int i = 0; i < LETTERSIZE; i++)
  {
    if(letters[i] == c) 
    { 
      int pos = (i + shift) % LETTERSIZE; if (pos < 0) pos = LETTERSIZE + pos;
      return letters[pos];
    }
  }
  return -1;
}


/*
 * Método que descifra un caracter
 * @param c Caracter a ser descifrado
 * @param shift Clave utilizada para descifrar
 * @return Caracter descifrado
 */
char decrypt_char(char c, int shift)
{
  if(isspace(c)) return c;
  if(c == '.') return c;
  for(int i = 0; i < LETTERSIZE; i++)
  {
    if(letters[i] == c) 
    { 
      int pos = (i - shift) % LETTERSIZE; if (pos < 0) pos = LETTERSIZE + pos;
      return letters[pos];
    }
  }
  return -1;
}


/*
 * Procedimiento que descifra utilizando el código Bacon
 * @param s String a ser descifrado
 * @return Caracter según el string s
 */
char decrypt_bacon_transform(char* s)
{
  if (strcmp("AAAAAA", s) == 0) return 'a';
  if (strcmp("AAAAAB", s) == 0) return 'b';
  if (strcmp("AAAABA", s) == 0) return 'c';
  if (strcmp("AAAABB", s) == 0) return 'd';
  if (strcmp("AAABAA", s) == 0) return 'e';
  if (strcmp("AAABAB", s) == 0) return 'f';
  if (strcmp("AAABBA", s) == 0) return 'g';
  if (strcmp("AAABBB", s) == 0) return 'h';
  if (strcmp("AABAAA", s) == 0) return 'i';
  if (strcmp("AABAAB", s) == 0) return 'j';
  if (strcmp("AABABA", s) == 0) return 'k';
  if (strcmp("AABABB", s) == 0) return 'l';
  if (strcmp("AABBAA", s) == 0) return 'm';
  if (strcmp("AABBAB", s) == 0) return 'n';
  if (strcmp("AABBBA", s) == 0) return 'o';
  if (strcmp("AABBBB", s) == 0) return 'p';
  if (strcmp("ABAAAA", s) == 0) return 'q';
  if (strcmp("ABAAAB", s) == 0) return 'r';
  if (strcmp("ABAABA", s) == 0) return 's';
  if (strcmp("ABAABB", s) == 0) return 't';
  if (strcmp("ABABAA", s) == 0) return 'u';
  if (strcmp("ABABAB", s) == 0) return 'v';
  if (strcmp("ABABBA", s) == 0) return 'w';
  if (strcmp("ABABBB", s) == 0) return 'x';
  if (strcmp("ABBAAA", s) == 0) return 'y';
  if (strcmp("ABBAAB", s) == 0) return 'z';
  if (strcmp("ABBABA", s) == 0) return '0';
  if (strcmp("ABBABB", s) == 0) return '1';
  if (strcmp("ABBBAA", s) == 0) return '2';
  if (strcmp("ABBBAB", s) == 0) return '3';
  if (strcmp("ABBBBA", s) == 0) return '4';
  if (strcmp("ABBBBB", s) == 0) return '5';
  if (strcmp("BAAAAA", s) == 0) return '6';
  if (strcmp("BAAAAB", s) == 0) return '7';
  if (strcmp("BAAABA", s) == 0) return '8';
  if (strcmp("BAAABB", s) == 0) return '9';
  if (strcmp("BAABAA", s) == 0) return '.';
  if (strcmp("BAABAB", s) == 0) return ' ';
  if (strcmp("BAABBA", s) == 0) return '\n';
  return -1;
}


/*
 * Procedimiento que cifra utilizando el código Bacon
 * @param s Caracter a ser cifrado
 * @return String según el caracter c
 */
char* bacon_transform(char c)
{
  switch(c) 
  {
    case 'a': return "AAAAAA";
    case 'b': return "AAAAAB";
    case 'c': return "AAAABA";
    case 'd': return "AAAABB";
    case 'e': return "AAABAA";
    case 'f': return "AAABAB";
    case 'g': return "AAABBA";
    case 'h': return "AAABBB";
    case 'i': return "AABAAA";
    case 'j': return "AABAAB";
    case 'k': return "AABABA";
    case 'l': return "AABABB";
    case 'm': return "AABBAA";
    case 'n': return "AABBAB";
    case 'o': return "AABBBA";
    case 'p': return "AABBBB";
    case 'q': return "ABAAAA";
    case 'r': return "ABAAAB";
    case 's': return "ABAABA";
    case 't': return "ABAABB";
    case 'u': return "ABABAA";
    case 'v': return "ABABAB";
    case 'w': return "ABABBA";
    case 'x': return "ABABBB";
    case 'y': return "ABBAAA";
    case 'z': return "ABBAAB";
    case '0': return "ABBABA";
    case '1': return "ABBABB";
    case '2': return "ABBBAA";
    case '3': return "ABBBAB";
    case '4': return "ABBBBA";
    case '5': return "ABBBBB";
    case '6': return "BAAAAA";
    case '7': return "BAAAAB";
    case '8': return "BAAABA";
    case '9': return "BAAABB";
    case '.': return "BAABAA";
    case ' ': return "BAABAB";
    case '\n': return "BAABBA";
    default: return NULL;
  }
}


/*
 * Método que genera un caracter aleatoriamente
 * @param c Caracter a ser cambiado
 * @return El caracter en mayúscula o minúscula 
 */
char random_char(char c)
{
  int cs = 97 + rand() % LETTERSIZE;

  if(c == 'A') return toupper(cs);
  else if (c == 'B') return cs;
  else return -1;
}


/*
 * Procedimiento que cifra un mensaje
 * @param msg Mensaje a ser cifrado
 * @param msn_size Entero que indica el tamaño del mensaje
 * @param outBuffer Mensaje cifrado
 * @param offset Clave usado para cifrar
 * @return el tamaño del mensaje final
 */
int encrypt_msg(char* msg, int msg_size, char* outBuffer, int offset)
{
  if(msg_size > 400) return -2;

  for(int i = 0; i < msg_size; i++)
    msg[i] = tolower(msg[i]);

  for(int i = 0; i < msg_size; i++)
    if ((msg[i] = encryipt_char(msg[i], offset)) == -1)
      return -1;
    
  int pos = 0;
  outBuffer[pos++] = SERVERID;
  for(int i = 0; i < msg_size; i++)
  {
    char* tchar; 
    if((tchar = bacon_transform(msg[i])) == NULL) return -1;
    for(int j = 0; j < BACONSIZE; j++)
      outBuffer[pos++] = tchar[j];  
  }

  for (int i = 1; i < pos; i++)
    if((outBuffer[i] = random_char(outBuffer[i])) == -1)
      return -1;
  
  outBuffer[pos] = '\0';
  return pos;
}


/*
 * Método que descifra un caracter
 * @param c Caracter a ser descifrado
 * @return A o B dependiendo de si el caracter es A o B
 */
char decrypt_random_char(char c)
{
  if(isupper(c)) return 'A';
  else if (!isupper(c)) return 'B';

  return -1;
}


/*
 * Procedimiento que descifra un mensaje
 * @param msg Mensaje a ser descifrado
 * @param msn_size Entero que indica el tamaño del mensaje
 * @param outBuffer Mensaje descifrado
 * @param offset Clave usado para descifrar
 * @return el tamaño del mensaje final
 */
int decrypt_msg(char* msg, int msg_size, char* decryptBuffer, int offset)
{
  if(SERVERID == msg[0]) { msg++; msg_size--; }
  else return -2;

  if(msg_size % BACONSIZE != 0) return -1; //SI ESTO PASA HAY QUE RETORNAR UN CODIGO DE ERROR
  
  for(int i = 0; i < msg_size; i++)
    if((msg[i] = decrypt_random_char(msg[i])) == -1) return -1;

  int pos = 0;
  
  for(int i = 0; i < msg_size;)
  {
    char temp[BACONSIZE + 1];
    for(int j = 0; j < BACONSIZE; j++)
      temp[j] = msg[i++];

    temp[BACONSIZE] = '\0';

    if((decryptBuffer[pos++] = decrypt_bacon_transform(temp)) == -1) return -1;   
  }

  for(int i = 0; i < pos; i++)
    if((decryptBuffer[i] = decrypt_char(decryptBuffer[i], offset)) == -1) return -1;

  decryptBuffer[pos] = '\0';
  return pos;
}


/*
 * Procedimiento que divide un string dado el punto de division
 * @param temp El buffer donde estará almacenado el resultado
 * @param original Texto original
 * @param tam Entero que indica el punto de quiebre del texto original
 */
void split(char* temp, char* original, int tam)
{
  strncpy(temp, &(original[tam]), strlen(original) - tam);
}


/*
 * Función que parsea la solicitud proveniente del cliente
 * @param request El mensaje construido por el cliente para el servidor
 * @return el header utilizado para el cifrado/descifrado
 */
header* parse_request(char* request)
{
  header* head = (header*) malloc(sizeof(header));

  //Variables para guardar los resultados temporales del parseo
  char temp[10];
  char* auxToken = strtok(request, "\n"); 

  if(strcmp(auxToken, "CIF") == 0) head->mode = 1;
  else if (strcmp(auxToken, "DES") == 0) head->mode = 0;
  else { head->mode = -1; return head; }

  auxToken = strtok(NULL, "\n");
  
  if (strstr(auxToken, "key: ") != NULL) 
  {
    split(temp, auxToken, 5); //Descartamos la palabra "key: ".
    head->cant = strtol(temp, NULL, 10);
  } else
  {
    head->mode = -1;
    return head;
  }

  auxToken = strtok(NULL, "\n");
  if(strstr(auxToken, "address: ") != NULL)
  {
    split(temp, auxToken, 9); //Descartamos la palabra "address: ";
    if(strcmp(temp, "i") == 0) head->cant = -1 * head->cant;
  } else
  {
    head->mode = -1;
    return head;
  }
  
  auxToken = strtok(NULL, "\n"); //Parseamos la linea de la hora y el dia y la descartamos.
  if(strstr(auxToken, "time: ") == NULL)
  {
    head->mode = -1;
    return head;
  }
 
  auxToken = strtok(NULL, "\0"); //Obtenemos el mensaje a descifrar.
  head->message = auxToken;
  return head;
}


/*
 * Procedimiento que construye la respuesta del servidor
 * @param code Código de respuesta
 * @param buffer Mensaje cifrado/descifrado
 * @param outBuffer Mensaje construido por el servidor enviado al cliente
 * @return entero con el tamaño del mensaje construido
 */
int create_response(int code, char* buffer, char* outBuffer)
{
  time_t tiempo = time(0);
  struct tm *tlocal = localtime(&tiempo);
  char bufferTime[128];
  strftime(bufferTime,128,"%d/%m/%Y %H:%M:%S",tlocal);

  char bufferCode[4];
  sprintf(bufferCode, "%d", code);
  bufferCode[3] = '\0';

  strcpy(outBuffer, bufferCode);
  strcat(outBuffer, "\n");
  strcat(outBuffer, "time: ");
  strcat(outBuffer, bufferTime);
  strcat(outBuffer, "\n");
  printf("%d\n", (int) strlen(outBuffer));
  strcat(outBuffer, buffer);
  return strlen(outBuffer);
}


/*
 * Método que procesa la solicitud del cliente
 * @param request Mensaje a cifrar/descifrar
 * @param outBuffer Mesaje cifrado/descifrado
 * @return tam del mensaje
 */
int process_request(char* request, char* outBuffer)
{
  header* head = parse_request(request);
  char* auxBuffer = (char*) malloc(OUTBUFSIZE * sizeof(char));
  int code;

  if (head->mode == -1) { auxBuffer = ""; code = 200; }
  else if (head->mode == 1) 
  {
    int res = encrypt_msg(head->message, (int) strlen(head->message), auxBuffer, head->cant);
    if(res == -1) code = 300;
    else if (res == -2) code = 500;
    else code = 100;
  }
  else if (head->mode == 0) 
  {
    int res = decrypt_msg(head->message, (int) strlen(head->message), auxBuffer, head->cant);
    if(res == -1) code = 301;
    else if (res == -2) code = 400;
    else code = 100;
  }
  else { auxBuffer = ""; code = 900; }

  return create_response(code, auxBuffer, outBuffer);
}

/*
 * Método que procesa la solicitud del cliente utilizando TCP
 * @param clntSocket Socket utilizado por el cliente
 * @param id Entero que identifica al servidor
 */
void HandleTCPClient(int clntSocket, int id, char* ipcli)
{
    char outBuffer[OUTBUFSIZE];
    char inBuffer[RCVBUFSIZE];
    int recvMsgSize;

    SERVERID = id;
    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, inBuffer, OUTBUFSIZE, 0)) < 0)
    {
      write_entry_log("recv", "ERROR FATAL: Fallo la recepcion del requerimiento del cliente", ipcli);
      DieWithError("ERROR FATAL: Fallo la recepcion del requerimiento del cliente");
    } else
      write_entry_log("recv", "Se recibio una peticion del cliente", ipcli);

    inBuffer[recvMsgSize] = '\0';

    int size = process_request(inBuffer, outBuffer);

    /* Echo message back to client */
    if (send(clntSocket,outBuffer, size, 0) != size)
    {
      write_entry_log("send", "ERROR FATAL: Fallo el envio de la respuesta al cliente", ipcli);
      DieWithError("ERROR FATAL: Fallo el envio de la respuesta al cliente");
    } else
      write_entry_log("send", "Se respondio al cliente", ipcli);

    close(clntSocket);    /* Close client socket */
}
