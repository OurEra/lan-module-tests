#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

/*this function illustrate fork function*/
int main(int argc, char **argv)
{
  printf("process pid is:%d,parent process id is:%d\n", getpid(),
	 getppid());
  pid_t val = fork();
  /*fork error */
  if (val == -1) {
    perror("fork error\n");
    exit(-1);
  }/*child process */
  else if (val == 0) {
    printf("val value is:%d,process pid is:%d,parent process id is:%d\n",
	   val, getpid(), getppid());
  }/*parent process */
  else if (val > 0) {
    printf("val value is:%d,process pid is:%d,parent process id is:%d\n",
	   val, getpid(), getppid());
  }
}
