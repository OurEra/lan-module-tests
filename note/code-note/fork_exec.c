#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
int main(int argc,char **argv)
{
	pid_t pid;
	printf("PID = %d\n",getpid());
	pid=fork();
	if(pid==0)
	{
		printf("\nchild proess,PID = %d\n",getpid());
		printf("parrent proess,PPID = %d\n",getppid());
		execvp("ls",argv);
		//execv("/bin/ls",argv);
		//execl("/bin/ls","ls","-l","/",NULL);
		//指令径，指令，参数，目录，空
		//execlp("ls","ls","-al","/",NULL);
	}else if(pid!=-1){
		printf("\nParrent porcess,PID = %d\n",getpid());
	}else{
		printf("error fork() child proess!");
	}
	return 0 ;
}
