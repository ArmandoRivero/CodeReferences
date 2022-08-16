
/**************************************************************************//**
 * @file        slaveRPi.c
 * @brief       Server side for socket communication
 * @author      Armando Rivero
 * @date        c: 2019.10.11
 * @date        m: 2020.03.05
 * @version     1.26
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
  printf(" ------ Test: 1.26                      --------\n");
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
  const char answer2[12] = "client quit";
  const char answer3[15] = "ToFUS Finished";
  const char answerWhoIsThere[15] = "RiverDepth3000";

  // commands receided from GUI
  char cmd1[] = "shootImgAcqu";
  char cmd2[] = "MBsens";
  char cmd3[] = "LLsens";
  char cmd4[] = "whoIsThere?";
  char cmd5[] = "quit";
  char cmd6[] = "ToFUS";
  char cmd7[] = "ToFUSpwm";
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

  int pid;

  // --------------------------------------------------------------------------
  // Application program
  // --------------------------------------------------------------------------
  printf("Entering main loop:\n");
  printf("---------------------\n");
  while (socketLoop)
  {
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddress,
               (socklen_t*)&addrlen)) < 0)
    {
      perror("\taccept");
      exit(EXIT_FAILURE);
    }

    fork();
    pid = getpid();
    printf("pid of program is %d\n" , pid);

    valread = read( clientSocket , buffer, 1024);

    printf("*************************************************************" \
      "******************\n");
    printf("******** NEW MEASURE ****************************************" \
      "******************\n");

    printf("\n-----> cnt_iteration: %d\n", cnt_iteration);

    printf("\n-----> valread: %d\n", valread);
    if (valread < 0)
    {
       break;
    }
    else if(valread == 0)
    {
      printf("Closing client connection, PID is %d\n", pid);
      close(clientSocket);
    }
    else if(valread > 0)
    {
      printf("\tReceived message: %s\n",buffer );
      token = strtok(buffer,".");
      printf("\t1st token: %s\n",buffer );


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

        send(clientSocket , answer1 , strlen(answer1) , 0 );
        printf("\t<- message sent to client\n");

        cnt_iteration++;

        close(clientSocket);
        printf("KILLING PID: %d\n", pid);
        kill(pid,SIGKILL);

      }
      else
      {
        printf("\tClosing socket, and reinitializing socket\n");
closeAgain:
        successClosing = close(clientSocket);
        if (successClosing == 0 )
        {
          printf("Socket successfully closed\n");
          //shutdown(serverSocket,3);
          //close(clientSocket);
          //shutdown(clientSocket,3);
          goto reInit;
        }
        else
          printf("Socket not closed, properly, repeating closing\n");
          goto closeAgain;
      }

    }
    // else
    // {
    //    printf("\tnothing received from client\n");

    //    // close the socket
    //    close(serverSocket);
    //    goto reInit;
    // }

  }

  return 0;
}
// --------------------------------------------------------------------------
