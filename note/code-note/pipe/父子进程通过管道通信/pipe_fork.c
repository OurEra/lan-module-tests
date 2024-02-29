#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>

#define MAX_LINE        80

int main()
{
  int thePipe[2];
  char buf[MAX_LINE + 1];
  char buf2[MAX_LINE + 1];
  const char *testbuf = { "a test string." };
  /*create a pipe*/
  if (pipe(thePipe) == 0) {
    pid_t val = fork();
    if (val == 0) {
     /*child process part*/
      sleep(1);
      close(thePipe[1]);
      /*read from pipe*/
      int bytes_read = read(thePipe[0], buf, MAX_LINE);
      if (bytes_read == -1) {
	perror("read pipe error");
	exit(-1);
      }
      buf[bytes_read] = 0;
      printf("Child read %s\n", buf);
      close(thePipe[0]);
    } else if (val > 0) {
      /*parent part process*/
      close(thePipe[0]);
      /*write to pipe*/
      int bytes_write = write(thePipe[1], testbuf, strlen(testbuf));
      if (bytes_write == -1) {
	perror("write pipe error");
	exit(-1);
      }
      close(thePipe[1]);
    }
  }
  return 0;
}
