/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "scdax.h"
#include <time.h>
#include <string.h>     /* for memset() */
#include <stdlib.h>

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

typedef struct {
  int mode;
  int cant;
  char* message;
} header;

char* letters = "abcdefghijklmnopqrstuvwxyz";

void DieWithError(char *errorMessage);  /* Error handling function */

void create_time(char *output)
{
	time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    strftime(output,128,"%a %b %H:%M:%S %Y",tlocal);
}

char* description_response(int code)
{
	char *description = malloc(128 * sizeof(char));
	switch(code) {
	    case 100:
	      strcpy(description, "El mensaje se pudo cifrar o descifrar correctamente.");
	      break;
	    case 200:
	      strcpy(description, "El mensaje enviado por el cliente no sigue las reglas definidas por el protocolo");
	      break;
	    case 300:
	      strcpy(description, "El mensaje no pudo ser cifrado correctamente");
	      break;
	    case 301:
	      strcpy(description, "El mensaje no pudo ser descifrado correctamente.");
	      break;
	    case 302:
	      strcpy(description, "Se intento descifrar un mensaje que no esta cifrado.");
	      break;
	    case 400:
	      strcpy(description, "El mensaje no fue cifrado por este servidor.");
	      break;
	    case 500:
	      strcpy(description, "El mensaje es muy grande para ser cifrado.");
	      break;
	    case 900:
	      strcpy(description, "Ocurrió un error desconocido");
	      break;
	    default:
	      strcpy(description, "Ocurrió un error desconocido");
	  }
	
}


void write_file_binnacle(char* buffer, char* file)
{
  FILE *fp = fopen(file, "a");
  if(fp == NULL) DieWithError("OCURRIO UN ERROR ABRIENDO EL ARCHIVO");
  fputs(buffer, fp);
  fclose(fp);
}


char encryipt_char(char c, int shift)
{
  if(isspace(c)) return c;
  int i;
  for(i = 0; i < LETTERSIZE; i++)
  {
    if(letters[i] == c) 
    { 
      int pos = (i + shift) % LETTERSIZE; if (pos < 0) pos = LETTERSIZE + pos;
      return letters[pos];
    }
  }
  return -1;
}


char decrypt_char(char c, int shift)
{
  if(isspace(c)) return c;
  int i;
  for(i = 0; i < LETTERSIZE; i++)
  {
    if(letters[i] == c) 
    { 
      int pos = (i - shift) % LETTERSIZE; if (pos < 0) pos = LETTERSIZE + pos;
      return letters[pos];
    }
  }
  return -1;
}


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

char random_char(char c)
{
  int cs = 97 + rand() % LETTERSIZE;

  if(c == 'A') return toupper(cs);
  else if (c == 'B') return cs;
  else return -1;
}


char decrypt_random_char(char c)
{
  if(isupper(c)) return 'A';
  else if (!isupper(c)) return 'B';

  return -1;
}

void split(char* temp, char* original, int tam)
{
  strncpy(temp, &(original[tam]), strlen(original) - tam);
}

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
 
  auxToken = strtok(NULL, "\n"); //Obtenemos el mensaje a descifrar.
  head->message = auxToken;
  return head;
}

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
  strcat(outBuffer, buffer);
  return strlen(outBuffer);
}


int process_request(char* request, char* outBuffer)
{
  header* head = parse_request(request);
  char* auxBuffer = (char*) malloc(OUTBUFSIZE * sizeof(char));
  int code;

  if (head->mode == -1) { auxBuffer = ""; code = 200; }
  else if (head->mode == 1) 
    if ((encrypt_msg(head->message, (int) strlen(head->message), auxBuffer, head->cant)) == -1)
      code = 300;
    else
      code = 100;
  else if (head->mode == 0) 
    if ((decrypt_msg(head->message, (int) strlen(head->message), auxBuffer, head->cant)) == -1)
      code = 300;
    else
      code = 100;
  else { auxBuffer = ""; code = 900; }

  return create_response(code, auxBuffer, outBuffer);
}


int encrypt_msg(char* msg, int msg_size, char* outBuffer, int offset)
{
  int i;
  for(i = 0; i < msg_size; i++)
    msg[i] = tolower(msg[i]);

  for(i = 0; i < msg_size; i++)
    if ((msg[i] = encryipt_char(msg[i], offset)) == -1)
      return -1;
    

  int pos = 0;
  outBuffer[pos++] = SERVERID;

  for(i = 0; i < msg_size; i++)
  {
    char* tchar; 
    if((tchar = bacon_transform(msg[i])) == NULL) return -1;
    int j;
    for(j = 0; j < BACONSIZE; j++)
      outBuffer[pos++] = tchar[j];  
  }


  for (i = 1; i < pos; i++)
    if((outBuffer[i] = random_char(outBuffer[i])) == -1)
      return -1;
  
  outBuffer[pos] = '\0';
  return pos;
}

int decrypt_msg(char* msg, int msg_size, char* decryptBuffer, int offset)
{
  if(SERVERID == msg[0]) msg++;
  else return -2;

  if(msg_size % BACONSIZE != 0) return -1; //SI ESTO PASA HAY QUE RETORNAR UN CODIGO DE ERROR
  
  int i;
  for(i = 0; i < msg_size; i++)
    if((msg[i] = decrypt_random_char(msg[i])) == -1) return -1;

  int pos = 0;
  

  for(i = 0; i < msg_size;)
  {
    char temp[BACONSIZE + 1];
    int j;
    for(j = 0; j < BACONSIZE; j++)
      temp[j] = msg[i++];

    temp[BACONSIZE] = '\0';

    decryptBuffer[pos++] = decrypt_bacon_transform(temp);   
  }


  for(i = 0; i < pos; i++)
    decryptBuffer[i] = decrypt_char(decryptBuffer[i], offset);

  decryptBuffer[pos] = '\0';
  return pos;
}


/************************************************************************************/
int *
encrypt_msg_1_svc(message *argp, struct svc_req *rqstp)
{
	static int  result;
	char outBuffer[OUTBUFSIZE];     /* Buffer for echo string */
    char inBuffer[OUTBUFSIZE];
    int recvMsgSize;
    char *getTime = malloc(128 * sizeof(char));
    char *bufferBinnacle = malloc(128 * sizeof(char));
    char errorCode[3];
    char *description;

	/*
	 * insert server code here
	 */
	result = 1;
	//printf("%s\n", argp->out_msg);
	//printf("%d\n", SERVERID);


	int size  = process_request(argp->out_msg, outBuffer);
	printf("Buffer de Salida:\n");
	printf("%s\n", outBuffer);
	int i;
	int tam = strlen(outBuffer);
	for (i = 0; i < 3; ++i)
	{
		errorCode[i] = outBuffer[i];
		printf("%c\n", errorCode[i]);
	}
	errorCode[i] = '\0';
	
	int codError = atoi(errorCode);
	printf("Error code: %d\n", codError);
	create_time(getTime);
	printf("TIME: %s\n", getTime);
	strcpy(bufferBinnacle, "[");
	strcat(bufferBinnacle, getTime);
	strcat(bufferBinnacle, "]");
	strcat(bufferBinnacle, "[");
	strcat(bufferBinnacle, errorCode);
	strcat(bufferBinnacle, "]");
	strcat(bufferBinnacle, "[cliente ");

	strcat(bufferBinnacle, "]");
	strcat(bufferBinnacle, "[");
	strcat(bufferBinnacle, description_response(codError));
	strcat(bufferBinnacle, "]");
	strcat(bufferBinnacle, "\n");
	printf("BITACORA:%s\n", bufferBinnacle);
	write_file_binnacle(bufferBinnacle,archivoBitacoraSVC);


	return &result;
}

int *
decrypt_msg_1_svc(message *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}
