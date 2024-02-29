#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  char buf[100];
  int fd, readnum;
  
  if (argc < 2) {
    printf("usage:fiforead fifoname\n");
    exit(1);
  }

  printf("Preparing for reading bytes... ...\n");

  /*open fifo */
  if ((fd = open(argv[1], 0666)) < 0) {
    perror("open fifo error");
    exit(1);
  }

  /*read from fifo */
  while (1) {
    bzero(buf, sizeof(buf));
    if ((readnum = read(fd, buf, sizeof(buf))) < 0) {
      if (errno == EAGAIN) {
	printf("no data yet\n");
      }
    }
    if (readnum != 0) {
      buf[readnum] = '\0';
      printf("read %s from FIFO_SERVER\n", buf);
    }
    sleep(1);
  }
  return 0;
}
