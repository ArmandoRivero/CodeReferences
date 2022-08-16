/**************************************************************************//**
 * @file        threads.c
 * @brief       simle example with threads
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


#define THREADS_NUMBER 5

void *printText(void *threadid)
{
  long tid;
  tid = (long)threadid;
  printf("\tthis is thread: %ld\n",tid);
  pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{
  printf("pid of program is %d\n" , getpid());

  printf("\n**************************************************\n");
  printf(" ------ Starting thread example --------\n");
  printf(" ------ ver:  1.00              --------\n");
  printf(" ------ Test: 1                 --------\n");
  printf("**************************************************\n");


  pthread_t threads[THREADS_NUMBER];

  int rc;
  int i;

  for(i=0; i<THREADS_NUMBER; i++)
  {
    printf("-> Creating thread: %d\n", i);
    rc = pthread_create(&threads[i], NULL, printText, (void *)i);
    if (!rc){
      printf("-> Thread created: %d\n", i);
    }
    else if (rc){
      printf("ERROR: unable to create thread, %d", rc);
    }
  }

  for (i=0; i<100;i++)
  {
    printf("%d ",i);
  }

  pthread_exit(NULL);

  return 0;
}
// --------------------------------------------------------------------------
