/*
 * shm-at-dt.c - Attaching a shared memory segment
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int shmid;
  char *shmbuf;
  if (argc != 2) {
    printf("USAGE: %s <identifier>", argv[0]);
    exit(EXIT_FAILURE);
  }

  /*attach shared memory to process */
  shmid = atoi(argv[1]);
  shmbuf = shmat(shmid, 0, 0);
  printf("segment attached at %p\n", shmbuf);
  system("ipcs -m");

  /*detach memory from process */
  if ((shmdt(shmbuf)) < 0) {
    perror("shmdt");
    exit(EXIT_FAILURE);
  }
  system("ipcs -m");

  exit(EXIT_SUCCESS);
}
