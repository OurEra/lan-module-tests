//sigdemo7.c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(){
  printf("hello\n");
 }
main()
{ 
    signal(SIGUSR1,handler);
    raise(SIGUSR1);
}
