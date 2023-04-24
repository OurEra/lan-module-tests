#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>  // fcntl
#include <sys/socket.h>
#include <sys/un.h>

#include "play_config.h"
#include "kcp_wrap.h"
#include "network.h"
#include "ev_wrap.h"

ev_io stdin_watcher;
ev_io remote_w;
ev_io send_w;

typedef struct client_context {
  int remote_fd;
  int kcpid;
  char *stdLINE;
} client_context_t;

static client_context_t clic;

static void good2write(const char *buf, int len, void* user) {
  client_context_t* cct = (client_context_t*) user;
  send(cct->remote_fd, buf, len, 0 /* flags */);
}

static void good2read(const char *buf, int len, void* user) {
  printf("read buf %s\n", buf);
}

static void kcp_check_handler(EV_P_ ev_periodic *w, int revents) {
  kcp_wrap_process(clic.kcpid);
}

static void send_cb(EV_P_ ev_io *w, int revents) {
  if (revents & EV_WRITE) {
    kcp_wrap_prepare_for_write(clic.stdLINE, strlen(clic.stdLINE) + 1, clic.kcpid);
    // once the data is sent, stop notifications that
    // data can be sent until there is actually more
    // data to send
    ev_io_stop(EV_A, &send_w);
    ev_io_set(&send_w, clic.remote_fd, EV_READ);
    ev_io_start(EV_A, &send_w);
  } else if (revents & EV_READ) {
    int n;
    char str[100];
    n = recv(clic.remote_fd, str, 100, 0);
    if (n <= 0) {
      if (0 == n) {
        // an orderly disconnect
        puts("orderly disconnect");
        ev_io_stop(EV_A, &send_w);
        close(clic.remote_fd);
      } else if (EAGAIN == errno) {
        puts("should never get in this state with libev");
      } else {
        perror("recv");
      }
      return;
    }
    kcp_wrap_prepare_for_read(str, n, clic.kcpid);
  }
}

static void stdin_cb(EV_P_ ev_io *w, int revents) {
  size_t len;
  puts("stdin written to, reading...");
  getline(&clic.stdLINE, &len, stdin);
  ev_io_stop(EV_A, &send_w);
  ev_io_set(&send_w, clic.remote_fd, EV_READ | EV_WRITE);
  ev_io_start(EV_A, &send_w);
}

static void remote_cb(EV_P_ ev_io *w, int revents) {
  puts("connected, now watching stdin");
  // Once the connection is established, listen to stdin
  ev_io_start(EV_A, &stdin_watcher);
  // Once we're connected, that's the end of that
  ev_io_stop(EV_A, &remote_w);
}

static void connection_new(EV_P_ char *sock_path) {
  int len;
  struct sockaddr_un remote;

  if (-1 == (clic.remote_fd = socket(AF_UNIX, SOCK_STREAM, 0))) {
    perror("socket");
    exit(1);
  }

  // Set it non-blocking
  if (-1 == net_setnonblock(clic.remote_fd)) {
    perror("echo client socket nonblock");
    exit(EXIT_FAILURE);
  }

  // this should be initialized before the connect() so
  // that no packets are dropped when initially sent?
  // http://cr.yp.to/docs/connect.html

  // initialize the connect callback so that it starts the stdin asap
  ev_io_init(&remote_w, remote_cb, clic.remote_fd, EV_WRITE);
  ev_io_start(EV_A, &remote_w);
  // initialize the send callback, but wait to start until there is data to
  // write
  ev_io_init(&send_w, send_cb, clic.remote_fd, EV_READ);
  ev_io_start(EV_A, &send_w);

  remote.sun_family = AF_UNIX;
  strcpy(remote.sun_path, sock_path);
  len = strlen(remote.sun_path) + sizeof(remote.sun_family);

  if (-1 == connect(clic.remote_fd, (struct sockaddr *)&remote, len)) {
    perror("connect");
    // exit(1);
  }
}

int main(void) {
  wrapev_init();
  memset(&clic, 0, sizeof(clic));

  // initialise an io watcher, then start it
  // this one will watch for stdin to become readable
  net_setnonblock(0);
  ev_io_init(&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);

  clic.kcpid = kcp_wrap_new(good2write, good2read, &clic);
  struct ev_periodic kcp_check_ev;
  ev_periodic_init(&kcp_check_ev, kcp_check_handler, 0, 0.01, 0);
  ev_periodic_start(EV_A_ &kcp_check_ev);

  connection_new(EV_A_ "/tmp/libev-echo.sock");

  // now wait for events to arrive
  ev_loop(EV_A, 0);

  wrapev_deinit();
  kcp_wrap_release(clic.kcpid);

  // unloop was called, so exit
  return 0;
}
