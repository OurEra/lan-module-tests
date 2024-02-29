#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 no process write to pipe
 so as to block pipe read operation
*/
int main()
{
  const char *string = { "A sample message" };
  int ret, myPipe[2];
  char buffer[80 + 1];

  /*create a pipe */
  ret = pipe(myPipe);
  if (ret == -1) {
    perror("pipe error");
    exit(-1);
  } else if (ret == 0) {
    /*no process write to pipe*/ 
    /*read from pipe */
    int bytes_read = read(myPipe[0], buffer, bytes_write);
    if (bytes_read == -1) {
      perror("read pipe error");
      exit(-1);
    }
    buffer[bytes_read] = 0;
    printf("%s\n", buffer);
  }
  return 0;
}
