#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
void PrintMsg(int Num)
{
	printf("Hello World\n");
}
main(int argc, char* argv[])
{
	signal(SIGALRM, PrintMsg);
	struct itimerval tick;
	tick.it_value.tv_sec = 10;  
	tick.it_value.tv_usec = 0;
  tick.it_interval.tv_sec  =1;     
	tick.it_interval.tv_usec = 0;
  int ret = setitimer(ITIMER_REAL, &tick, NULL);
	getchar();
} 