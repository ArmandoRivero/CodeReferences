/**************************************************************************//**
 * @file        serialLogger.c
 * @brief       Serial Logger
 * @author      Armando Rivero
 * @date        c: 2019.05.21
 * @date        m: 2019.05.31
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
#include "fileManagement.h"

char serialName[]    = "/dev/ttyUSB0";
const char stringToSend[]    = "t";

/* Timestamp configuration  */
char timestampSystem[32] = {0};
char newTimestamp[32] = {0};


unsigned long GetTimeStamp() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*(unsigned long)1000000+tv.tv_usec;
}

struct timeval GetTimeStamp1() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv;
}

/******************************************************************
                          MAIN PROGRAM
******************************************************************/
int main(void)
{
  int fd = 0;
  unsigned long timeInus;
  char myStr[100];

  char storeFile[32];
  FILE *pFile;


  char readBufRAW[30];


  char buf[32] = {0};
  time_t rawtime = time(NULL);
  struct tm *ptm = localtime(&rawtime);



  // create Log folder
  struct stat st = {0};
  if(stat("./log_RD3000",&st)==-1)
    mkdir("./log_RD3000",0777);


  printf("\n**************************************************\n");
  printf(" ------ Starting RD3000 data acquisition --------\n");
  printf("**************************************************\n\n");

  createFile(storeFile);
  pFile = fopen(storeFile,"a");
  if(pFile == NULL)
  {
	printf("Problem to open file : %s", storeFile);
	exit(1);
  }
  printf("--> File '%s' succesfully created!\n", storeFile);


  


  fd = open_port(serialName);
  //if(fd == -1)
   // continue;

  /* Application program */
  write_port(fd, stringToSend, strlen(stringToSend));
 
  time2string(timestampSystem);
  sprintf(newTimestamp,"%s \n",timestampSystem);
  //write_port(fd, newTimestamp, strlen(newTimestamp));
  
  printf("%lu \n",GetTimeStamp());
 
  timeInus = GetTimeStamp();
  sprintf(myStr,"%ld \n",timeInus);
//  write_port(fd, myStr, strlen(myStr));



  read_port(fd, readBufRAW, 15);

  // get time when read was complete
  strftime(buf, 32, "%Y-%m-%d,%H:%M:%S,", ptm);


  fwrite(buf, 1, strlen(buf), pFile);
  /* Writing data in log file */
  fwrite(readBufRAW, 1, strlen(readBufRAW), pFile);
  //printf("\n--->strlen(readBufRAWR)=%i\n",strlen(readBufRAWR));
  fclose(pFile);



  close_port(fd);

  return 0;
}


