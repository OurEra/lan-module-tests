#include<sys/types.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<errno.h>
#include<ctype.h>
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/select.h>
#include<sys/poll.h>

#define RET_OK 0
#define RET_ERR -1
#define LISTEN_QUEUE_NUM 5

#define BUFFER_SIZE 256
#define ECHO_PORT   2029

int main(int argc, char **argv)
{
	int sock;
	struct sockaddr_in servaddr;
	struct hostent *server;
	static struct pollfd cpoll[2] = {0};
	int nfound, bytesread;
	char buf[BUFFER_SIZE];

	if(argc < 2)
	{
		fprintf(stderr, "usage %s hostname\n", argv[0]);
		return RET_ERR;
	}
	if((server = gethostbyname(argv[1])) == NULL)
	{
		perror("gethostbyname. ");
		return RET_ERR;
	}

	/*setup socket*/
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}

	/*fillup ip address structure*/
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = *(uint32_t *) server->h_addr;
	servaddr.sin_port = htons((uint16_t) ECHO_PORT);

	/*connect to server*/
	if(connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect");
		return -1;
	}

	/*fill up poll struct*/
	cpoll[0].fd = fileno(stdin);
	cpoll[0].events = POLLIN;
	cpoll[1].fd = sock;
	cpoll[1].events = POLLIN;

	while (1)
	{
		if((nfound = poll(cpoll, 2, -1)) < 0)
		{
			if (errno == EINTR)
			{
				fprintf(stderr, "interruptedb system call\n");
				continue;
			}
			perror("select");
			exit(1);
		}
		/*check stdin is ready or not ? */
		if(cpoll[0].revents & (POLLIN | POLLERR))
		{
			if(fgets(buf, sizeof(buf), stdin) == NULL)
			{
				if (ferror(stdin))
				{
					perror("stdin");
					return -1;
				}
				return 0;
			}
			/*write to socket*/
			if (write(sock, buf, strlen(buf)) < 0)
			{
				perror("write");
				return -1;
			}
		}

		/*check socket is ready or not ? */
		if(cpoll[1].revents & (POLLIN | POLLERR))
		{
			/*read from socket*/
			if((bytesread = read(sock, buf, sizeof(buf))) < 0)
			{
				perror("read");
				exit(1);
			}else if(bytesread == 0)
			{
				fprintf(stderr, "server disconnect\n");
				exit(0);
			}
			buf[bytesread] = 0;
			printf("%s\n", buf);
		}
	}
	return 0;
}

