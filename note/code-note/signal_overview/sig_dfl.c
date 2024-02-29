//sig_dfl.c
//演示信号如何执行缺省操作
#include <stdio.h>
#include <signal.h>

main()
{
  signal(SIGINT,SIG_DFL);
  while(1){
  sleep(1);
  printf("haha!!!!!!!!!!!!!\n");
 }
}
