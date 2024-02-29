//域名解析程序
#include<stdio.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc, char **argv)
{
  char **pptr;
  char str[46];
  struct hostent *hptr;
  if (argc < 2) {
    fprintf(stderr, "uesge: domain <domain>\n");
    return -1;
  }

  /*get hostent struct*/
  if ((hptr = gethostbyname(argv[1])) == NULL) {
    fprintf(stderr, "gethostbyname call failed.%s\n", hstrerror(h_errno));
    return -1;
  }

  printf("official name:%s\n", hptr->h_name);

  /*get alias name*/
  for (pptr = hptr->h_aliases; *pptr != NULL; pptr++) {
    printf("\t alias:%s\n", *pptr);
  }

  /*get network domain*/
  if (hptr->h_addrtype != AF_INET) {
    fprintf(stderr, "Invalid address type %d\n", hptr->h_addrtype);
    return -1;
  }

  /*get ip address*/
  pptr = hptr->h_addr_list;
  for (; *pptr != NULL; pptr++) {
    printf("\t address:%s\n",
	   inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
  }
  return 0;
}

