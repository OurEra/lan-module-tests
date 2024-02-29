#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

#define MAXFILE 65535

int main()
{
  char *buf = "this is a Dameon\n";
  int len = strlen(buf);
  pid_t pc = fork();
  if (pc < 0) {
    printf("error fork\n");
    exit(1);
  } else if (pc > 0)
    exit(0);
  setsid();
  chdir("/");
  umask(0);
  int i = 0, fd = -1;

  /* close all files */
  for (i = 0; i < MAXFILE; i++) {
    close(i);
  }

  /*write text string to file*/
  while (1) {
    if ((fd = open("/tmp/dameon.log", O_CREAT | O_WRONLY | O_APPEND,
		   0600)) < 0) {
      perror("open");
      exit(1);
    }
    if (write(fd, buf, len + 1) == -1) {
      close(fd);
      break;
    }
    close(fd);
    sleep(10);
  }
}
