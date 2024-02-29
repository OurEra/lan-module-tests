#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

//get message queue
int main(void)
{
  key_t key= 113;
  int qid = msgget(key, IPC_CREAT | 0666);
  if (qid < 0) {
    perror("msgget");
    exit(1);
  }
  printf("creat queue id : %d \n", qid);
  system("ipcs -q");
  exit(0);
}
