/*
使用select()复用IO的TCP_ECHO客户端
*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<errno.h>

#define RET_OK 0
#define RET_ERR -1

#define LISTEN_QUEUE_NUM 5

#define BUFFER_SIZE 256

#define ECHO_PORT 2029

int main(int argc,char **argv)
{
	int maxfd=0;
	fd_set rset,set;
	int nfound,bytesread;
	int sockfd;
	int ret=RET_OK;
	struct sockaddr_in servaddr;
	struct hostent *server;
	char buffer[BUFFER_SIZE];

	if(argc<2)
	{
		fprintf(stderr,"usage &s hostname\n",argv[0]);
		return RET_ERR;
	}
	
	if((server=gethostbyname(argv[1]))==NULL)
	{
		herror("gethostbyname.");
		return RET_ERR;
	}
	
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		herror("ERROR opening socket");
		return RET_ERR;
	}


	memset(&servaddr,0,sizeof(servaddr));//申请信息表空间

	servaddr.sin_family=AF_INET;//设为IPv4
	servaddr.sin_addr.s_addr=*(uint32_t *)server->h_addr;//地址转换,32位大端字序
	servaddr.sin_port=htons((uint16_t)ECHO_PORT);//设定端口,16位大端字序转换

	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		perror("ERROR connect");
		return RET_ERR;
	}

	maxfd=fileno(stdin);
	FD_ZERO(&set);
	FD_SET(sockfd,&set);
	FD_SET(maxfd,&set);
	maxfd=(maxfd>sockfd?maxfd:sockfd)+1;

	while(1)
	{
		
		rset=set;
		if((nfound=select(maxfd,&rset,(fd_set*)0,(fd_set*)0,NULL))<0)
		{
			if(errno==EINTR)
			{
				fprintf(stderr,"interrupted system call\n");
				continue;
			}
			perror("select");
			exit(1);
		}
		
		if(FD_ISSET(fileno(stdin),&rset))
		{
			
			if(fgets(buffer,sizeof(buffer)-1,stdin)==NULL)
			{
				if(ferror(stdin))
				{
					perror("stdin");
					return -1;
				}
				return 0;
			}
			if(write(sockfd,buffer,strlen(buffer))<0)
			{	
				perror("ERROR writing");
				return -1;
			}
		}
		if(FD_ISSET(sockfd,&rset))
		{
			if((bytesread=read(sockfd,buffer,sizeof(buffer)))<0)
			{	
				perror("ERROR read");
				exit(1);
			}else if(bytesread==0)
			{
				fprintf(stderr,"server disconnect\n");
				exit(0);
			}
				
		buffer[bytesread]=0;//最后加上结束符
		printf("buffer: %s",buffer);
		}
	
	}

	return 0;

}
