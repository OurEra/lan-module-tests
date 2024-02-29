#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

/*this function illustrate execlp usage*/
int main()
{
  close(1);
  int fd = open("text", O_WRONLY);
  if ((execlp("/bin/ls", "ls", "-l", NULL)) == -1) {
    perror("execlp error");
    exit(-1);
  }
}
