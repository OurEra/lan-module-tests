#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem1, sem2;
char buffer[128];

void *producter_f(void *arg);
void *consumer_f(void *arg);


int main(void)
{
  pthread_t consumer_t, producter_t;
  sem_init(&sem1, 0, 1);
  sem_init(&sem2, 0, 0);
  pthread_create(&producter_t, NULL, (void *) producter_f, NULL);
  pthread_create(&consumer_t, NULL, (void *) consumer_f, NULL);
  pthread_join(consumer_t, NULL);
  pthread_join(producter_t, NULL);
  sem_destroy(&sem1);
  sem_destroy(&sem2);
  return 0;
}

void *producter_f(void *arg)
{
  int i = 0;
  while (1) {
    sem_wait(&sem1);
    sprintf(buffer, "data is %d", i);
    printf("produce data:[%s]\n", buffer);
    i = (i++) % 10000;
    sem_post(&sem2);
  }
}

void *consumer_f(void *arg)
{
  while (1) {
    sem_wait(&sem2);
    printf("consume:total:[%s]\n", buffer);
    sem_post(&sem1);
  }
}
