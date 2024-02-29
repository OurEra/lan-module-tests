#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
  FILE *stream;
  FILE *wstream;
  char buf[1024];
  memset(buf, '\0', sizeof(buf));
  
  //use popen start a process with pipe
  stream = popen("ls -l", "r");
  if (stream == NULL) {
    perror("popen error");
    exit(-1);
  }

  while(fgets(buf,sizeof(buf), stream) != NULL)
	  printf("%s", buf);

  /*
  //open  a file with write mode
  wstream = fopen("test_popen.txt", "w+");
  if (wstream == NULL) {
    perror("fopen error");
    exit(-1);
  }
  //read from pipe and write to above file
  fread(buf, sizeof(char), sizeof(buf), stream);
  fwrite(buf, 1, sizeof(buf), wstream);
  */
  pclose(stream);
  //fclose(wstream);
  return 0;
}
