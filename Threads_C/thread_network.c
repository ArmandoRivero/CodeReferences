/**************************************************************************//**
 * @file        thread_network.c
 * @brief       Server side for socket communication using threads
 * @author      Armando Rivero
 * @date        c: 2020.03.10
 * @date        m: 2020.03.11
 * @version     1.11
 * @bug         -
 *****************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <signal.h>

#include <pthread.h>


#define PORT 65432  // desired port for socket
#define MAX_LISTEN_CLIENTS 3 // maximum mumber of client connections


// create and listen to a socket
int createListenSocket(void)
{
  int opt = 1;
  int serverSocket_fd;
  struct sockaddr_in serverAddress;

  // Creating the server socket
  if ((serverSocket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("\tsocket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port defined as PORT
  if (setsockopt(serverSocket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                           &opt, sizeof(opt)))
  {
   perror("\tsetsockopt");
   exit(EXIT_FAILURE);
  }
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY; // acccepts any IP address
  serverAddress.sin_port = htons( PORT );

  // Bind the socket to the IP and PORT
  if (bind(serverSocket_fd, (struct sockaddr *)&serverAddress,
                           sizeof(serverAddress)) < 0)
  {
   perror("\tbind failed");
   exit(EXIT_FAILURE);
  }

  // Listen for connections
  if (listen(serverSocket_fd, MAX_LISTEN_CLIENTS) < 0)
  {
   perror("\tlisten");
   exit(EXIT_FAILURE);
  }

  return serverSocket_fd;

}


// thread to accept socket and execute program
void *netAcceptThread(void * threadArg)
{
  int cnt_connections = 0;
  int socketLoop = 1;
  int serverSocket_fd = (int) threadArg;
  int addrlen;
  struct sockaddr_in serverAddress;

  int clientSocket_fd;

  int valread;
  char socket_buffer[1024] = {0};
  char *token;


  // application comands received
  char cmd1[] = "shootImgAcqu";
  char cmd4[] = "whoIsThere?";
  const char answerWhoIsThere[17] = "RiverDepth3000\r\n";
  char cmd5[] = "quit";
  const char answerQuit[14] = "client quit\r\n";
  int measQuant = 0;
  int camShutSpeed = 0;
  int camGain = 0;
  int ToFmode = 0;
  int ToFqtty = 0;
  int ToFpwmqtty = 0;
  int USqtty = 0;
  char *cmd1Path_location;

  const char answerFake[35] = "123.567,901.345,789.123,567.901,\r\n";
  const char answerAcquFinished[23] = "acquisition Finished\r\n";
  char cmdOk[] = "Ok";



  printf(" -> entering socket loop\n");
  while (socketLoop)
  {
    printf("\n-------------------------------------------------------------\n");
    printf("  --> accept new client? (%d)\n", cnt_connections);
    if ((clientSocket_fd = accept(serverSocket_fd, (struct sockaddr *)&serverAddress,
                 (socklen_t*)&addrlen)) < 0)
    {
      perror("   ---> accept failed");
      exit(EXIT_FAILURE);
    }
    else
    {
      printf("   ---> accept and connected to : %d\n",clientSocket_fd);
      cnt_connections ++;
    }


    // acquire message from  socket
    valread = read( clientSocket_fd , socket_buffer, 1024);
    printf("  --> valread: %d\n", valread);
    if (valread < 0)
    {
      perror("accept"); // error reading fd
      break;
    }
    else if(valread == 0)
    {
      printf("Nothing received\n");
      break;
    }
    else if(valread > 0)
    {
      printf("\tReceived message: %s\n",socket_buffer );
      token = strtok(socket_buffer,".\r\n");
      printf("\t1st token: %s\n",socket_buffer );

      // verify if packet is valid
      if ( !(strcmp(token,cmd1)) )
      {
        printf("\tPacket is valid\n");

        // Gather all tokens
        token = strtok(NULL,".");     // 2nd token
        // printf("\t2nd token: %s\n", token );
        measQuant = atoi(token);
        // printf("\t2nd token - measQuant: %d\n", measQuant);

        cmd1Path_location = strtok(NULL,".");  // 3rd token
        // printf("\t3nd token - cmd1Path_location: %s\n", cmd1Path_location );

        token = strtok(NULL,".");     // 4th token
        // printf("\t4th token: %s\n", token );
        camShutSpeed = atoi(token);
        // printf("\t4th token - camShutSpeed: %d\n", camShutSpeed);

        token = strtok(NULL,".");     // 5th token
        // printf("\t5th token: %s\n", token );
        camGain = atoi(token);
        // printf("\t5th token - camGain: %d\n", camGain);

        token = strtok(NULL,".");     // 6th token
        // printf("\t26th token: %s\n", token );
        ToFmode = atoi(token);
        // printf("\t6th token - ToFmode: %d\n", ToFmode);

        token = strtok(NULL,".");     // 7th token
        // printf("\t7th token: %s\n", token );
        ToFqtty = atoi(token);
        // printf("\t7th token - ToFqtty: %d\n", ToFqtty);

        token = strtok(NULL,".");     // 8th token
        // printf("\t8th token: %s\n", token );
        USqtty = atoi(token);
        // printf("\t8th token - USqtty: %d\n", USqtty);


//         // Create and append data to the local log
//         token = strtok(cmd1Path_location,"/");
//         printf("\t-- 3rd token, 1st subtoken: %s\n", token );
//         cmd1Path_session = strtok(NULL," ");     // 2nd token
//         printf("\t-- 3rd token, 2nd subtoken: %s\n", cmd1Path_session );


        printf("-----> DUMMY DATA IS GENERATED <-------\n");
        send(clientSocket_fd , answerFake , strlen(answerFake) , 0 );
        printf("\t<- message answerFake sent to client\n");

        send(clientSocket_fd , answerAcquFinished , strlen(answerAcquFinished) , 0 );
        printf("\t<- message answerAcquFinished sent to client\n");

retryOk:
        valread = read( clientSocket_fd , socket_buffer, 1024);
        printf("Waiting for Ok\n");
        printf("-----> valread: %d\n", valread);
        if(valread == 0)
        {
          printf("Nothing received\n");
          break;
        }
        else if(valread > 0)
        {
          printf("\tReceived message: [%s]\n",socket_buffer );
          token = strtok(socket_buffer,"\n");
          printf("\tok token?: %s\n", token );
          if ( !(strcmp(token,cmdOk)) )
          {
            printf("\tok token received\n");
            //cnt_iteration++;
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
        printf("\twho is there received\n");
        send(clientSocket_fd , answerWhoIsThere , strlen(answerWhoIsThere) , 0 );
        printf("\t--> %s\n", answerWhoIsThere);
      }
      else if (!(strcmp(token,cmd5)) )
      {
        printf("\tQuit command received\n");
        send(clientSocket_fd , answerQuit , strlen(answerQuit) , 0 );
        printf("\t--> %s\n", answerQuit);
        printf("Closing server program...\n");
        shutdown(clientSocket_fd, SHUT_RDWR);
        close(serverSocket_fd);
        goto exitNetThread;
      }
      else
      {
        printf("\tPacket NOT valid\n");
      }
    }

    printf("  --> closing socket\n");
closeAgain:
    shutdown(clientSocket_fd, SHUT_RDWR);
    if (close(clientSocket_fd) == 0)
    {
      printf(" -> Socket successfully closed\n");
    }
    else
    {
      printf(" -> Socket not closed, properly, retrying to close\n");
      goto closeAgain;
    }
  }

exitNetThread:
  printf(" -> exiting thread\n");
  pthread_exit(NULL);
}



int main(int argc, char const *argv[])
{
  pthread_t t;

  // --------------------------------------------------------------------------
  // Socket management
  // --------------------------------------------------------------------------
  int serverSocket_fd;
  int rc;


  printf("pid of program is %d\n" , getpid());
  printf("\n**************************************************\n");
  printf(" ------ Threads and network               --------\n");
  printf(" ------ Test: 1.11                        --------\n");
  printf("**************************************************\n");



  // Create and listen to socket
  serverSocket_fd = createListenSocket();

  printf("Creating network thread\n");
  rc = pthread_create(&t,NULL,netAcceptThread,(void *)serverSocket_fd);

  if (!rc)
  {
    printf(" -> Thread created\n");
  }
  else if (rc){
    printf("ERROR: unable to create thread, %d", rc);
  }

  //sleep(3);
  printf("main() program paused\n");
  pause();


  return 0;
}
// --------------------------------------------------------------------------
