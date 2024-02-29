/*
ECHO广播式的UDP客户端
服务器接收端没有区别,和P273_6-5的是一样的
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
#include <pthread.h>

#define RET_OK 0
#define RET_ERR -1

#define LISTEN_QUEUE_NUM 5

#define BUFFER_SIZE 256

#define ECHO_PORT 2029

int sockfd;
struct sockaddr_in servaddr,recvaddr;
pthread_t t1;

void rev()
{
	int ret;
	char buffer[BUFFER_SIZE];
	uint32_t len=sizeof(recvaddr);
	while(1)
	{
		if((ret=recvfrom(sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&recvaddr,&len))<0)
		{
			perror("ERROR reading from socket");
			break;
		}
		buffer[ret]=0;
		printf("[form:%s:%u]%s\n",inet_ntoa(recvaddr.sin_addr),ntohs(recvaddr.sin_port), buffer);

		//printf("Server echo message: %s\n",buffer);

	}
}
int main(int argc,char **argv)
{
	int opt=1;
	int ret=RET_OK;
	char buffer[BUFFER_SIZE];
	struct hostent *server;

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
	
	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0)
	{
		herror("ERROR opening socket");
		return RET_ERR;
	}
#if 0
	//广播设置套接字
	if((setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt)))<0)
	{
		perror("ERROR setsockopt");
		goto failed;
	}
#endif

	memset(&servaddr,0,sizeof(servaddr));//申请信息表空间

	servaddr.sin_family=AF_INET;//设为IPv4
	servaddr.sin_addr.s_addr=*(uint32_t *)server->h_addr;//地址转换,32位大端字序
	servaddr.sin_port=htons((uint16_t)ECHO_PORT);//设定端口,16位大端字序转换

	if((ret=connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)))<0) //创建连接
	{
		perror("ERROR connecting");
		goto failed;
	}

	pthread_create(&t1, NULL, (void*)rev, NULL);

	while(1)
	{
		printf("Enter the message : ");
		if(fgets(buffer,sizeof(buffer)-1,stdin)==NULL)break;
		if((ret=sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&servaddr,sizeof(servaddr)))<0)
		{
			perror("ERROR writing to socket");
			break;
		}
		
#if 0
		retry:
		if((ret=recvfrom(sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&recvaddr,&len))<0)
		{
			perror("ERROR reading from socket");
			break;
		}

		/*  单播时的代码段 */
		if(servaddr.sin_family!=recvaddr.sin_family||servaddr.sin_addr.s_addr!=recvaddr.sin_addr.s_addr||servaddr.sin_port!=recvaddr.sin_port)
		{
			fprintf(stderr,"Received %s echo\r\n",inet_ntoa(recvaddr.sin_addr));
			goto retry;
		}
#endif

	}
	failed:
	close(sockfd);
	return ret<0?RET_ERR:RET_OK;

}
