#include<stdarg.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>

extern char **environ;

void pid_printf(char *format,...)//自定义printf,在每行输出前插入进程ID
{
	va_list ap;
	va_start(ap,format);
	printf("%d:",getpid());//每行前插入
	vprintf(format,ap);
}

void signal_handler(int signo)//收到信号后的处理fun
{
	int ret;
	pid_t pid_c;
	switch(signo)
	{
		case SIGCHLD:
			pid_c=wait(&ret);//使进程睡眠，直到它的一个子进程退出时唤醒
			//pid_t wait(int *status) 返回值是退出的子进程的pid , status是子进程退出的状态信息 
			//还有一个是pid_t waitpid(pid_t pid,int *status,int options)
			pid_printf("Child process PID = %d return %d.\n",pid_c,ret);
			break;
		case SIGUSR2:
			pid_printf("Signal SIGUSR2 received\n");
			break;
		default:
			pid_printf("Signal %d reveived.\n",signo);
			break;
	}

}
int main(int argc,char **argv)
{
	pid_t pid;
	signal(SIGCHLD,signal_handler);//捕捉SIGCHLD信号
	signal(SIGUSR2,signal_handler);//捕捉SIGUSR2信号
	pid=fork();
	if(pid==0)
	{
		pid_printf("Child process send signal SIGUSR2 to parent proess.\n");

		kill(getppid(),SIGUSR2);//发送信号SIGUSR2给父进程
		//int kill(pid_t pid,int sig)
		//pid>0 给pid的进程发信号 ， pid=0全同一进程组所有进程发信号，
		//pid<-1给其它的进程组为(-pid)的发信号, pid=-1给除自身之外的>1的进程发信号

		pid_printf("Child process will exit with status 0.\n");
		exit(0);
	}else if(pid!=-1)
	{
		while(1)sleep(1);
	}else
	{
		pid_printf("There was a error with fork() !\n");
	}

	return 0;
}


