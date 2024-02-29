#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#define SEGSIZE 100

void removeshm(int shmid)
{
  shmctl(shmid, IPC_RMID, 0);
  printf("shared memory segmentmask for deletion\n");
}

int main(int argc, char *argv[])
{
  int shmid;
  if (argc < 2) {
    printf("Useage:shm-rt key\n");
    exit(0);
  }
  key_t key = atol(argv[1]);
  if ((shmid = shmget(key, SEGSIZE, IPC_CREAT | IPC_EXCL | 0666)) == -1) {
    printf("shared memory segment exit-opening as client\n");
    if ((shmid = shmget(key, SEGSIZE, 0)) == -1) {
      perror("shmget");
      exit(1);
    }
  } else {
    printf("Creating new shared memory segment\n");
  }
  removeshm(shmid);
  return 0;
}
