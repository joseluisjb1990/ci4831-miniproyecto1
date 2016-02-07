#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <ctype.h>
#include <string.h>
#include <ctype.h>

#define RCVBUFSIZE 32   /* Size of receive buffer */
#define OUTBUFSIZE 160
#define LETTERSIZE 26
#define BACONSIZE 6
char* letters = "abcdefghijklmnopqrstuvwxyz";

void DieWithError(char *errorMessage);  /* Error handling function */

char encryipt_char(char c, int shift)
{
  for(int i = 0; i < LETTERSIZE; i++)
  {
    if(letters[i] == c) 
    { 
      int pos = (i + shift) % LETTERSIZE; if (pos < 0) pos = LETTERSIZE + pos;
      return letters[pos];
    }
  }

  if(' '  == c) return ' ';
  if('\n' == c) return '\n';

  return -1;
}


char decrypt_char(char c, int shift)
{
  for(int i = 0; i < LETTERSIZE; i++)
  {
    if(letters[i] == c) 
    { 
      int pos = (i - shift) % LETTERSIZE; if (pos < 0) pos = LETTERSIZE + pos;
      return letters[pos];
    }
  }

  if(isspace(c)) return c;

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
    default: return "BBBBBB";
  }
}

char random_char(char c)
{
  int cs = 97 + rand() % LETTERSIZE;

  if(c == 'A') return toupper(cs);
  else if (c == 'B') return cs;
  else return -1;
}

int encrypt_msg(char* msg, int msg_size, char* outBuffer)
{
  for(int i = 0; i < msg_size; i++)
    msg[i] = tolower(msg[i]);

  for(int i = 0; i < msg_size; i++)
    msg[i] = encryipt_char(msg[i], -5);


  int pos = 0;
  for(int i = 0; i < msg_size; i++)
  {
    char* tchar = bacon_transform(msg[i]);
    for(int j = 0; j < BACONSIZE; j++)
      outBuffer[pos++] = tchar[j];  
  }

  for (int i = 0; i < pos; i++)
  {
    outBuffer[i] = random_char(outBuffer[i]);
  }
  
  return pos;
}

char decrypt_random_char(char c)
{
  if(isupper(c)) return 'A';
  else if (!isupper(c)) return 'B';

  return -1;
}

int decrypt_msg(char* msg, int msg_size, char* decryptBuffer)
{
  if(msg_size % BACONSIZE != 0) return -1; //SI ESTO PASA HAY QUE RETORNAR UN CODIGO DE ERROR
  
  for(int i = 0; i < msg_size; i++)
  {
    msg[i] = decrypt_random_char(msg[i]); 
  }

  int pos = 0;
  
  for(int i = 0; i < msg_size;)
  {
    char temp[BACONSIZE + 1];
    for(int j = 0; j < BACONSIZE; j++)
      temp[j] = msg[i++];

    temp[BACONSIZE] = '\0';

    decryptBuffer[pos++] = decrypt_bacon_transform(temp);   
  }

  for(int i = 0; i < pos; i++)
    decryptBuffer[i] = decrypt_char(decryptBuffer[i], -5);

  return pos;
}

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    char decryptBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    char outBuffer[OUTBUFSIZE];
    int recvMsgSize;                    /* Size of received message */

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

    echoBuffer[recvMsgSize] = '\0';
    
    //int out_size = encrypt_msg(echoBuffer, recvMsgSize, outBuffer);

    //outBuffer[out_size] = '\0';

    //int final_pos = decrypt_msg(outBuffer, out_size, decryptBuffer);

    //decryptBuffer[final_pos] = '\0';
    printf("%s\n", echoBuffer);
    
    /* Echo message back to client */
    if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
        DieWithError("send() failed");

    close(clntSocket);    /* Close client socket */
}
