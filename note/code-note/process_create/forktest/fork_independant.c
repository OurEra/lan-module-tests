#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

/*this function illustrate after forking 
  parent process and child process run in
  their independant space
*/
int main(int argc, char **argv)
{
  int value=100;
  pid_t val = fork();
  /*fork error */
  if (val == -1) {
    perror("fork error\n");
    exit(-1);
  }else if (val == 0) {/*child process */
    value++;
    printf("val is:%d,value is:%d\n",val,value);
    sleep(30);
  }else if (val > 0) {/*parent process */
    value--;
    printf("val is:%d,value is:%d\n",val, value);
    wait();
  }
}
