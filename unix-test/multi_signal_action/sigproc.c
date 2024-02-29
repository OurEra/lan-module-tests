#include <signal.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main()
{
  sigset_t intmask;
  int i;
  sigemptyset(&intmask);
  sigaddset(&intmask, SIGINT);
  while (1) {
     //block all signals in intmask
    sigprocmask(SIG_BLOCK, &intmask, NULL);
    printf("blocked signal Ctrl+C\n");
    for (i = 0; i < 10; i++) {
      printf("%d\n", i);
      sleep(1);
    }

    //unblock all signals in intmask
    sigprocmask(SIG_UNBLOCK, &intmask, NULL);
    printf("unblocked signal Ctrl+C\n");
    for (i = 0; i < 10; i++) {
      printf("%d\n", i);
      sleep(1);
    }
  }
  exit(0);
}
