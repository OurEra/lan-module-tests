#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

main()
{
  sigset_t *set;
  set = (sigset_t *) malloc(sizeof(set));
  sigemptyset(set);
  sigaddset(set, SIGINT);
  sigaddset(set, SIGQUIT);
  sigprocmask(SIG_SETMASK, set, NULL);
  while (1);
}
