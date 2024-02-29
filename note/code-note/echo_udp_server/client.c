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

int main(int argc, char *argv[])
{
  int sockfd, ret = RET_OK;
  struct sockaddr_in servaddr, recvaddr;
  struct hostent *server;
  char buffer[BUFFER_SIZE];

  if (argc < 2) {
    fprintf(stderr, "usage %s hostname\n", argv[0]);
    return RET_ERR;
  }

  if ((server = gethostbyname(argv[1])) == NULL) {
    perror("gethostbyname. ");
    return RET_ERR;
  }

  /*set up socket */
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("ERROR opening ocket");
    return RET_ERR;
  }

  /*fill up server ip address structure */
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = *(uint32_t *) server->h_addr;
  servaddr.sin_port = htons((uint16_t) ECHO_PORT);

  while (1) {
    printf("Enter the messagen : ");

    /*get a line string then send to server */
    if (fgets(buffer, sizeof(buffer) - 1, stdin) == NULL)
      break;
    if ((ret = sendto(sockfd, buffer, strlen(buffer), 0,
		      (struct sockaddr *) &servaddr,
		      sizeof(servaddr))) < 0) {
      perror("ERROR writing to socket");
      break;

    }
    uint32_t len = sizeof(recvaddr);
  retry:

    /*receive message from server */
    if ((ret = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
			(struct sockaddr *) &recvaddr, &len)) < 0) {
      printf("ERROR reading from socket");
      break;
    }

    /*check if data come from server or not ? */
    if (servaddr.sin_family != recvaddr.sin_family
	|| servaddr.sin_addr.s_addr != recvaddr.sin_addr.s_addr
	|| servaddr.sin_port != recvaddr.sin_port) {
      fprintf(stderr, "Received %s echo\r\n",
	      inet_ntoa(recvaddr.sin_addr));
      goto retry;
    }

    buffer[ret] = 0;
    printf("Server echo message:%s\n", buffer);
  }
  close(sockfd);
  return ret < 0 ? RET_ERR : RET_OK;
}