#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void address_fill()
{
  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family=AF_INET;
  sin.sin_port = htons(3001);
  sin.sin_addr.s_addr = htonl(0x0a80201);	//192.168.2.1
}

int main(int argc, char **argv)
{
  address_fill();
  return 0;
}
