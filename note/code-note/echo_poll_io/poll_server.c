#include<sys/types.h>
#include<ctype.h>
#include<strings.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<ctype.h>
#include<errno.h>
#include<sys/time.h>
#include<stdio.h>
#include<string.h>
#include<sys/poll.h>
#include<stdlib.h>

#define LISTEN_QUEUE_NUM 5
#define MAX_CONNECT 1024

#define BUFFER_SIZE 256
#define ECHO_PORT 2029

static struct pollfd clipoll[MAX_CONNECT];
int max = 0;

/*initialize poll struct*/
static void poll_init()
{
	int i;
	for (i = 0; i < MAX_CONNECT; i++)
		clipoll[i].fd = -1;
}

/*
 find a unused pollfd struct
 if found then return its index
 else return -1
*/ 
static int poll_alloc()
{
	int i;
	for (i = 0; i < MAX_CONNECT; i++)
	{
		if (clipoll[i].fd < 0)
		return i;
	}
	return -1;
}

/*find used max index in pllfd*/
static void update_max()
{
	int i;
	max = 0;
	for (i = 0; i < MAX_CONNECT; i++)
	{
		if (clipoll[i].fd > 0)
			max = i;
	}
}

/*free a pollfd when unused*/
static int poll_free(int i)
{
	close(clipoll[i].fd);
	clipoll[i].fd = -1;
	clipoll[i].events = 0;
	clipoll[i].revents = 0;
	update_max();
}

/*
  fill up pollfd with file descriptable
  and event
*/
static int poll_set_item(int fd, uint32_t events)
{
	int i;
	if ((i = poll_alloc()) < 0)
		return -1;
	clipoll[i].fd = fd;
	clipoll[i].events = events;
	clipoll[i].revents = 0;
	if (i > max)
		max = i;
	return 0;
}

int main(int argc, char **argv)
{
	struct sockaddr_in servaddr, remote;
	int request_sock, new_sock;
	int nfound, i, bytesread;
	uint32_t addrlen;
	char buf[BUFFER_SIZE];
	/*setup socket*/
	if ((request_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}
	/*fill up ip address structure*/
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(ECHO_PORT);
	/*bind server ip address structure*/
	if (bind(request_sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) <	0)
	{
		perror("bind");
		return -1;
	}
	/*listen client*/
	if (listen(request_sock, LISTEN_QUEUE_NUM) < 0)
	{
		perror("listen");
		return -1;
	}
	poll_init();
	poll_set_item(request_sock, POLLIN);
	while (1)
	{
	/*check whether ready file exist ?*/
		if ((nfound = poll(clipoll, max + 1, 500)) < 0)
		{
			if (errno == EINTR)
			{
				printf("interruptedb system call\n");
				continue;
			}
			perror("poll");
			return -1;
		}else if(nfound == 0)
		{
			printf(".");
			fflush(stdout);
			continue;
		}
		/*check listen socket if ready or not ?*/
		if (clipoll[0].revents & (POLLIN | POLLERR))
		{
			addrlen = sizeof(remote);
			if ((new_sock =	accept(request_sock, (struct sockaddr *) &remote,	&addrlen)) < 0)
			{
				perror("accept");
				return -1;
			}
			printf("connection fromm host %s,port %d, socket %d\r\n",
					inet_ntoa(remote.sin_addr), ntohs(remote.sin_port), new_sock);
			if (poll_set_item(new_sock, POLLIN) < 0)
				fprintf(stderr, "Too many connects\r\n");
			nfound--;
		}
		/*check communicating socket if ready or not ? */
		for (i = 1; i <= max && nfound > 0; i++)
		{
			if (clipoll[i].fd >= 0 && clipoll[i].revents & (POLLIN | POLLERR))
			{
				nfound--;
				if((bytesread = read(clipoll[i].fd, buf, sizeof(buf) - 1)) < 0)
				{
					perror("read");
					poll_free(i);
					continue;
				}
				if(bytesread == 0)
				{
					fprintf(stderr, "server: end of file on %d\r\n", clipoll[i].fd);
					poll_free(i);
					continue;
				}
				buf[bytesread] = 0;
				printf("%s:%d bytes from %d :%s\n", argv[0], bytesread, clipoll[i].fd, buf);

				if (write(clipoll[i].fd, buf, bytesread) != bytesread)
				{
					perror("echo");
					poll_free(i);
					continue;
				}
			}
		}
	}
	return 0;
}

