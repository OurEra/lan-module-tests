#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/select.h>

/*try to read count bytes from fd file*/
ssize_t readn(int fd, void *buf, size_t count)
{
  char *strtmp = NULL;
  ssize_t reval = 0;
  ssize_t realcount = 0;
  strtmp = (char *) buf;
  while (count > 0) {
    reval = read(fd, strtmp, count);
    if (reval < 0) {
      if (errno == EINTR) {
	continue;
      } else {
	return -1;
      }
    } else if (reval > 0) {
      count -= reval;
      strtmp += reval;
      realcount += reval;
      continue;
    } else {
      break;
    }
  }
  return realcount;
}

/*try ro write count bytes from fd file*/
ssize_t writen(int fd, const void *buf, size_t count)
{
  char *strtmp = NULL;
  ssize_t reval = 0;
  ssize_t realcount = 0;
  strtmp = (char *) buf;
  while (count > 0) {
    reval = write(fd, strtmp, count);
    if (reval < 0) {
      if (errno == EINTR) {
	continue;
      } else {
	return -1;
      }
    } else if (reval > 0) {
      count -= reval;
      strtmp += reval;
      realcount += reval;
      continue;
    } else {
      break;
    }
  }
  return realcount;
}

/*read line message*/
ssize_t readline(int fd, void *buf, int size)
{
  char *strtmp = NULL;
  ssize_t reval;
  ssize_t realcount = 0;
  strtmp = (char *) buf;
  while (size > 1) {
    reval = read(fd, strtmp, 1);
    if (reval < 0) {
      if (errno == EINTR) {
	continue;
      } else {
	return -1;
      }
    } else if (reval == 0) {
      break;
    } else {
      realcount++;
      size--;
      if (*strtmp++ == '\n') {
	break;
      }
    }
  }
  *strtmp = '\0';
  return realcount;
}
      
int main(int argc, char **argv)
{
  struct sockaddr_in severaddr;
  fd_set read_fd_set;
  char rcv_buf[100];
  char stdin_buf[100];

  if (argc < 2) {
    printf("usage:need argument ip address\n");
    exit(-1);
  }

  /*fill up ip address structure */
  severaddr.sin_family = AF_INET;
  severaddr.sin_port = htons(5005);
  int ret = inet_pton(AF_INET, argv[1],
		      &(severaddr.sin_addr.s_addr));
  if (ret < 0) {
    perror("inet_pton err");
    return -1;
  }

  /*setup socket */
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) {
    perror("socket err");
    return -1;
  }

  /*connect to server with 3 handshake */
  ret = connect(socketfd, (const struct sockaddr *) &severaddr,
		sizeof(struct sockaddr_in));
  if (ret < 0) {
    perror("connect err");
    return -1;
  }

  while (1) {
    /*initialize file descriptable set */
    FD_ZERO(&read_fd_set);
    FD_SET(0, &read_fd_set);
    FD_SET(socketfd, &read_fd_set);

    /*select from file descriptable set */
    ret = select(socketfd + 1, &read_fd_set, NULL, NULL, NULL);
    if (ret < 0) {
      perror("select err");
      return -1;
    }

    /*check standard input if ready or no ? */
    if (FD_ISSET(0, &read_fd_set)) {
      /*read from standard input and write to socket */
      memset(stdin_buf, 0, sizeof(stdin_buf));
      ret = readline(0, stdin_buf, sizeof(stdin_buf));
      if (ret > 0) {
	writen(socketfd, stdin_buf, ret);
      } else if (ret == 0) {
	close(socketfd);
	break;
      } else {
	perror("read 0 err");
	break;
      }
    }

    /*check socket if ready or no? */
    if (FD_ISSET(socketfd, &read_fd_set)) {
      /*read from socket */
      memset(rcv_buf, 0, sizeof(rcv_buf));
      ret = readline(socketfd, rcv_buf, sizeof(rcv_buf));
      if (ret > 0) {
	printf("rcv_buf = %s\n", rcv_buf);
      } else if (ret == 0) {
	printf("server end\n");
	close(socketfd);
	break;
      } else {
	perror("read socketfd err");
	break;
      }
    }
  }
  return 0;
}
