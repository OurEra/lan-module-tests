#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
  if (argc != 2) {
    printf("Usage:[%s] fifo_filename\n", argv[0]);
    return -1;
  }
 
 if (unlink(argv[1]) < 0) {
    perror("mkfifo");
    return -1;
  }
  return 0;
}
