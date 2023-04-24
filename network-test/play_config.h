#ifndef PLAY_CONFIG_H
#define PLAY_CONFIG_H

#include <stdint.h>

#define PLAY_SEND_FILE "yueyunpeng.mp3"
#define PLAY_RECV_FILE "yueyunpeng_receive.mp3"

#define PLAY_PORT 2029
#define MAX_PEER_NUM 100

#define HEADER_SZIE 4
#define CMD_START_STREAMING 0x100
#define CMD_STREAMING_CONTENT 0x101
#define CMD_FINISH_STREAMING 0x102

#endif // PLAY_CONFIG_H
