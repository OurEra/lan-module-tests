#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>

static void (*set_signal(int signo,void(*func)(int)))(int)
{
	struct sigaction act,oact;
	act.sa_handler=func;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	if(signo!=SIGALRM) act.sa_flags|=SA_RESTART;
	if(sigaction(signo,&act,&oact)<0) return NULL;
	return(oact.sa_handler);
}	

static void sig_routine(int signo)
{
	switch(signo)
	{
		case SIGCHLD:
			while(waitpid(-1,NULL,WNOHANG)>0);
			break;
	}
	return;
}

void make_daemon()
{
	pid_t pid;
	if((pid=fork())!=0)exit(0);
	setsid();
	set_signal(SIGHUP,SIG_IGN);
	set_signal(SIGCHLD,sig_routine); //sig_routine()信号处理,调用waitpid()避免僵尸进程
	if((pid=fork())!=0)exit(0);
	
	chdir("/");
	umask(0);
}

int main()
{
	make_daemon();
	return 0;
}
