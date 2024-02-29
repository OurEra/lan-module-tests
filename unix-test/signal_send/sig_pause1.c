//sigdemo11.c
#include <stdio.h>
#include <signal.h>
void wake_up(int signum);
main()
{
  signal(SIGALRM,wake_up);
  alarm(4);
  pause();
  printf("finished process!\n");
}
void wake_up(int signum)
{
 printf("alarm receive from kernel\n");
} 
