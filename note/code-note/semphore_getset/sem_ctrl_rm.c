#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

void delete_sem(int sem_id)
{
  if ((semctl(sem_id, 0, IPC_RMID, 0)) == -1) {
    perror("sem remove error\n");
    exit(0);
  }
}

int main(int argc, char **argv)
{
  if (argc < 2) {
    printf("usage:setval:1 key");
    exit(0);
  }
  int semid = atol(argv[1]);
  delete_sem(semid);
  return 0;
}
