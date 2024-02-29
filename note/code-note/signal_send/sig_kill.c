//sig_kill.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
 pid_t pid;
 if(argc!=2) 
 {
   printf("Usage : %s PID\n", argv[0]);
   exit(1);
 }
pid = atol(argv[1]);
if(kill(pid,SIGKILL)!=0) 
{
  printf("kill failed\n");
}
 return 0;
}
