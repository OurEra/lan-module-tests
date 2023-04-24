#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "network.h"

// Simply adds O_NONBLOCK to the file descriptor of choice
int net_setnonblock(int fd) {
  int flags;
  flags = fcntl(fd, F_GETFL);
  flags |= O_NONBLOCK;
  return fcntl(fd, F_SETFL, flags);
}


/* Returns true if socket addresses are equal, false otherwise.  Only
 * families, IP addresses, and ports are compared.
 */
int
sockaddr_eq(const struct sockaddr *a, const struct sockaddr *b) {
  if (a->sa_family == AF_INET)
    return a->sa_family == b->sa_family
            && ((struct sockaddr_in *) a)->sin_addr.s_addr
                            == ((struct sockaddr_in *) b)->sin_addr.s_addr
            && ((struct sockaddr_in *) a)->sin_port
                            == ((struct sockaddr_in *) b)->sin_port;
  else
      return a->sa_family == b->sa_family
            && ((struct sockaddr_in6 *) a)->sin6_port ==
                                ((struct sockaddr_in6 *) b)->sin6_port
            && 0 == memcmp(&((struct sockaddr_in6 *) a)->sin6_addr,
                            &((struct sockaddr_in6 *) b)->sin6_addr,
                            sizeof(((struct sockaddr_in6 *) b)->sin6_addr));
}

void read_bige_uint32(const char buf[], uint32_t* out) {
   *out = buf[0];
   for (size_t i = 1; i < sizeof(uint32_t); ++i) {
     *out <<= 8;
     // Must cast to uint8_t to avoid clobbering by sign extension.
     *out |= (uint8_t)(buf[i]);
   }
}

void write_bige_uint32(char buf[], uint32_t val) {
   for (size_t i = 0; i < sizeof(uint32_t); ++i) {
     buf[sizeof(uint32_t) - i -1] = (char)(val & 0xFF);
     val >>= 8;
   }
}

