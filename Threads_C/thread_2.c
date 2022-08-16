/**************************************************************************//**
 * @file        thread_2.c
 * @brief       Second example with threads
 * @author      Armando Rivero
 * @date        c: 2022.01.11
 * @date        m: 2022.01.12
 * @version     1.00
 * @bug         -
 * https://stackoverflow.com/questions/19232957/pthread-create-and-passing-an-integer-as-the-last-argument
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *myThreadFun(void *cnt)
{
  long cnt1;
  cnt1 = *((long *)cnt);
  printf("\tPrinting text from Thread (%ld)\n", cnt1);
  if(cnt1 == 2)
  {
    printf("\tcnt is two !!!\n");
  }
  return NULL;
}

int main(void)
{
  printf("\n**************************************************\n");
  printf(" ------ Starting thread 2 example --------\n");
  printf(" ------ date: 2022.01.11          --------\n");
  printf(" ------ ver:  1.00                --------\n");
  printf(" ------ Test: 1                   --------\n");
  printf("**************************************************\n");
  
  pthread_t thread_id;
  char threadCreationCheck = 0;
  int i = 0;
  
  int *cnt = malloc(sizeof(*cnt));
  // if ( cnt == NULL ) {
  //     fprintf(stderr, "Couldn't allocate memory for thread cnt.\n");
  //     exit(EXIT_FAILURE);
  // }


  for (i=0; i<100;i++)
  {
    printf("i=%d\n",i);
    *cnt = i;

    printf("Before Thread\n");
    // threadCreationCheck = pthread_create(&thread_id, NULL, myThreadFun, (void *)cnt);
    threadCreationCheck = pthread_create(&thread_id, NULL, myThreadFun, cnt);

    /* wait for our thread to finish before continuing */
    //pthread_join(thread_id, NULL);
    printf("After Thread\n");

  }

  exit(0);
}
