#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

int main(int argc,char **argv)
{
	int status;
	pid_t pid;
	pid_t pid_c;
	pid=fork();
	if(pid==0)
	{
		printf("child proess will sleep 5 seconds.\n");
		sleep(5);
		printf("child PID=%d will exit with status 3.\n",getpid());
		exit(3);
	}else if(pid!=-1)
	{
		do{
			pid_c=waitpid(pid,&status,WNOHANG);
			if(pid_c==0)
			{
				printf("Not child proess exit\n");
				sleep(1);
			}
		}while(pid_c==0);
		if(WIFEXITED(status))
		{
			printf("the child proess %d exit normally.\n",pid_c);
			printf("the return code is %d.\n",WEXITSTATUS(status));
		}else
		{
			printf("the child process %d exit abnormally\n",pid_c);
		}
	}else
	{
		printf("There are an error with forking!\n");
	}
	return 0;
}
