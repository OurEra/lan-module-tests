#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#include "ev_wrap.h"
#include "kcp_wrap.h"
#include "network.h"
#include "play_code.h"
#include "play_config.h"

typedef struct cli_talk_context {
  ev_io sockio;
  int sockfd;
  int kcpid;
  short did_notify;
  int total_receive;
  struct sockaddr_in servaddr;
} cli_talk_context_t;

static cli_talk_context_t tc_;

static void good2write(const char *buf, int len, void* user) {
  cli_talk_context_t* cli = (cli_talk_context_t*) user;
  sendto(cli->sockfd, buf, len, 0,
    (struct sockaddr *) &(cli->servaddr), sizeof(cli->servaddr));
}

static void good2read(const char *buf, int len, void* user) {
  cli_talk_context_t* cli = (cli_talk_context_t*) user;
  uint32_t header_cmd = 0;
  read_bige_uint32(buf, &header_cmd);
  cli->total_receive += (len - HEADER_SZIE);
  if (header_cmd == CMD_FINISH_STREAMING) {
    printf("receive CMD_FINISH_STREAMING, total %d\n", cli->total_receive);
  }
}

static void socket_event_handler(EV_P, ev_io *w, int revents) {
  cli_talk_context_t* cli = (cli_talk_context_t*) w;
  kcp_wrap_process(cli->kcpid);
  if ((revents & EV_WRITE) && !tc_.did_notify) {
    uint32_t header_cmd = CMD_START_STREAMING;
    uint8_t packet[sizeof(header_cmd)];
    write_bige_uint32(packet, header_cmd);
    tc_.did_notify = 1;
    kcp_wrap_prepare_for_write(packet, sizeof(packet),cli->kcpid);
  } else if (revents & EV_READ) {
    char buffer[1500];
    struct sockaddr_in recvaddr;
    uint32_t alen = sizeof(recvaddr);
    int ret = recvfrom(tc_.sockfd, buffer, sizeof(buffer), 0,
			(struct sockaddr *) &recvaddr, &alen);
    if (ret > 0) {
      kcp_wrap_prepare_for_read(buffer, ret, cli->kcpid);
    } else {
      perror("read socket");
    }
  }
}

int main(int argc, char *argv[])
{
  int ret = RET_OK;
  memset(&tc_, 0, sizeof(cli_talk_context_t));
  struct hostent *server;

  if (argc < 2) {
    fprintf(stderr, "usage %s hostname\n", argv[0]);
    return RET_ERR;
  }

  if ((server = gethostbyname(argv[1])) == NULL) {
    perror("gethostbyname. ");
    return RET_ERR;
  }

  /*set up socket */
  if ((tc_.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("ERROR opening ocket");
    return RET_ERR;
  }
  // set it non-blocking
  if (-1 == net_setnonblock(tc_.sockfd)) {
    perror("echo client socket nonblock");
    exit(EXIT_FAILURE);
  }

  /*fill up ip address structure */
  memset(&tc_.servaddr, 0, sizeof(tc_.servaddr));
  tc_.servaddr.sin_family = AF_INET;
  tc_.servaddr.sin_addr.s_addr = *(uint32_t *) server->h_addr;
  tc_.servaddr.sin_port = htons((uint16_t) PLAY_PORT);

  tc_.kcpid = kcp_wrap_new(good2write, good2read, &tc_);

  wrapev_init();
  ev_io_init(&tc_.sockio, socket_event_handler, tc_.sockfd, EV_WRITE | EV_READ);
  ev_io_start(EV_A, &tc_.sockio);

  /*connect to server */
  if (connect(tc_.sockfd, (struct sockaddr *) &tc_.servaddr, sizeof(tc_.servaddr)) < 0) {
    perror("ERROR connecting");
    goto failed;
  }

  ev_loop(EV_A, 0);

failed:
  wrapev_deinit();
  kcp_wrap_release(tc_.kcpid);
  close(tc_.sockfd);
  return ret < 0 ? RET_ERR : RET_OK;
}
