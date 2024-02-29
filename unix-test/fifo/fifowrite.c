#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{
  int fd, nwrite;

  if (argc < 3) {
    printf("usage:write fifoname string\n");
    exit(1);
  }


  /*open fifo */
  if ((fd = open(argv[1], 0666)) < 0) {
    perror("open");
    exit(1);
  }

  /*write to fifo */
  if ((nwrite = write(fd, argv[2], strlen(argv[2]))) < 0) {
    if (errno == EAGAIN) {
      printf("The FIFO has not been read yet.Please try later\n");
    }
  } else {
    printf("write %s to FIFO\n", argv[2]);
  }
  return 0;
}

