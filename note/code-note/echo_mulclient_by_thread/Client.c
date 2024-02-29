#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

/*
  try to read count bytes from fd
  return real read count
*/
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

/*
try to write count bytes to fd
return real write count
*/
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

/*read line message from fd*/
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

/*thread function:write message to socket*/
void *thread_read_stdin(void *arg)
{
  char stdin_buf[128];
  int socketfd = -1;
  int ret = -1;
  int sum = 0;
  socketfd = *((int *) arg);
  free(arg);
  while (1) {
    sum++;
    /*format text */
    sprintf(stdin_buf, "sum = %d\n", sum);
    printf("stdin_buf is:%s\n", stdin_buf);
    /*write to socket */
    ret = writen(socketfd, stdin_buf, strlen(stdin_buf));
    if (ret < 0) {
      perror("write error");
      return NULL;
    }
  }
}

/*thread function:read message from socket*/
void *thread_read_server(void *arg)
{
  int socketfd = -1;
  char rcv_buf[128];
  int ret = -1;
  socketfd = *((int *) arg);
  free(arg);
  while (1) {
    memset(rcv_buf, 0, sizeof(rcv_buf));
    ret = readline(socketfd, rcv_buf, sizeof(rcv_buf));
    if (ret > 0) {
      printf("rcv_buf is:%s\n", rcv_buf);
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

int main(int argc, char **argv)
{
  int ret = -1;
  int i = 0;
  int socketfd = -1;
  int *psocket = NULL;
  struct sockaddr_in severaddr;

  if (argc < 2) {
    printf("usage:need ip address\n");
    exit(-1);
  }

  /*initialize ip address structure */
  severaddr.sin_family = AF_INET;
  severaddr.sin_port = htons(5005);
  ret = inet_pton(AF_INET, argv[1], &(severaddr.sin_addr.s_addr));
  if (ret < 0) {
    perror("inet_pton err");
    return -1;
  }

  /*setup socket */
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) {
    perror("socket err");
    return -1;
  }

  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  printf("11 %ld sec %ld ms\n", ts.tv_sec, ts.tv_nsec / 1000 / 1000);
  /*connect to server */
  ret = connect(socketfd, (const struct sockaddr *) &severaddr,
		sizeof(struct sockaddr_in));
  if (ret < 0) {
    perror("connect err");
    return -1;
  }
  struct timespec ts1;
  clock_gettime(CLOCK_MONOTONIC, &ts1);
  printf("22 %ld sec %ld ms\n", ts1.tv_sec, ts1.tv_nsec / 1000 / 1000);

  /*create 2 thread seperately */
  pthread_t tid[2];
  void *(*thread_fun_array[2]) (void *) = {
  thread_read_stdin, thread_read_server};
  for (i = 0; i < 2; i++) {
    psocket = malloc(sizeof(int));
    *psocket = socketfd;
    ret = pthread_create(&tid[i], NULL, thread_fun_array[i], psocket);
    if (ret < 0) {
      perror("pthread_create err");
      return -1;
    }
  }

  /*wait two threads */
  ret = pthread_join(tid[0], NULL);
  if (ret < 0) {
    perror("pthread_join err");
    return -1;
  }
  ret = pthread_join(tid[1], NULL);
  if (ret < 0) {
    perror("pthread_join err");
    return -1;
  }

  return 0;
}
