#include<stdlib.h>
#include<stdio.h>
#include<linux/unistd.h>
#include<sys/types.h>
#include<errno.h>
#include<pthread.h>

void thread(void)
{
  int i;
  for (i = 0; i < 3; i++) {
    printf("This is a pthread.\n");
    sleep(1);
  }
  pthread_exit((void *) i);
}

int main(void)
{
  pthread_t id;
  int i, ret;
  void *result;
  ret = pthread_create(&id, NULL, (void *) thread, NULL);
  if (ret != 0) {
    printf("Create pthread error!");
    exit(1);
  }
  for (i = 0; i < 3; i++) {
    printf("This is the main process.\n");
    sleep(1);
  }
  pthread_join(id, &result);
  printf("Child thread return [%d]\n", (int) result);
  return 0;
}
