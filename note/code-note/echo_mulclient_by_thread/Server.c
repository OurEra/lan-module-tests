#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>

/*try to read count bytes from fd*/
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

/*try to write count bytes to fd file*/
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

/*read line test from fd*/
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

/*thread function*/
void *thread_fun(void *arg)
{
  int conn_sock = *((int *) arg);
  char rec_buf[50];
  int ret = -1;
  free(arg);
  while (1) {
    memset(rec_buf, 0, sizeof(rec_buf));
    ret = readline(conn_sock, rec_buf, sizeof(rec_buf));
    if (ret > 0) {
      //sleep(1);
      writen(conn_sock, rec_buf, ret);
    } else if (ret == 0) {
      printf("clent end\n");
      close(conn_sock);
      break;
    } else {
      perror("read err");
    }
  }
  return NULL;
}

/*server main function*/
int main()
{
  int ret = -1;
  uint32_t ip = 0;
  char ip_str[30];
  struct sockaddr_in clientaddr;
  socklen_t clientaddr_len = 0;
  struct sockaddr_in severaddr;
  char rec_buf[50];
  pthread_t tid;
  int *pconnsocke = NULL;
  int i = 0;
  struct hostent *phost = NULL;

  /*initialize ip address structure */
  severaddr.sin_family = AF_INET;
  severaddr.sin_port = htons(5005);
  severaddr.sin_addr.s_addr = INADDR_ANY;

  /*setup socket */
  int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_sock < 0) {
    perror("socket err");
    return -1;
  }

  /*bind server ip address structure */
  ret = bind(listen_sock, (struct sockaddr *) &severaddr,
	     sizeof(struct sockaddr_in));
  if (ret < 0) {
    perror("bind err");
    return -1;
  }

  /*listen clients */
  ret = listen(listen_sock, 5);
  if (ret < 0) {
    perror("listen err");
    return -1;
  }

  while (1) {
    /*accept client */
    clientaddr_len = sizeof(struct sockaddr_in);
    int conn_sock = accept(listen_sock,
			   (struct sockaddr *) &clientaddr,
			   &clientaddr_len);
    if (conn_sock < 0) {
      perror("accept err");
      return -1;
    }

    /*create thread process client request */
    pconnsocke = (int *) malloc(sizeof(int));
    *pconnsocke = conn_sock;
    ret = pthread_create(&tid, NULL, thread_fun, (void *) pconnsocke);
    if (ret < 0) {
      perror("pthread_create err");
      return -1;
    }
  }

  /*close listen socket */
  close(listen_sock);
  return 0;
}
