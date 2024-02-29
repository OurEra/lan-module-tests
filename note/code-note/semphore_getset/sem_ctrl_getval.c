#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int get_sem_value(int semid, int index)
{
  int semval = semctl(semid, index, GETVAL, 0);
  return semval;
}

int main(int argc, char **argv)
{
  if (argc < 3) {
    printf("usage:%s:semid index\n", argv[0]);
    exit(0);
  }
  int semid = atol(argv[1]);    //sem id
  int index = atoi(argv[2]);    //sem index
  int semval = get_sem_value(semid, index);
  printf("sem value is :%d\n", semval);
}
