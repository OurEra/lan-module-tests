//sigdemo6.c
#include <signal.h>
#include <stdio.h>

void sig_usr(int);
main()
{
  if(signal(SIGINT,sig_usr)==SIG_ERR)
    perror("error");
  while(1);
}
void sig_usr(int signo)
{
  if(signo==SIGINT)
     printf("received CTRL_C signal\n");
  kill(getpid(),SIGKILL);
}
