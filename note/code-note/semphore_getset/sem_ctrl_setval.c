#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int set_sem_value(int semid, int index, int semval)
{
  struct sembuf lock_it;	/*semop data structure */

  /*semctrl union */
  union semun {
    int val;			//SETVAL
    struct semid_ds *buf;	//IPC_STAT |IPC_SET
    unsigned short *ary;	//SETALL |GETALL
  } options;

  /*set value of semaphore */
  options.val = semval;
  if ((semctl(semid, index, SETVAL, options)) == -1) {
    perror("setctl error");
    return -1;
  }
  return 0;
}

int main(int argc, char **argv)
{
  if (argc < 4) {
    printf("usage:%s:semid index semval\n", argv[0]);
    exit(0);
  }
  int semid = atol(argv[1]);         //sem id
  int index = atoi(argv[2]);         //sem index
  int semval = atoi(argv[3]);        //sem value
  if ((set_sem_value(semid, index, semval)) == -1) {
    perror("set sem value error");
    exit(-1);
  }
}
