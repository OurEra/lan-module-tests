#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
  this program illustrate interchange pipe
  read part and write part to prove pipe
  commuication with single direction
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
    /*write to pipe read part*/
    int bytes_write = write(myPipe[0], string, strlen(string));
    if (bytes_write == -1) {
      perror("write pipe error");
      exit(-1);
    }
    /*read from pipe write part*/
    int bytes_read = read(myPipe[1], buffer, bytes_write);
    if (bytes_read == -1) {
      perror("read pipe error");
      exit(-1);
    }
    buffer[bytes_read] = 0;
    printf("%s\n", buffer);
  }
  return 0;
}
