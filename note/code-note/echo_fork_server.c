#include"echo_net.h"

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

static int my_readline(int fd,void *buf,unsigned len)
{
	int n,rc;
	char c,*ptr;
	ptr=buf;
	for(n=1;n<len;n++)
	{
		again:
		if((rc=read(fd,&c,1))==1)
		{
			*ptr++=c;
			if(c=='\n')break;
		}else if(rc==0)
		{
			if(n==1)return 0;
			else break;
		}else
		{
			if(errno==EINTR)goto again;
			return -1;
		}
	}
	*ptr=0;
	return n;
}

int proc_echo(int consock)
{
	int ret;
	char buf[BUFFER_SIZE];
	while(1)
	{
		if((ret=my_readline(consock,buf,sizeof(buf)))<0)
		{
			perror("read");
			break;
		}else if(ret==0)
		{
			printf("client disconnect.\n");
			break;
		}
		if((ret=write(consock,buf,ret))<0)
		{
			perror("write");
			break;
		}
	}
	close(consock);
	return ret;
}

int main(int argc,char **argv)
{
	int sockfd,nsock;
	int ret=RET_OK;
	uint32_t len;
	pid_t pid;
	struct sockaddr_in servaddr,cliaddr;

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("ERROR opening socket");
		return RET_ERR;
	}
	memset(&servaddr,0,sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(ECHO_PORT);
	if((ret=bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)))<0)
	{
		perror("ERROR on binding");
		goto failed;
	}
	if((ret=listen(sockfd,LISTEN_QUEUE_NUM))!=0)
	{
		perror("ERROR on listen");
		goto failed;
	}
	
	if((set_signal(SIGCHLD,sig_routine))<0)
	{
		perror("ERROR on set_signal");
		goto failed;
	}
	

	while(1)
	{
		len=sizeof(cliaddr);
		if((ret=accept(sockfd,(struct sockaddr*)&cliaddr,(uint32_t*)&len))<0)
		{
			if(errno==EINTR)continue;
			perror("accept");
			break;
		}
		nsock=ret;

		if((pid=fork())<0)break;
		else if(pid==0)
		{
			close(sockfd);
			return 	proc_echo(nsock);
		}
		close(nsock);

	}
	failed:
	close(sockfd);
	return ret<0?RET_ERR:RET_OK;
}


