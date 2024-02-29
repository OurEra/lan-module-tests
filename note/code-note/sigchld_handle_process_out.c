#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
//主进程接收子进程中止信号并处理，可称为软中断
//主进程 wait时被阻塞，其它的代码将被暂停，不能运行。
//用信号控制效率较高，主进程无需阻塞wait子进程，只需收到信号时处理就可

extern char **environ;

void handle_sig_child()//捕捉SIGCHLD信号后的处理fun
{
	int ret;	
	pid_t pid_c;
	pid_c=wait(&ret);//使进程睡眠，直到它的一个子进程退出时唤醒
	//pid_t wait(int *status) 返回值是退出的子进程的pid , status是子进程退出的状态信息 
	//还有一个是pid_t waitpid(pid_t pid,int *status,int options)
	printf("Child proess PID = %d return %d.\n",pid_c,ret);
}

int main(int argc,char **argv)
{
	pid_t pid;
	signal(SIGCHLD,handle_sig_child);//捕捉SIGCHLD信号,并调取handle_sig_child()进行处理 ,(信号值，处理func)
	//signal 不支持信号传递信息，用于前32种非实时信号的安装，
	pid=fork();//创建了一个子进程
	if(pid==0)
	{	//子进程代码区
		printf("Child PID = %d will exit with status 0.\n",getpid());
		exit(0);
	}else if(pid!=-1)
	{
		//主进程代码区
		while(1)sleep(1);

	}else
	{
		printf("there was an error with forking!\n");
	}
	

	return 0;
}
