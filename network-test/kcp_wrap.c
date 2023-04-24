#include <ikcp.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include "kcp_wrap.h"
#include "play_config.h"

// kcp session exchange between client and server
uint32_t KCP_SESSION = 0x998712;
#define MAX_SESSION 100

typedef struct user_context_wrap {
  short id;
  void* user;
} user_context_wrap_t ;

typedef struct kcp_talk_context {
  ikcpcb *kcp;
  user_context_wrap_t uwrap;
  good2write_cb g2w;
  good2read_cb g2r;
} kcp_talk_context_t;

static kcp_talk_context_t kcps_[MAX_SESSION];
static short next_kcp_id = 0;

/* get clock in millisecond 64 */
static inline IINT64 iclock64(void) {
  IINT64 value;
  struct timeval time;
  gettimeofday(&time, NULL);
  value = ((IINT64)time.tv_sec) * 1000 + (time.tv_usec / 1000);
  return value;
}

static inline IUINT32 iclock() { return (IUINT32)(iclock64() & 0xfffffffful); }

static int ikcp_good2write(const char *buf, int len, ikcpcb *kcp, void *kuser) {
  user_context_wrap_t* uwrap = (user_context_wrap_t*) kuser;
  kcps_[uwrap->id].g2w(buf, len, uwrap->user);
  return 0;
}

static void ikcp_log_output(const char *log, struct IKCPCB *kcp, void *user) {
  printf("[klog]: %s\n", log);
}

int kcp_wrap_new(good2write_cb wcb, good2read_cb rcb, void* user) {
  if (next_kcp_id == 0) {
    memset(&kcps_, 0, sizeof(kcps_));
  }
  kcps_[next_kcp_id].uwrap.user = user;
  kcps_[next_kcp_id].uwrap.id = next_kcp_id;
  // kcp object
  kcps_[next_kcp_id].kcp = ikcp_create(KCP_SESSION, &(kcps_[next_kcp_id].uwrap));
  kcps_[next_kcp_id].kcp->logmask = 0;// 0: DISABLE; 0xfff: ALL
  kcps_[next_kcp_id].kcp->output = ikcp_good2write;
  kcps_[next_kcp_id].kcp->writelog = ikcp_log_output;
  ikcp_wndsize(kcps_[next_kcp_id].kcp, 128, 128);
  ikcp_nodelay(kcps_[next_kcp_id].kcp, 2, 10, 2, 1);
  kcps_[next_kcp_id].g2w = wcb;
  kcps_[next_kcp_id].g2r = rcb;
  return next_kcp_id++;
}

int kcp_wrap_prepare_for_write(const char *buffer, int len, int id) {
  return ikcp_send(kcps_[id].kcp, buffer, len);
}

int kcp_wrap_prepare_for_read(const char *data, long size, int id) {
  return ikcp_input(kcps_[id].kcp, data, size);
}

void kcp_wrap_process(int id) {
  ikcp_update(kcps_[id].kcp, iclock());
  char buffer[1500];
  int len = ikcp_recv(kcps_[id].kcp, buffer, 1500);
  if (len > 0) {
    user_context_wrap_t* uwrap = (user_context_wrap_t*) kcps_[id].kcp->user;
    kcps_[id].g2r(buffer, len, uwrap->user);
  }
}

void kcp_wrap_release(int id) {
  ikcp_release(kcps_[id].kcp);
}
