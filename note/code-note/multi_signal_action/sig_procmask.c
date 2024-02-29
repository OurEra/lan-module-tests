#include <signal.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  sigset_t intmask;
  int i = 0;
  sigemptyset(&intmask);
  sigaddset(&intmask, SIGINT);
  while (1) {
    sigprocmask(SIG_BLOCK, &intmask, NULL);
    for (i = 0; i < 5; i++) {
      sleep(1);
      printf("blocked SIGINT:i is :%d\n", i);
    }
    sigprocmask(SIG_UNBLOCK, &intmask, NULL);
    for (i = 10; i < 15; i++) {
      sleep(1);
      printf("unblocked SIGINT:i is :%d\n", i);
    }
  }
  exit(0);
}
