#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define	NUM	5
 void *print_msg(void *);
/*this example illustrate how to create thread*/
int main(int argc, char **argv)
{
  pthread_t t1, t2;		/* two threads */
int i = 0;
  char *a = "asb";

  /*create two thread */
  pthread_create(&t1, NULL, print_msg, (void *)a);
  pthread_create(&t2, NULL, print_msg, (void *) "world\n");
  for (i = 0; i < NUM; i++) {
    printf("main\n");
    fflush(stdout);
    sleep(1);
  }
}

/*thread running function*/
void *print_msg(void *m)
{
  char *cp = (char *) m;
  int i;
  for (i = 0; i < NUM; i++) {
    printf("%s", cp);
    fflush(stdout);
    sleep(1);
  }
  return NULL;
}
