#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

main()
{
  sigset_t *set;
  set = (sigset_t *) malloc(sizeof(set));
  sigemptyset(set);
  sigaddset(set, SIGUSR1);
  sigaddset(set, SIGINT);
  if ((sigismember(set, SIGUSR1)) == 1)
    printf("SIGUSR1\n");
  if ((sigismember(set, SIGUSR2)) == 1)
    printf("SIGUSR2\n");
  if ((sigismember(set, SIGINT)) == 1)
    printf("SIGINT\n");
}
