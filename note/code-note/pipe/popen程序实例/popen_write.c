#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
  FILE *stream;
  FILE *wstream;
  char buf[1024];

  if (argc < 3) {
    printf("usage:popen command filename\n");
    exit(-1);
  }

  memset(buf, '\0', sizeof(buf));

  //use popen start a process with pipe
  stream = popen(argv[1], "w");
  if (stream == NULL) {
    perror("popen error");
    exit(-1);
  }
  //open  a file with write mode
  wstream = fopen(argv[2], "r");
  if (wstream == NULL) {
    perror("fopen error");
    exit(-1);
  }
  //read from pipe and write to above file
  fread(buf, sizeof(char), sizeof(buf), wstream);
  fwrite(buf, 1, sizeof(buf), stream);
  pclose(stream);
  fclose(wstream);
  return 0;
}
