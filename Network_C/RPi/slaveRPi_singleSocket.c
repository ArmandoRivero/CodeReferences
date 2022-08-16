
/**************************************************************************//**
 * @file        slaveRPi.c
 * @brief       Server side for socket communication
 * @author      Armando Rivero
 * @date        c: 2020.03.06
 * @date        m: 2020.03.06
 * @version     1.28
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
  int camShutSpeed = 0;
  int camGain = 0;
  int ToFmode = 0;
  int ToFqtty = 0;
  int ToFpwmqtty = 0;
  int USqtty = 0;

unsigned long timeStamp = 0;


  //pid_t pidProgram;
  printf("pid of program is %d\n" , getpid());


  printf("\n**************************************************\n");
  printf(" ------ Starting RD3000 data acquisition --------\n");
  printf(" ------ Test: 1.28d                      --------\n");
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
  const char answer1[23] = "acquisition Finished\r\n";
  const char answerFake[35] = "123.567,901.345,789.123,567.901,\r\n";
  const char answer2[14] = "client quit\r\n";
  const char answer3[15] = "ToFUS Finished";
  const char answerWhoIsThere[17] = "RiverDepth3000\r\n";

  // commands receided from GUI
  char cmd1[] = "shootImgAcqu";
  char cmd2[] = "MBsens";
  char cmd3[] = "LLsens";
  char cmd4[] = "whoIsThere?";
  char cmd5[] = "quit";
  char cmd6[] = "ToFUS";
  char cmd7[] = "ToFUSpwm";
  char cmdOk[] = "Ok";
  char sensorStr[80];
  char *token;
  char *cmd1Path_location;
  char *cmd6Path_location;
  char *cmd7Path_location;
  char *cmd1Path_session;
  char *cmd6Path_session;
  char *cmd7Path_session;

  char socketLoop = 1;

  int cnt_iteration = 0;

  int successClosing = 0;

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
      if ( !(strcmp(token,cmd1)) )
      {
        // Gather all tokens
        token = strtok(NULL,".");     // 2nd token
        // printf("\t2nd token: %s\n", token );
        measQuant = atoi(token);
        printf("\t2nd token - measQuant: %d\n", measQuant);

        cmd1Path_location = strtok(NULL,".");  // 3rd token
        printf("\t3nd token - cmd1Path_location: %s\n", cmd1Path_location );

        token = strtok(NULL,".");     // 4th token
        // printf("\t4th token: %s\n", token );
        camShutSpeed = atoi(token);
        printf("\t4th token - camShutSpeed: %d\n", camShutSpeed);

        token = strtok(NULL,".");     // 5th token
        // printf("\t5th token: %s\n", token );
        camGain = atoi(token);
        printf("\t5th token - camGain: %d\n", camGain);

        token = strtok(NULL,".");     // 6th token
        // printf("\t26th token: %s\n", token );
        ToFmode = atoi(token);
        printf("\t6th token - ToFmode: %d\n", ToFmode);

        token = strtok(NULL,".");     // 7th token
        // printf("\t7th token: %s\n", token );
        ToFqtty = atoi(token);
        printf("\t7th token - ToFqtty: %d\n", ToFqtty);

        token = strtok(NULL,".");     // 8th token
        // printf("\t8th token: %s\n", token );
        USqtty = atoi(token);
        printf("\t8th token - USqtty: %d\n", USqtty);


        // Create and append data to the local log
        token = strtok(cmd1Path_location,"/");
        printf("\t-- 3rd token, 1st subtoken: %s\n", token );
        cmd1Path_session = strtok(NULL," ");     // 2nd token
        printf("\t-- 3rd token, 2nd subtoken: %s\n", cmd1Path_session );


        printf("-----> DUMMY DATA IS GENERATED <-------\n");
        send(clientSocket , answerFake , strlen(answerFake) , 0 );
        printf("\t<- message answerFake sent to client\n");

        send(clientSocket , answer1 , strlen(answer1) , 0 );
        printf("\t<- message answer1 sent to client\n");

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
          if ( !(strcmp(token,cmdOk)) )
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
      else if (!(strcmp(token,cmd4)) )
      {
        printf("who is there received\n");
        send(clientSocket , answerWhoIsThere , strlen(answerWhoIsThere) , 0 );
        printf("\t--> %s\n", answerWhoIsThere);
        printf("Closing client connction\n");
      }
      else if (!(strcmp(token,cmd5)) )
      {
        printf("Quit command received\n");
        send(clientSocket , answer2 , strlen(answer2) , 0 );
        printf("\t--> %s\n", answer2);
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
