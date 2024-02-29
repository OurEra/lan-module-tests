#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
extern char **environ;
int main(int argc,char **argv)
{
	int ret;
	pid_t pid;
	pid_t pid_c;
	pid=fork();//子进程
	if(pid==0){
		printf("Child PID=%d will exit with status 0.\n",getpid());
		exit(0);
	}else if(pid!=-1){
		pid_c=wait(&ret);//使进程睡眠，直到它的一个子进程退出时唤醒
		//pid_t wait(int *status) 返回值是退出的子进程的pid , status是子进程退出的状态信息 
		//还有一个是pid_t waitpid(pid_t pid,int *status,int options)
		printf("Child Pid=%d return %d\n",pid_c,ret);
	}else{
		printf("error with forking!\n");
	}
	return 0;
}
