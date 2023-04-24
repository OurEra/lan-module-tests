#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#include "play_config.h"
#include "array-heap.h"
#include "network.h"
#include "kcp_wrap.h"
#include "ev_wrap.h"

struct sock_ev_serv {
  ev_io io;
  int fd;
  struct sockaddr_un socket;
  int socket_len;
  array clients;
};

struct sock_ev_client {
  ev_io io;
  int fd;
  int kcpid;
  int index;
  struct sock_ev_serv* server;
};

static void good2write(const char *buf, int len, void* user) {
  struct sock_ev_client* client = (struct sock_ev_client*) user;
  int ret = send(client->fd, buf, len, 0 /* flags */);
}

static void good2read(const char *buf, int len, void* user) {
  struct sock_ev_client* client = (struct sock_ev_client*) user;
  printf("read buf [%s]\n", buf, client->fd);
}

// This callback is called when client data is available
static void client_io_handler(EV_P, ev_io* w, int revents) {
  struct sock_ev_client* client = (struct sock_ev_client*)w;
  kcp_wrap_process(client->kcpid);
  if (revents & EV_READ) {
    int n;
    char str[1000];
    n = recv(client->fd, str, 1000, 0);
    if (n <= 0) {
      if (0 == n) {
        // an orderly disconnect
        puts("orderly disconnect");
        ev_io_stop(EV_A, &client->io);
        close(client->fd);
      } else if (EAGAIN == errno) {
        puts("should never get in this state with libev");
      } else {
        perror("recv");
      }
      return;
    }
    kcp_wrap_prepare_for_read(str, n, client->kcpid);
  } else if (revents & EV_WRITE) {
    // TODO
  }
}

inline static struct sock_ev_client* client_new(int fd) {
  struct sock_ev_client* client;

  client = realloc(NULL, sizeof(struct sock_ev_client));
  client->fd = fd;
  net_setnonblock(client->fd);

  client->kcpid = kcp_wrap_new(good2write, good2read, client);
  ev_io_init(&client->io, client_io_handler, client->fd, EV_READ | EV_WRITE);

  return client;
}

// This callback is called when data is readable on the unix socket.
static void server_io_handler(EV_P_ ev_io* w, int revents) {
  puts("unix stream socket has become readable");

  int client_fd;
  struct sock_ev_client* client;

  // since ev_io is the first member,
  // watcher `w` has the address of the
  // start of the sock_ev_serv struct
  struct sock_ev_serv* server = (struct sock_ev_serv*)w;

  while (1) {
    client_fd = accept(server->fd, NULL, NULL);
    if (client_fd == -1) {
      if (errno != EAGAIN && errno != EWOULDBLOCK) {
        printf("accept() failed errno=%i (%s)", errno, strerror(errno));
        exit(EXIT_FAILURE);
      }
      break;
    }
    puts("accepted a client");
    client = client_new(client_fd);
    client->server = server;
    client->index = array_push(&server->clients, client);
    ev_io_start(EV_A, &client->io);
  }
}

int unix_socket_init(struct sockaddr_un* socket_un, char* sock_path) {
  int fd;
  unlink(sock_path);

  // Setup a unix socket listener.
  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (-1 == fd) {
    perror("echo server socket");
    exit(EXIT_FAILURE);
  }

  // Set it non-blocking
  if (-1 == net_setnonblock(fd)) {
    perror("echo server socket nonblock");
    exit(EXIT_FAILURE);
  }

  // Set it as unix socket
  socket_un->sun_family = AF_UNIX;
  strcpy(socket_un->sun_path, sock_path);

  return fd;
}

int server_init(struct sock_ev_serv* server, char* sock_path) {
  server->fd = unix_socket_init(&server->socket, sock_path);
  server->socket_len =
      sizeof(server->socket.sun_family) + strlen(server->socket.sun_path);

  array_init(&server->clients, 100);

  if (-1 ==
      bind(server->fd, (struct sockaddr*)&server->socket, server->socket_len)) {
    perror("echo server bind");
    exit(EXIT_FAILURE);
  }

  if (-1 == listen(server->fd, 100)) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  return 0;
}

void client_free_handler(void* msg) {
  struct sock_ev_client * client = (struct sock_ev_client*) msg;
  kcp_wrap_release(client->kcpid);
}

int main(void) {
  struct sock_ev_serv server;

  wrapev_init();
  // Create unix socket in non-blocking fashion
  server_init(&server, "/tmp/libev-echo.sock");

  // Get notified whenever the socket is ready to read
  ev_io_init(&server.io, server_io_handler, server.fd, EV_READ);
  ev_io_start(EV_A_ & server.io);

  // Run our loop, ostensibly forever
  ev_loop(EV_A, 0);

  wrapev_deinit();
  array_free(&server.clients, client_free_handler);
  // This point is only ever reached if the loop is manually exited
  close(server.fd);
  return EXIT_SUCCESS;
}
