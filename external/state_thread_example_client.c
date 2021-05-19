#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define RET_OK 0
#define RET_ERR -1

#define LISTEN_QUEUE_NUM 5

#define BUFFER_SIZE 256

#define ECHO_PORT 8556

int main(int argc,char **argv)
{
	int sockfd, ret = RET_OK;
	struct sockaddr_in servaddr;
	struct hostent *server;

	char buffer[BUFFER_SIZE];

	if(argc < 2) {
		fprintf(stderr,"usage %s hostname\n", argv[0]);
		return RET_ERR;
	}
	if((server = gethostbyname(argv[1])) == NULL) {
		herror("gethostbyname.");
		return RET_ERR;
	}

	if((sockfd = socket(AF_INET,SOCK_STREAM, 0)) < 0) {
		perror("ERROR opening socket");
		return RET_ERR;
	}

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = *(uint32_t *)server->h_addr;
	servaddr.sin_port = htons((uint16_t)ECHO_PORT);

	if((ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0) {
		perror("ERROR connecting");
		goto failed;
	}

	while(1) {
		printf("Enter the message : ");
		if(fgets(buffer, sizeof(buffer)-1, stdin) == NULL) {
			break;
		}
		if((ret = write(sockfd, buffer, strlen(buffer))) < 0) {
			perror("ERROR write to socket");
			break;
		}
		if((ret = read(sockfd, buffer, sizeof(buffer)-1)) < 0) {
			perror("ERROR read from socket");
			break;
		}
		if(ret == 0) {
			printf("Server disconnect\n");
			break;
		}
		buffer[ret] = 0;
		printf("Server echo message: %s\n", buffer);
	}
	failed:
		close(sockfd);
		return ret < 0 ? RET_ERR:RET_OK;
}


