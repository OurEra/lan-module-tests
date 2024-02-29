#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int set_sem_op(int semid, int index, int semop_val)
{
  struct sembuf lock_it;
  lock_it.sem_num = index;
  lock_it.sem_op = semop_val;
  lock_it.sem_flg = IPC_NOWAIT;
  if (semop(semid, &lock_it, 1) == -1) {
    printf("can not lock semaphore.\n");
    exit(1);
  }
}

int main(int argc, char **argv)
{
  if (argc < 4) {
    printf("usage:%s:semid nsems semval", argv[0]);
    exit(0);
  }
  int semid = atol(argv[1]);         //sem id
  int index = atoi(argv[2]);         //sem index
  int semop_val = atoi(argv[3]);     //sem operation
  if ((set_sem_op(semid, index, semop_val)) == -1) {
    perror("set sem value error");
    exit(-1);
  }
}
