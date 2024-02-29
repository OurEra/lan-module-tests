#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

static int isBigEndian()
{
  uint32_t thisx = 0x01020304;
  uint8_t *thisp = (uint8_t *) & thisx;
  return (*thisp == 1) ? 1 : 0;
}

int main(int argc, char **argv)
{
  printf("Byte order: %s Endian\n", isBigEndian()? "Big" : "Little");
  return 0;
}
