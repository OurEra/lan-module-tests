#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/select.h>
#include <netdb.h>

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

int my_select(int nfds, fd_set * readfds, fd_set * writefds,
	      fd_set * exceptfds, struct timeval *timeout)
{
  int ret = -1;
  while (1) {
    ret = select(nfds, readfds, writefds, exceptfds, timeout);
    if (ret < 0 && errno == EINTR) {
      continue;
    } else {
      break;
    }
  }
  return ret;
}

int main()
{
  uint32_t ip = 0;
  char ip_str[30];
  struct sockaddr_in clientaddr;
  socklen_t clientaddr_len = 0;
  struct sockaddr_in severaddr;
  char rec_buf[50];
  int client_sock_array[100];
  int client_size = 0;
  int maxfd = -1;
  fd_set read_fd_set;
  int i = 0;

  /*fill up ip address structure*/
  severaddr.sin_family = AF_INET;
  severaddr.sin_port = htons(5005);
  severaddr.sin_addr.s_addr = INADDR_ANY;

  /*setup socket*/
  int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_sock < 0) {
    perror("socket err");
    return -1;
  }

  /*bind server ip address structure*/
  int ret = bind(listen_sock,(struct sockaddr *) &severaddr, 
                 sizeof(struct sockaddr_in));
  if (ret < 0) {
    perror("bind err");
    return -1;
  }

  /*listen clients*/
  ret = listen(listen_sock, 5);
  if (ret < 0) {
    perror("listen err");
    return -1;
  }
  for (i = 0; i < 100; i++) {
    client_sock_array[i] = -1;
  }

  while (1) {

    /*set up file descriptable set again*/
    FD_ZERO(&read_fd_set);
    FD_SET(listen_sock, &read_fd_set);
    maxfd = listen_sock;
    client_size = 0;
    for (i = 0; client_sock_array[i] >= 0 && i < 100; i++) {
      FD_SET(client_sock_array[i], &read_fd_set);
      if (client_sock_array[i] > maxfd) {
	maxfd = client_sock_array[i];
      }
      client_size++;
    }

    /*
      select from file descriptable set
      if no file is ready then block
      else return
    */
    ret = my_select(maxfd + 1, &read_fd_set, NULL, NULL, NULL);
    if (ret < 0) {
      perror("select err");
      return -1;
    }

    /*check listen socket if ready or not*/
    if (FD_ISSET(listen_sock, &read_fd_set)) {
      clientaddr_len = sizeof(struct sockaddr_in);
      int conn_sock = accept(listen_sock,
			     (struct sockaddr *) &clientaddr, 
                             &clientaddr_len);
      if (conn_sock < 0) {
	perror("accept err");
	return -1;
      }
      for (i = 0; i < 100; i++) {
	if (client_sock_array[i] < 0) {
	  client_sock_array[i] = conn_sock;
	  break;
	}
      }
    }

    /*check all communicating sockets if ready or not*/
    for (i = 0; client_sock_array[i] >= 0 && i < 100; i++) {
      if (FD_ISSET(client_sock_array[i], &read_fd_set)) {
	memset(rec_buf, 0, sizeof(rec_buf));
	ret = readline(client_sock_array[i], rec_buf, sizeof(rec_buf));
	if (ret > 0) {
	  writen(client_sock_array[i], rec_buf, ret);
	} else if (ret == 0) {
	  printf("clent end\n");
	  close(client_sock_array[i]);
	  client_sock_array[i] = client_sock_array[client_size - 1];
	  client_sock_array[client_size - 1] = -1;
	  i--;
	} else {
	  perror("read err");
	}
      }
    }
  }
  close(listen_sock);
  return 0;
}
