#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

/*get semaphore*/
int main(void)
{
  int semid;			/* Semaphore identifier */
  int nsems = 2;		/* How many semaphores to create */
  int flags = 0666;		/* World read-alter mode */
  key_t key = ftok(".", 'a');
  semid = semget(key, nsems, flags | IPC_CREAT);
  if (semid < 0) {
    perror("semget");
    exit(EXIT_FAILURE);
  }
  printf("semaphore created: %d\n", semid);
  system("ipcs -s");
  exit(EXIT_SUCCESS);
}
