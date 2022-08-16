/**************************************************************************//**
 * @file        thread_single.c
 * @brief       simple example with threads
 * @author      Armando Rivero
 * @date        c: 2020.03.09
 * @date        m: 2020.03.09
 * @version     1.00
 * @bug         -
 *****************************************************************************/
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


/* This is our thread function.  It is like main(), but for a thread */
void *threadFunc(void *arg)
{
  char *str;
  int i = 0;

  str = (char*)arg;

  while(i < 10 )
  {
    usleep(1);
    ++i;
    printf("%d threadFunc says: %s\n",i, str);
  }

  return NULL;
}




int main(int argc, char const *argv[])
{
  printf("pid of program is %d\n" , getpid());


  printf("\n**************************************************\n");
  printf(" ------ Starting single thread example --------\n");
  printf(" ------ ver:  1.00                     --------\n");
  printf(" ------ Test: 1                        --------\n");
  printf("**************************************************\n");


  pthread_t pth;  // this is our thread identifier
  int i = 0;

  /* Create worker thread */
  pthread_create(&pth, NULL, threadFunc, "processing...");

  /* wait for our thread to finish before continuing */
  pthread_join(pth, NULL /* void ** return value could go here */);

  while(i < 10 )
  {
    usleep(1);
    ++i;
    printf("%d main() is running...\n",i);
  }

  return 0;
}
// --------------------------------------------------------------------------
