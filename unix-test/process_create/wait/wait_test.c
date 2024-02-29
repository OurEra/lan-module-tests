#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*this function illustrate wait function*/
int main()
{
  pid_t pc = fork();
  if (pc < 0) {
    //fork error
    printf("create child prcocess error\n");
    exit(1);
  } else if (pc == 0) {
    //child process part
    printf("child pid is:%d\n", getpid());
    sleep(10);
    exit(0);
  } else {
    //parent process part
    printf("parent pid is: %d\n", getpid());
    printf("waiting child exit.\n");
    pid_t pr = wait(NULL);
    if (pr > 0) {
      printf("catched child pid is: %d\n", pr);
    } else {
      printf("wait error\n");
    }
  }
  exit(0);
}
