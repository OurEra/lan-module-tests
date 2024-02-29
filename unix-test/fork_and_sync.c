#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
int main(int argc,char **argv)
{
	pid_t pid;
	int i;
	printf("PID = %d\n",getpid());
	pid=fork();
	if(pid==0){
		for(i=0;i<10;i++)
		{
			printf("child PID = %d,parrent PID = %d\n",getpid(),getppid());
			sleep(1);
		}
	}else if(pid!=-1)
	{
		for(i=0;i<10;i++)
		{
			printf("parrent PID = %d,child PID = %d\n",getppid(),getpid());
			sleep(2);
		}
	}else{
		printf("error fork() child !");
	}
	
	return 0;
}
