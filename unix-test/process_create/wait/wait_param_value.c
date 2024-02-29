#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define	DELAY	5

/*this program illustrate wait parameter meaning*/
int main(int argc, char **argv)
{
  int newpid;
  void child_code(), parent_code();
  printf("before: mypid is %d\n", getpid());
  if ((newpid = fork()) == -1) {
  //fork error
    perror("fork");
    exit(-1);
  } else if (newpid == 0) {
  //child process part
    child_code(DELAY);
  } else {
  //parent process part
    parent_code(newpid);
  }
}

void child_code(int delay)
{
  printf("child %d sleep %d seconds\n", getpid(), delay);
  sleep(delay);
  printf("child done. about to exit\n");
  exit(17);
}

void parent_code(int childpid)
{
  int wait_rv;			/* return value from wait() */
  int child_status;             /*wait parameter*/
  int high_8, low_7, bit_7;
  wait_rv = wait(&child_status);
  printf("wait for %d. Wait returned: %d\n", childpid, wait_rv);
  high_8 = child_status >> 8;	/* 1111 1111 0000 0000 */
  low_7 = child_status & 0x7F;	/* 0000 0000 0111 1111 */
  bit_7 = child_status & 0x80;	/* 0000 0000 1000 0000 */
  printf("status: exit=%d, sig=%d, core=%d\n", high_8, low_7, bit_7);
}
