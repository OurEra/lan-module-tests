#include<sys/wait.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include"pid_printf.h"

void child_process(int pfd[2])
{
	char buf[256];
	close(pfd[0]);//关闭读
	
	pid_printf("Child is ready.\n");

	pid_printf("Enter mess(Ctrl + D to exit): ");

	fflush(stdout);
	while(fgets(buf,sizeof(buf),stdin)!=NULL)
	{
		pid_printf("Transmitting mess: %s",buf);
		write(pfd[1],buf,strlen(buf));
	}
	pid_printf("Child exiting.\n");
	close(pfd[1]);
	exit(EXIT_SUCCESS);
}

void parent_process(int pfd[2])
{
	char buf[256]={0};
	int count;
	int status;

	close(pfd[1]);
	pid_printf("parent is ready.\n");
	while(1)
	{
		count=0;
		while(count< (sizeof(buf)-1))
		{
			status=read(pfd[0],buf+count,1);
			if(status<1)
			{
				pid_printf("read EOF,parent exiting\n");
				close(pfd[0]);
				return;
			}
			if(buf[count]=='\n')
			{
				break;
			}
			count++;
		}
		pid_printf("Recevice mess: %s",buf);
	}

	pid_printf("No more data,parent exiting\n");
	close(pfd[0]);
}

int main(int argc,char **argv)
{
	int pfd[2];	
	pid_t pid;
	
	if(pipe(pfd)==-1)
	{
		perror("PIPE");
		exit(EXIT_FAILURE);
	}

	if((pid=fork())<0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}else if(pid==0)
	{
		child_process(pfd);
	}else
	{
		parent_process(pfd);
		wait(NULL);pid_printf("No more data,parent exiting\n");
		close(pfd[0]);
		exit(EXIT_SUCCESS);
	}
}

