#ifndef PLAY_NETWORK_H
#define PLAY_NETWORK_H

int net_setnonblock(int fd);

void read_bige_uint32(const char buf[], uint32_t* out);

void write_bige_uint32(char buf[], uint32_t val);


#endif // PLAY_NETWORK_H
