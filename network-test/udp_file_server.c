#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
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
#include "list.h"

enum peer_state {
  IDEL = 0,
  TALKING,
  FINISH,
};

typedef struct talk_peer {
  struct sockaddr_in client_addr;
  int kcpid;
  FILE* task_file;
  enum peer_state state;
  list_head link;
} talk_peer_t;

typedef struct talk_context {
  ev_io sockio;
  int sockfd;
  list_head peer_list;
} talk_context_t;

static talk_context_t tc_;

static void good2write(const char *buf, int len, void* user) {
  talk_peer_t* peer = (talk_peer_t*) user;
  list_head *pos = NULL;
  sendto(tc_.sockfd, buf, len, 0,
       (struct sockaddr *) &peer->client_addr, sizeof(peer->client_addr));
}

static void good2read(const char *buf, int len, void* user) {
  talk_peer_t* peer = (talk_peer_t*) user;
  uint32_t header_cmd = 0;
  read_bige_uint32(buf, &header_cmd);
  if (header_cmd == CMD_START_STREAMING) {
    peer->state = TALKING;
    printf("receive CMD_START_STREAMING\n");
  }
}

void serve_peer_incoming(struct sockaddr_in* in_addr, uint8_t* buffer, size_t len) {
  list_head *pos = NULL;
  talk_peer_t* target_peer = NULL;
  int new_peer = 1;
  list_for_each(pos, &tc_.peer_list) {
    talk_peer_t *peer = list_entry(pos, talk_peer_t, link);
    if (sockaddr_eq((struct sockaddr *)&(peer->client_addr), (struct sockaddr *)in_addr)) {
      target_peer = peer;
      new_peer = 0;
      break;
    }
  }
  if (new_peer) {
    target_peer = malloc(sizeof(talk_peer_t));
    target_peer->kcpid = kcp_wrap_new(good2write, good2read, target_peer);
    list_init(&target_peer->link);
    target_peer->client_addr = (*in_addr);
    target_peer->task_file = fopen("test.txt", "r");
    printf("new peer %s, open task file %p\n", inet_ntoa(target_peer->client_addr.sin_addr), target_peer->task_file);
    list_add_tail(&target_peer->link, &tc_.peer_list);
  }
  kcp_wrap_prepare_for_read(buffer, len, target_peer->kcpid);
}

void serve_peers_task() {
  list_head *pos = NULL;
  list_for_each(pos, &tc_.peer_list) {
    talk_peer_t *peer = list_entry(pos, talk_peer_t, link);
    if (peer->state == TALKING) {
       char buf[1000 + HEADER_SZIE];
       uint32_t header_cmd = CMD_STREAMING_CONTENT;
       write_bige_uint32(buf, header_cmd);
       int readed = fread(buf + HEADER_SZIE, 1, sizeof(buf) - HEADER_SZIE, peer->task_file);
       if (readed > 0) {
          int ret = kcp_wrap_prepare_for_write(buf, readed + HEADER_SZIE, peer->kcpid);
       } else {
         uint32_t header_cmd = CMD_FINISH_STREAMING;
         uint8_t packet[sizeof(header_cmd)];
         write_bige_uint32(packet, header_cmd);
         kcp_wrap_prepare_for_write(packet, sizeof(packet), peer->kcpid);
         peer->state = FINISH;
         printf("peer %s, finish with %d\n", inet_ntoa(peer->client_addr.sin_addr), ftell(peer->task_file));
       }
    }
  }
}

void free_talk_peers() {
  list_head *pos = NULL, *temp = NULL;
  list_for_each_safe(pos, temp, &tc_.peer_list) {
    talk_peer_t *peer = list_entry(pos, talk_peer_t, link);
    kcp_wrap_release(peer->kcpid);
    if (peer->task_file) {
      fclose(peer->task_file);
    }
    list_del(pos);
    free(peer);
  }
}

static void socket_event_handler(EV_P_ ev_io *w, int revents) {
  list_head *pos = NULL;
  list_for_each(pos, &tc_.peer_list) {
    talk_peer_t *peer = list_entry(pos, talk_peer_t, link);
    kcp_wrap_process(peer->kcpid);
  }
  if (revents & EV_READ) {
    uint8_t buffer[256];
    struct sockaddr_in talk_addr;
    int len = sizeof(talk_addr);
    int recv = recvfrom(tc_.sockfd, buffer, sizeof(buffer), 0,
			(struct sockaddr *) &talk_addr, &len);
    if (recv > 0) {
      serve_peer_incoming(&talk_addr, buffer, recv);
    }
  } else if (revents & EV_WRITE) {
      serve_peers_task();
  }
}

int main(int argc, char **argv)
{
  struct sockaddr_in server_addr;
  memset(&tc_, 0, sizeof(tc_));
  list_init(&tc_.peer_list);

  int opt = 1;
  int ret = RET_OK;

  /*setup socket */
  if ((tc_.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("ERROR opening socket");
    return RET_ERR;
  }

  // set it non-blocking
  if (-1 == net_setnonblock(tc_.sockfd)) {
    perror("echo client socket nonblock");
    exit(EXIT_FAILURE);
  }

  /*set network options */
  if ((ret = setsockopt(tc_.sockfd, SOL_SOCKET, SO_REUSEADDR,
			&opt, sizeof(opt))) < 0) {
    perror("ERROR setsockpot");
    goto failed;
  }

  /*fill up server ip address structure */
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PLAY_PORT);

  wrapev_init();
  ev_io_init(&tc_.sockio, socket_event_handler, tc_.sockfd, EV_WRITE | EV_READ);
  ev_io_start(EV_A, &tc_.sockio);

  /*bind server address */
  if ((ret = bind(tc_.sockfd, (struct sockaddr *) &server_addr,
		  sizeof(server_addr))) < 0) {
    perror("ERROR on binding");
    goto failed;
  }

  ev_loop(EV_A, 0);

failed:
  wrapev_deinit();
  close(tc_.sockfd);
  free_talk_peers();
  return 0;
}
