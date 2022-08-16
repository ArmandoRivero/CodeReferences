/**************************************************************************//**
 * @file        socketTest_server_2.c
 * @brief       Server side to demonstrate Socket programming
 * @author      Armando Rivero
 * @date        c: 2019.10.11
 * @date        m: 2019.10.11
 * @version     0.01
 * @bug         No known bugs
 *****************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>


#define PORT 65432  // desired port

int main(int argc, char const *argv[])
{
   int serverSocket, clientSocket, valread;

   // specify address for the socket
   struct sockaddr_in serverAddress;

   int opt = 1;
   int addrlen = sizeof(serverAddress);
   char buffer[1024] = {0};
   char *hello = "Hello from server";
   char *answer = "acquisition Finished";
   char *answer2 = "client quit";

   char *token;
   char cmd1[] = "initImgAcqu";
   char cmd2[] = "quit";

   char enableLoop = 1;


   printf("---------------------------------------------\n");
   printf("Test v1.03\n");

   // Creating the server socket
   if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
   {
      perror("socket failed");
      exit(EXIT_FAILURE);
   }

   // Forcefully attaching socket to the port defined as PORT
   if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                             &opt, sizeof(opt)))
   {
     perror("setsockopt");
     exit(EXIT_FAILURE);
   }
   serverAddress.sin_family = AF_INET;
   serverAddress.sin_addr.s_addr = INADDR_ANY; // acccepts any IP address
   serverAddress.sin_port = htons( PORT );

   // Bind the socket to the IP and PORT
   if (bind(serverSocket, (struct sockaddr *)&serverAddress,
                             sizeof(serverAddress)) < 0)
   {
     perror("bind failed");
     exit(EXIT_FAILURE);
   }

   // Listen for connections
   if (listen(serverSocket, 3) < 0)
   {
     perror("listen");
     exit(EXIT_FAILURE);
   }



   // Application program
   printf("enterning enableLoop:\n");
   while (enableLoop)
   {

      if ((clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddress,
                 (socklen_t*)&addrlen)) < 0)
      {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      valread = read( clientSocket , buffer, 1024);

      printf("-> valread: %d\n", valread);
      if (valread < 0)
         break;

      else if(valread > 0)
      {
         printf("-before token: %s\n",buffer );
         token = strtok(buffer,".");
         printf("after token: %s\n",buffer );


         if ( !(strcmp(token,cmd1)) )
         {
            send(clientSocket , answer , strlen(answer) , 0 );
            printf("<- initImgAcqu message sent to client\n");
            token = strtok(NULL,".");
            printf("2nd token: %s\n", token );
         }
         else if (!(strcmp(token,cmd2)) )
         {
            send(clientSocket , answer2 , strlen(answer2) , 0 );
            printf("<- quit message sent to client\n");

            enableLoop = 0;
            printf("Closing server program\n");
         }
      }
      else
      {
         printf("nothing received from client\n");
         // close the socket
         close(serverSocket);
      }

   }


   return 0;
}

