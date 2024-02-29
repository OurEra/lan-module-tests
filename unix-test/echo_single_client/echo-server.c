/*echo server*/
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

/*read a line message from fd*/
static int my_readline(int fd, void *buf, unsigned len)
{
  int n, rc;
  char c, *ptr;
  ptr = buf;
  for (n = 1; n < len; n++) {
  again:
    if ((rc = read(fd, &c, 1)) == 1) {
      *ptr++ = c;
      if (c == '\n')
	break;
    } else if (rc == 0) {
      if (n == 1)
	return (0);
      else
	break;
    } else {
      if (errno == EINTR)
	goto again;
      return (-1);
    }
  }
  *ptr = 0;
  return (n);
}

/*after read a line message then write to sockfd*/
static int proc_echo(int sockfd)
{
  int ret;
  char buf[BUFFER_SIZE];
  while (1) {
    if ((ret = my_readline(sockfd, buf, sizeof(buf) - 1)) < 0) {
      perror("read");
      return -1;
    } else if (ret == 0) {
      printf("client disconnect.\n");
      return 0;
    }
    buf[ret] = 0;
    if ((ret = write(sockfd, buf, strlen(buf))) < 0) {
      perror("write");
      return -1;
    }
  }
  return -1;
}

int main(int argc, char **argv)
{
  int sockfd, nsock, ret = 0;
  uint32_t len;
  struct sockaddr_in servaddr, cliaddr;

  /*setup socket */
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("ERROR opening socket");
    return RET_ERR;
  }

  /*initialize server ip address structure */
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons((uint16_t) ECHO_PORT);

  /*bind server ip address structure */
  if ((ret = bind(sockfd, (struct sockaddr *) &servaddr,
		  sizeof(servaddr))) < 0) {
    perror("ERROR on binding");
    goto failed;
  }

  /*listen client */
  if ((ret = listen(sockfd, LISTEN_QUEUE_NUM)) != 0) {
    perror("ERROR on listen");
    goto failed;
  }

  while (1) {
    len = sizeof(cliaddr);
    /*
       accept client:
       if no client then server blocking;
       else unblock server and get client ip 
       address structure to cliaddr structure
     */
    if ((nsock = accept(sockfd, (struct sockaddr *) &cliaddr,
			(uint32_t *) & len)) < 0) {
      perror("accept");
      break;
    }
    /*process client request */
    proc_echo(nsock);
    close(nsock);
  }
failed:

  /*close socket */
  close(sockfd);
  return ret < 0 ? RET_ERR : RET_OK;
}
