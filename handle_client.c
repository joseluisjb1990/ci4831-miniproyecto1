#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */

#define RCVBUFSIZE 32   /* Size of receive buffer */
#define LETTERSIZE 26

char* letters = "abcdefghijklmnopqrstuvwxyz";

void DieWithError(char *errorMessage);  /* Error handling function */

char encryipt_char(char c, int shift)
{
  for(int i = 0; i < LETTERSIZE; i++)
  {
    if(letters[i] == c) 
    { 
      int pos = (i + shift) % LETTERSIZE; if (pos < 0) pos = LETTERSIZE + pos;
      printf("%d\n", pos);
      return letters[pos];
    }
  }

  if(' ' == c) return ' ';

  return -1;
}

void encrypt_msg(char* msg, int msg_size)
{
  for(int i = 0; i < msg_size; i++)
  {
    msg[i] = encryipt_char(msg[i], -5);
  }
}

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int recvMsgSize;                    /* Size of received message */

    /* Receive message from client */
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

    echoBuffer[recvMsgSize] = '\0';
    
    printf("%d\n", echoBuffer[0]);

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {

        /* Echo message back to client */
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");

        /* See if there is more data to receive */
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");
    }

    close(clntSocket);    /* Close client socket */
}
