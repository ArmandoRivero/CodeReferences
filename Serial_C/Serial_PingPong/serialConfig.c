/**************************************************************************//**
 * @file        serialConfig.c
 * @brief       Serial Logger configuration
 * @author      Armando Rivero
 * @date        c: 2019.05.21
 * @date        m: 2021.01.22
 * @version     0.02
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
#include <sys/select.h>

#include "serialConfig.h"
#include "errorList.h"

struct termios serialOptions;


/*******************************************************************************
******************************************************************************/
int open_port(char *serialName,int *fd,fd_set *set,struct timeval *timeout)
{

   int result = -3;

  *fd  = open(serialName, O_RDWR | O_NDELAY );// | );
  if (*fd  == -1)
  {
      result = ERROR_OPEN_SERIAL_PORT;
  }
  else
  {   result = SUCCESS_OPEN_SERIAL_PORT;
      fcntl(*fd , F_SETFL, 0);
  }


  /* Configuring serial port  */
  tcgetattr(*fd , &serialOptions);

  cfsetispeed(&serialOptions, BAUDRATE);
  cfsetospeed(&serialOptions, BAUDRATE);

  //serialOptions.c_cflag |= (BAUDRATE | CLOCAL | CREAD);

  /* setup for non-canonical mode */
  serialOptions.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  serialOptions.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  serialOptions.c_oflag &= ~OPOST;

  tcsetattr(*fd , TCSANOW, &serialOptions);

   //-Implement timeout
   FD_ZERO(set); /* clear the set */
   FD_SET(*fd, set); /* add our file descriptor to the set */
   timeout->tv_sec = 1;
   timeout->tv_usec = 0;
 


  return result;
}


/*******************************************************************************
******************************************************************************/
int configure_serial(char *baudrate)
{
 // TBD
  return 0;
}


/*******************************************************************************
******************************************************************************/
int write_port(int fd, const char *buf)
{
  return write(fd, buf, strlen(buf));
}



/*******************************************************************************
******************************************************************************/
int read_port(int fd, char *buf, int size)
{
   int count = size;
   int tryRead = 0;
   int ret;
   //~printf("<-> count_init %d\n",count);
   while(count)
   {
      ret = read(fd, buf, count);
      printf("<-> read ret:%d\n",ret);
      if(ret<0)
      {
         if(errno != EWOULDBLOCK && errno !=  EAGAIN)
         {
            printf("<-> ERROR GRAVE\n");
            return -1; // fatal error
         }
         else if(tryRead > MAX_TRY)
         {
            printf("<-> TIMEOUT\n");
            return -1; // timeout
         }
         else
         {
            printf("<-> ok\n");
            tryRead++;
         }
      }
      else
      {
         count -= ret;
         buf += ret;
         // printf("<-> count %d\n",count);
         tryRead = 0;
         *buf = 0;

         if (size > 2 && size - count > 2 ) {
            //~ printf("<-> last buf 3 val %d %d %d\n",
            //~ *(buf-3), *(buf-2), *(buf-1));
            return count;
         }
      }
   }
   //~ printf("<-> readport end\n");
   return count;
}


int read_portFullLength(int fd, char *buf, int size)
{
   int count = size;
   int tryRead = 0;
   int ret;

   //~printf("<-> count_init %d\n",count);
   while(count)
   {
      ret = read(fd, buf, count);
      printf("<-> read ret:%d\n",ret);
      if(ret<0)
      {
         if(errno != EWOULDBLOCK && errno !=  EAGAIN)
         {
            printf("<-> ERROR GRAVE\n");
            return -1; // fatal error
         }
         else if(tryRead > MAX_TRY)
         {
            printf("<-> TIMEOUT\n");
            return -1; // timeout
         }
         else
         {
            printf("<-> ok\n");
            tryRead++;
         }
      }
      else
      {
         count -= ret;
         buf += ret;
         // printf("<-> count %d\n",count);
         
         tryRead = 0;
         *buf = 0;

         if(count == 0)
         {
          return count;
         }
         /*if (size > 2 && size - count > 2 ) {
            //~ printf("<-> last buf 3 val %d %d %d\n",
            //~ *(buf-3), *(buf-2), *(buf-1));
            return count;
         }
         */
      }
   }
   //~ printf("<-> readport end\n");
   return count;
}


/*******************************************************************************
******************************************************************************/
int close_port(int fd)
{
   int result = -1;
   if(close(fd) != 0)
   {
      result =  ERROR_SERIAL_CLOSE;
   }
   else
   {
      result = SUCCESS_SERIAL_CLOSE;
   }

   return result;
}


/*******************************************************************************
******************************************************************************/
int time2string(char *str)
{
  struct timeval tv;

  gettimeofday(&tv,NULL);
  //printf(" tv.sec: %x  tv.usec: %X\n", tv.tv_sec, tv.tv_usec);
  //printf(" tv.sec: %x  tv.usec: %X\n", tv.tv_sec - TIMEFIX, tv.tv_usec);
  sprintf(str,"%d",(unsigned int) tv.tv_sec- TIMEFIX);
  //sprintf(str,"%08X",(unsigned int) tv.tv_sec- TIMEFIX);

  printf("   Linux time[s]: %lu - %lx\n", tv.tv_sec, tv.tv_sec);
  printf("   Linux time[s]: %lu - %lu\n", tv.tv_usec, tv.tv_usec);
  printf("   Offset[s]:     %u - %x\n", TIMEFIX, TIMEFIX);
  printf("    -> time2string: %X | %s\n", (unsigned int) tv.tv_sec- TIMEFIX, str);

  return 0;
}


/*----------------------------readUntillEnd-------------------------------
 ¦ Description: Read a serial string untill reach the "\n"      ¦
 ¦    char. It try for 100 time after it will       ¦
 ¦    send an error.                 ¦
 ¦ Input:   - int * fd_serial_uC:    file descriptor of the     ¦ 
 ¦                   opened serial comunication  ¦
 ¦    - char *readBuff_serial: Buffer in which save the read   
 ¦ Output:  - int return: Error code
 -----------------------------------------------------------------------*/
 
int readUntilEnd(char *readBuff_serial, int * fd_serial_uC,fd_set *set,struct timeval *timeout)
{  //-Init variables
   int ret = 0;
   int cnt = 0;
   int retry = 0;
   char buf = 'C';
   int result = 0;;
   int resTimeOut = 0;
   //-Set the buefer to 0
   memset(readBuff_serial, 0, 1000);

   //-Start while loop (exit on fail or on success if reach the '\n' char)

   while(result == 0)
   {  //-read one character per time
      //-Implement timeout
      FD_ZERO(set); /* clear the set */
      FD_SET(*fd_serial_uC, set); /* add our file descriptor to the set */
      timeout->tv_sec = 0;
      timeout->tv_usec = 10000;
      resTimeOut = select(*fd_serial_uC + 1, set, NULL, NULL, timeout);
      if(resTimeOut  > 0)
      {
         ret = read(*fd_serial_uC, &buf, 1);
      }
      else
      {  
         //-TimeOut
         ret = -1;
      }
      //-If correclty read it -> save in the buffer      
      if (ret != -1)
      {  
         readBuff_serial[cnt] = buf;
         cnt++;
         //-if reached the '\n' -> success and end of the loop
         if (readBuff_serial[cnt-1] =='\n')
         {
            result = SUCCESS_SERIAL_READ;
         }
         retry = 0;
      }
      //-If cannot read retry with a delay (max 100 times)
      else
      {  
         retry++;
         usleep(1000);
      }
      //-If tried 100 timed -> Fail and exit loop
      if(retry == 100)
      {  
         result = ERROR_SERIAL_READ;
      }
   }
   return result;
}

