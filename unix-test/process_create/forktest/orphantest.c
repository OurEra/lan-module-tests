#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

/*this function illustrate the orphan process*/
int main(int argc, char **argv)
{
  printf("process pid is:%d,parent process id is:%d\n", getpid(),
	 getppid());
  pid_t val = fork();
  /*fork error */
  if (val == -1) {
    perror("fork error\n");
    exit(-1);
  }  /*parent process */
  else if (val > 0) {
    printf("process pid is:%d,parent process id is:%d\n", getpid(),
	   getppid());
  } /*child process*/
  else if (val == 0) {
    int i = 1;
    while (i < 60) {
      printf("process pid is:%d,parent process id is:%d\n", getpid(),
	     getppid());
      sleep(1);
      i++;
    }
  }
}
