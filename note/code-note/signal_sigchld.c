#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<unistd.h>
//此代码是样本,并不可单独执行

static void sig_routine(int signo)
{
	switch(signo)
	{
		case SIGCHLD:
			printf("in sig routine\n");
			while(waitpid(-1,NULL,WNOHANG)>0)break;
	}
	return;
}
void (*set_signal(int signo,void(*func)(int)))(int)
{
	struct sigaction act,oact;
	act.sa_handler=func;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	if(signo!=SIGALRM)act.sa_flags|=SA_RESTART;
	if(sigaction(signo,&act,&oact)<0)return NULL;
	return (oact.sa_handler);
}

int main()
{
	set_signal(SIGCHLD,sig_routine);
	//...
	if(fork()==0)return process();
	//...
}
