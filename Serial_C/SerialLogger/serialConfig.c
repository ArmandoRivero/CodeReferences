/**************************************************************************//**
 * @file        serialConfig.c
 * @brief       Serial Logger configuration
 * @author      Armando Rivero
 * @date        c: 2019.05.21
 * @date        m: 2019.10.14
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

struct termios serialOptions;

/*******************************************************************************
******************************************************************************/
int open_port(char *serialName)
{
  int fd; /* File descriptor for the port */

  //~ fd = open(serialName, O_RDWR | O_NOCTTY | O_NDELAY);
  fd = open(serialName, O_RDWR);// | O_NDELAY);
  if (fd == -1){
    perror(" Open_port: Unable to open the VCP");
  }else{
    perror(" Open_port: VCP successfully opened!");
    fcntl(fd, F_SETFL, 0);
  }


  /* Configuring serial port  */
  tcgetattr(fd, &serialOptions);

  cfsetispeed(&serialOptions, BAUDRATE);
  cfsetospeed(&serialOptions, BAUDRATE);

  serialOptions.c_cflag |= (BAUDRATE | CLOCAL | CREAD);

  tcsetattr(fd, TCSANOW, &serialOptions);

  return (fd);
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
int write_port(int fd, const char *buf, int size)
{
  return write(fd, buf, size);
}


/*******************************************************************************
******************************************************************************/
int read_port(int fd, char *buf, int size)
{
   int count = size;
   int tryRead = 0;
   int ret;
   //printf("<-> count_init %d\n",count);
   while(count)
   {
      ret = read(fd, buf, count);
      //~ printf("<-> read ret:%d\n",ret);
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


/*******************************************************************************
******************************************************************************/
int close_port(int fd)
{
  close(fd);

  return 0;
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

