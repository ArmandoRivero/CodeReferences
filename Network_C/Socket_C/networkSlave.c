/**************************************************************************//**
 * @file        networkSlave.c
 * @brief       Server side for socket communication
 * @author      Armando Rivero
 * @date        c: 2021.04.13
 * @date        m: 2021.04.13
 * @version     1.00
 * @bug         Only network communication
 *****************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <signal.h>


#define PORT 65432  // desired port for socket




int main(int argc, char const *argv[])
{
  int measQuant = 0;
  int camGain = 0;
  int camShutSpeed = 0;
  
  //pid_t pidProgram;
  printf("pid of program is %d\n" , getpid());


  printf("\n**************************************************\n");
  printf(" ------ Starting RD3000 data acquisition --------\n");
  printf(" ------ Test: 1.02                       --------\n");
  printf("**************************************************\n");
  printf(" ------ TEST VERSION FOR SOCKET ONLY     --------\n");
  printf("**************************************************\n\n");


  // --------------------------------------------------------------------------
  // Socket management
  // --------------------------------------------------------------------------
  int serverSocket, clientSocket, valread;

  // specify address for the socket
  struct sockaddr_in serverAddress;

  int opt = 1;
  int addrlen = sizeof(serverAddress);
  char buffer[1024] = {0};
  const char ans_acquFinish[23] = "acquisition Finished\r\n";
  const char answerFake[35] = "123.567,901.345,789.123,567.901,\r\n";
  const char ans_quit[14] = "client quit\r\n";
  const char ans_WhoIsThere[17] = "RiverDepth3000\r\n";

  // commands receided from GUI
  char cmd_ACQ[] = "shootImgAcqu";
  char cmd_WIT[] = "whoIsThere?";
  char cmd_quit[] = "quit";
  char cmd_Ok[] = "Ok";
  char *token;

  char socketLoop = 1;

  int cnt_iteration = 0;

reInit:
  // Creating the server socket
  if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("\tsocket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port defined as PORT
  if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                           &opt, sizeof(opt)))
  {
   perror("\tsetsockopt");
   exit(EXIT_FAILURE);
  }
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY; // acccepts any IP address
  serverAddress.sin_port = htons( PORT );

  // Bind the socket to the IP and PORT
  if (bind(serverSocket, (struct sockaddr *)&serverAddress,
                           sizeof(serverAddress)) < 0)
  {
   perror("\tbind failed");
   exit(EXIT_FAILURE);
  }

  // Listen for connections
  if (listen(serverSocket, 3) < 0)
  {
   perror("\tlisten");
   exit(EXIT_FAILURE);
  }
  // --------------------------------------------------------------------------

reinitSocket:

  // --------------------------------------------------------------------------
  // Application program
  // --------------------------------------------------------------------------
  printf("Entering main socket loop:\n");
  printf("--------------------------\n");
  if ((clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddress,
             (socklen_t*)&addrlen)) < 0)
  {
    perror("\taccept");
    exit(EXIT_FAILURE);
  }


  int counterRetry = 0; // counter when client socket is lost
  while (socketLoop)
  {
    printf("pid of program is %d\n" , getpid());

    printf("*************************************************************" \
      "******************\n");
    printf("******** NEW MEASURE ****************************************" \
      "******************\n");

    // acquire message from  socket
    valread = read( clientSocket , buffer, 1024);

    printf("\n-----> cnt_iteration: %d\n", cnt_iteration);

    printf("\n-----> valread: %d\n", valread);
    if (valread < 0)
    {
      break;
    }
    else if(valread == 0)
    {
      if(counterRetry >= 500)
      {
        printf("socket communication lost, reinitialaizing comm. swith client\n");
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
        goto reinitSocket;
      }

      printf("Nothing received\n");
      counterRetry++;
    }
    else if(valread > 0)
    {
      printf("\tReceived message: %s\n",buffer );
      token = strtok(buffer,".\r\n");
      printf("\t1st token: %s\n",buffer );

      // verify if packet is valid
      if ( !(strcmp(token,cmd_ACQ)) )
      {
        // Gather all tokens
        token = strtok(NULL,".");     // 2nd token
        // printf("\t2nd token: %s\n", token );
        camGain = atoi(token);
        printf("\t2nd token - camGain: %d\n", camGain);

        token = strtok(NULL,".");     // 3rd token
        // printf("\t3rd token: %s\n", token );
        camShutSpeed = atoi(token);
        printf("\t3rd token - camShutSpeed: %d\n", camShutSpeed);




        // DO THE ACQUISITION OF THE CAMERA



        
        printf("-----> DUMMY DATA IS GENERATED <-------\n");
        send(clientSocket , answerFake , strlen(answerFake) , 0 );

        printf("-----> Indicate that data is send to client <-------\n");
        send(clientSocket , ans_acquFinish , strlen(ans_acquFinish) , 0 );
        printf("\t<- message ans_acquFinish sent to client\n");

retryOk:
        valread = read( clientSocket , buffer, 1024);
        printf("Waiting for Ok\n");
        printf("-----> valread: %d\n", valread);

        if(valread == 0)
        {
          printf("Nothing received\n");
          printf("OK was expected\n");
        }
        else if(valread > 0)
        {
          printf("\tReceived message: %s\n",buffer );
          printf("-----> ??\n");
          token = strtok(buffer,"\n");
          if ( !(strcmp(token,cmd_Ok)) )
          {
            cnt_iteration++;
          }
          else
          {
            printf("message OK not received\n");
            goto retryOk;
          }
        }

      }
      else if (!(strcmp(token,cmd_WIT)) )
      {
        printf("who is there received\n");
        send(clientSocket , ans_WhoIsThere , strlen(ans_WhoIsThere) , 0 );
        printf("\t--> %s\n", ans_WhoIsThere);
        printf("Closing client connction\n");
      }
      else if (!(strcmp(token,cmd_quit)) )
      {
        printf("Quit command received\n");
        send(clientSocket , ans_quit , strlen(ans_quit) , 0 );
        printf("\t--> %s\n", ans_quit);
        printf("Closing server program...\n");
        shutdown(clientSocket, SHUT_RDWR);
        close(serverSocket);
        goto exitProgram;
        break;
      }
      else
      {
        // packet is not valid
        printf("Message received not valid\n");
        printf("\tClosing client socket, and reinitilizing socket\n");
      }

    }
    else
    {
       printf("\tnothing VALID received from client\n");

       // close the socket
       shutdown(clientSocket, SHUT_RDWR);
       close(serverSocket);
       goto reInit;
    }

  }
exitProgram:
  printf("---------- Program finished ----------\n");

  return 0;
}
// --------------------------------------------------------------------------
