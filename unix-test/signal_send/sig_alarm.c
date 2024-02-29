//sigdemo10
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

main()
{
  unsigned int i;
  alarm(1);
  for(i=0;1;i++)
  printf("i=%d\n",i);
}
