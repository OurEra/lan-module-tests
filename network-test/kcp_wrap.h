#ifndef PLAY_KCP_WRAP_H
#define PLAY_KCP_WRAP_H

typedef void (*good2write_cb)(const char *buf, int len, void* user);
typedef void (*good2read_cb)(const char *buf, int len, void* user);

int kcp_wrap_new(good2write_cb wcb, good2read_cb rcb, void* user);

int kcp_wrap_prepare_for_write(const char *buffer, int len, int id);

int kcp_wrap_prepare_for_read(const char *data, long size, int id);

void kcp_wrap_process(int id);

void kcp_wrap_release(int id);

#endif // PLAY_KCP_WRAP_H
