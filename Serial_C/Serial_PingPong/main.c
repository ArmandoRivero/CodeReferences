/**************************************************************************//**
 * @file        main.c
 * @brief       Serial ping pong
 * @author      Armando Rivero
 * @date        c: 2019.05.21
 * @date        m: 2022.07.07
 * @version     0.01
 * @bug         No known bugs
 *****************************************************************************/

#include <stdio.h>     // Standard input/output definitions
#include <stdlib.h>
#include <string.h>    // String function definitions
#include <unistd.h>    // UNIX standard function definitions
#include <fcntl.h>     // File control definitions
#include <errno.h>     // Error number definitions
#include <termios.h>   // POSIX terminal control definitions
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>  // System Time control definitionsi
#include <time.h>

#include "serialConfig.h"
#include "errorList.h"


const int pingPongSelector = 1; // 1 = ping, 0 = Pong

const char stringToSendPing[]    = "PING\n";
const char stringToSendPong[]    = "PONG\n";
const char stringTocomparePing[]    = "PING";
const char stringTocomparePong[]    = "PONG";


/******************************************************************
                          MAIN PROGRAM
******************************************************************/
int main(void)
{
  char serialName_NJ[]    = "/dev/ttyTHS1";
  int fd_serial_NJ = -1;
  fd_set set;
  struct timeval timeout;

  char readBufRAW[30];
  char *token;
  char tokenStr[30] = "";

  int validString = 1;

  int pinpong_cnt = 1;



  printf("\n**************************************************\n");
  printf(" ------    Starting Serial ping-pong      --------\n");
  if(pingPongSelector == 1)
  {
    printf(" ---- Selected: PING (first in Tx) --------\n");
  }
  else
  {
    printf(" ---- Selected: PONG      --------\n");
  }
  printf("**************************************************\n");
  printf(" NOTE: Run first PONG program\n");
  printf("**************************************************\n\n");

  printf("Opening Serial port\n");
  open_port(serialName_NJ,&fd_serial_NJ,&set,&timeout);



  while(validString)
  {
    // PING ROUTINE
    if (pingPongSelector == 1)
    {
      // Send start string
      printf("Ping: -> Tx: [%s]\n",stringToSendPing);
      write_port(fd_serial_NJ, stringToSendPing);
      
      sleep(1);
      readUntilEnd(readBufRAW,&fd_serial_NJ,&set,&timeout);
      printf("Ping: <- Rx = [%s]\n", readBufRAW);

      token = strtok(readBufRAW, "\r\n");
      strcpy(tokenStr, token);
      memset(readBufRAW, 0, 30);

      //printf("dbg: tokenStr = [%s]\n", tokenStr);
      if (!strcmp(tokenStr,stringTocomparePong))
      {
        printf("Valid Rx : %d\n", pinpong_cnt);
        pinpong_cnt++;
      }
      else
      {
        printf("Ping: Rx wrong string received : [%s]\n", readBufRAW);
        validString = 0;
      }
    }
    else
    // PONG ROUTINE
    {
      sleep(1);
      // acquire data from serial
      readUntilEnd(readBufRAW,&fd_serial_NJ,&set,&timeout);
      printf("Pong: <- Rx = [%s]\n", readBufRAW);

      token = strtok(readBufRAW, "\r\n");
      strcpy(tokenStr, token);
      memset(readBufRAW, 0, 30);
      //printf("dbg: tokenStr = [%s]\n", tokenStr);

      if (!strcmp(tokenStr,stringTocomparePing))
      {
        printf("Valid Rx : %d\n", pinpong_cnt);
        printf("Pong: -> Tx (%d): [%s]\n", pinpong_cnt, stringToSendPong);
        write_port(fd_serial_NJ, stringToSendPong);
        pinpong_cnt++;
      }
      else
      {
        printf("Pong: Rx wrong string received : [%s]\n", readBufRAW);
        validString = 0;
      }
    }
  }

  printf("Closing Serial port\n");
  close_port(fd_serial_NJ);

  return 0;
}


