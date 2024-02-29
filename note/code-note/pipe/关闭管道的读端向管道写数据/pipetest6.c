#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

/*
 close pipe read part write to pipe cause 
 pipe break out
*/
void func(int signum)
{
 printf("pipe break out\n");
}

int main()
{
  const char *string = { "A sample message" };
  int ret, myPipe[2];
  char buffer[80 + 1];

  signal(SIGPIPE,func);
  /*create a pipe */
  ret = pipe(myPipe);
  if (ret == -1) {
    perror("pipe error");
    exit(-1);
  } else if (ret == 0) {
    /*close pipe read part */
    close(myPipe[0]);
    /*write to pipe */
    int bytes_write = write(myPipe[1], string, strlen(string));
    if (bytes_write == -1) {
      perror("write pipe error");
      exit(-1);
    } else {
      printf("bytes write is:%d\n", bytes_write);
    }
  }
  return 0;
}
