#include <stdio.h>
#include <signal.h>
main()
{
 int i;
 signal(SIGINT,SIG_IGN);
 while(1){
   sleep(1);
   printf("Haha\n");
 }
}
