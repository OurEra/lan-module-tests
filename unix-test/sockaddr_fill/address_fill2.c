#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

void address_fill()
{
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(3001);
  if (inet_pton(AF_INET, "192.168.2.1", &sin.sin_addr.s_addr) < 0) {
    perror("inet_pton error");
    exit(-1);
  }
}

int main(int argc, char **argv)
{
  address_fill();
  return 0;
}
