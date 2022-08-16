/**************************************************************************//**
 * @file        thread_1.c
 * @brief       First example with threads
 * @author      Armando Rivero
 * @date        c: 2022.01.11
 * @date        m: 2022.01.11
 * @version     1.00
 * @bug         -
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>

// A normal C function that is executed as a thread
// when its name is specified in pthread_create()
void *myThreadFun(void *vargp)
{
  sleep(1);
  printf("Printing text from Thread \n");
  return NULL;
}

int main()
{
  printf("\n**************************************************\n");
  printf(" ------ Starting thread 1 example --------\n");
  printf(" ------ date: 2022.01.11          --------\n");
  printf(" ------ ver:  1.00                --------\n");
  printf(" ------ Test: 1                   --------\n");
  printf("**************************************************\n");

  pthread_t thread_id;
  char threadCreationCheck = 0;

  printf("Before Thread\n");
  threadCreationCheck = pthread_create(&thread_id, NULL, myThreadFun, NULL);
  
  if (threadCreationCheck == 0) 
  {printf("\t-> Thread creation: OK\n");
  }else{ printf("\t-> Thread NOT created\n");}

  /* wait for our thread to finish before continuing */
  pthread_join(thread_id, NULL);
  printf("After Thread\n");

  exit(0);
}
