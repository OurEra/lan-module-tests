#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

/*this function illustrate use wait to avoid zombie process*/
int main(int argc, char **argv)
{
  printf("process pid is:%d,parent process id is:%d\n", getpid(),
	 getppid());
  pid_t val = fork();
  if (val == -1) {
    perror("fork error\n");
    exit(-1);
  } else if (val == 0) {
    /*child process part*/
    printf("process pid is:%d,parent process id is:%d\n", getpid(),
	   getppid());
  } else if (val > 0) {
    /*parent process part*/
    int i = 1;
    wait(NULL);
    while (i < 20) {
      printf("process pid is:%d,parent process id is:%d\n", getpid(),
	     getppid());
      sleep(1);
      i++;
    }
  }
}
