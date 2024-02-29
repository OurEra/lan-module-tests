#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

void byte_order_change()
{
 uint16_t  val=htons(2019);
 printf("val is :%d\n",val);
 uint32_t val2=htonl(15535);
 printf("val is :%p\n",val2);
}

int main()
{
 byte_order_change();
 return 0;
}
