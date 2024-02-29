#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

/*this function illustrate fork & execlp usage*/
int main(int argc, char **argv)
{
  if (argc < 3) {
    printf("usage:fork_exec cmd1 option1\n");
    exit(-1);
  }
  pid_t val = fork();
  if (val == -1) {
    perror("fork error\n");
    exit(-1);
  } else if (val == 0) {
    /*child process */
    if ((execlp(argv[1], argv[1], argv[2], NULL)) == -1) {
      perror("execlp error");
      exit(-1);
    }
  } else if (val > 0) {
    /*parent process */
    sleep(2);
  }
}
