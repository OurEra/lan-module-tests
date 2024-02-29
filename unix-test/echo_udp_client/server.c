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

int main(int argc, char **argv)
{
  int sockfd, opt = 1;
  uint32_t len;
  struct sockaddr_in server_addr, client_addr;
  uint8_t buffer[BUFFER_SIZE];
  int ret = RET_OK;

  /*setup socket */
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("ERROR opening socket");
    return RET_ERR;
  }

  /*set network options */
  if ((ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
			&opt, sizeof(opt))) < 0) {
    perror("ERROR setsockpot");
    goto failed;
  }

  /*fill up server ip address structure */
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(ECHO_PORT);

  /*bind server address */
  if ((ret = bind(sockfd, (struct sockaddr *) &server_addr,
		  sizeof(server_addr))) < 0) {
    perror("ERROR on binding");
    goto failed;
  }

  /* communicating with client */
  do {
    len=sizeof(client_addr);
    if ((ret = recvfrom(sockfd, buffer, sizeof(buffer), 0,
			(struct sockaddr *) &client_addr,
			&len)) > 0) {
      printf("Recv from %s\r\n", inet_ntoa(client_addr.sin_addr));
      ret = sendto(sockfd, buffer, ret, 0,
		   (struct sockaddr *) &client_addr, sizeof(client_addr));
    }
  } while (ret >= 0);

failed:close(sockfd);
  return 0;
}
