#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*this function illustrate wait and exit value*/
int main()
{
  int status;
  pid_t pc, pr;
  pc = fork();
  if (pc < 0) {
    //fork error
    printf("error occured\n");
    exit(-1);
  } else if (pc == 0) {
    //child process part
    printf("This is child process with pid of %d.\n", getpid());
    exit(3);
  } else {
    //parent process part
    pr = wait(&status);
    if (WIFEXITED(status)) {
      printf("The child process %d exit normally.\n", pr);
      printf("the return code is %d.\n", WEXITSTATUS(status));
    } else {
      printf("The child process %d exit abnormally.\n", pr);
    }
  }
  exit(0);
}
