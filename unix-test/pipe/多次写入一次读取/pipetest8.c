#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char *string1,*string2;
  int ret, myPipe[2];
  char buffer[80 + 1];

  string1=malloc(50);
  string2=malloc(50);
  strcpy(string1," a simple message ");
  strcpy(string2," hello world ");
 
  /*create a pipe */
  ret = pipe(myPipe);
  if (ret == -1) {
    perror("pipe error");
    exit(-1);
  } else if (ret == 0) {
   
    /*write to pipe */
    int bytes_write = write(myPipe[1], string1, strlen(string1));
    if (bytes_write == -1) {
      perror("write pipe error");
      exit(-1);
    }
    /*write to pipe again */
    bytes_write = write(myPipe[1], string2, strlen(string2));
    if (bytes_write == -1) {
      perror("write pipe error");
      exit(-1);
    }
    /*read from pipe */
    int bytes_read = read(myPipe[0], buffer,sizeof(buffer));
    if (bytes_read == -1) {
      perror("read pipe error");
      exit(-1);
    }
    buffer[bytes_read] = 0;
    printf("%s\n", buffer);
  }
  return 0;
}
