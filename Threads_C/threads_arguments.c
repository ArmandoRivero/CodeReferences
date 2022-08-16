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


struct thread_data {
   int  thread_id;
   char *message;
};


void *printText(void *threadarg){
  struct thread_data *my_data;

  my_data = (struct thread_data *) threadarg;

  printf(" --> thread ID: %d\n", my_data->thread_id );
  printf(" --> message: %s\n", my_data->message );

  pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{
  printf("pid of program is %d\n" , getpid());

  printf("\n**************************************************\n");
  printf(" ------ Starting thread with arguments example -----\n");
  printf(" ------ Test: 1.00                             -----\n");
  printf("**************************************************\n");


  pthread_t threads[THREADS_NUMBER];

  struct thread_data td[THREADS_NUMBER];

  int rc;
  int i;

  char buffer[100];

  for(i=0; i<THREADS_NUMBER; i++){
    printf("Creating thread: %d\n", i);

    td[i].thread_id = i;
    snprintf(buffer, 28, "This message for thread (%d)", i);
    td[i].message = buffer;

    rc = pthread_create(&threads[i], NULL, printText, (void *) &td[i]);
    if (!rc){
      printf(" -> Thread created: %d\n", i);
    }
    else if (rc){
      printf("ERROR: unable to create thread, %d", rc);
    }
  }

  pthread_exit(NULL);

  return 0;
}
// --------------------------------------------------------------------------
