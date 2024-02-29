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
	int sockfd;
	int ret=RET_OK;
	struct sockaddr_in servaddr,recvaddr;
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
	
	if((sockfd=socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP))<0)
	{
		herror("ERROR opening socket");
		return RET_ERR;
	}

	memset(&servaddr,0,sizeof(servaddr));//申请信息表空间

	servaddr.sin_family=AF_INET;//设为IPv4
	servaddr.sin_addr.s_addr=*(uint32_t *)server->h_addr;//地址转换,32位大端字序
	servaddr.sin_port=htons((uint16_t)ECHO_PORT);//设定端口,16位大端字序转换

	while(1)
	{
		printf("Enter the message : ");
		if(fgets(buffer,sizeof(buffer)-1,stdin)==NULL)break;
		if((ret=sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&servaddr,sizeof(servaddr)))<0)
		{
			perror("ERROR writing to socket");
			break;
		}
		uint32_t len=sizeof(recvaddr);
		
		retry:
		if((ret=recvfrom(sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&recvaddr,&len))<0)
		{
			perror("ERROR reading from socket");
			break;
		}
		if(servaddr.sin_family!=recvaddr.sin_family||servaddr.sin_addr.s_addr!=recvaddr.sin_addr.s_addr||servaddr.sin_port!=recvaddr.sin_port)
		{
			fprintf(stderr,"Received %s echo\r\n",inet_ntoa(recvaddr.sin_addr));
			goto retry;
		}
		buffer[ret]=0;
		printf("Server echo message: %s\n",buffer);
	
	}
	close(sockfd);
	return ret<0?RET_ERR:RET_OK;

}
