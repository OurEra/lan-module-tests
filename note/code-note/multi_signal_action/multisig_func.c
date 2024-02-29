#include	<stdio.h>
#include	<signal.h>

main()
{
  void func(int);
  int i;
  signal(SIGINT, func);
  signal(SIGQUIT, func);
  for (i = 0; i < 5; i++) {
    printf("hello\n");
    sleep(1);
  }
}

void func(int signum)
{
  int i = 0;
  switch (signum) {
  case SIGINT:
    for (i = 0; i < 10; i++) {
      printf("%d\n", i);
      sleep(1);
    }
    break;
  case SIGQUIT:
    for (i = 10; i < 20; i++) {
      printf("%d\n", i);
      sleep(1);
    }
    break;
  default:
    printf("signal is :%d\n", signum);
    break;
  }
}
