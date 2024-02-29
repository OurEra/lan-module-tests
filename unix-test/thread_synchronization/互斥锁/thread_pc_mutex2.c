
#include <stdio.h>
#include <pthread.h>

void *producter_f(void *arg);
void *consumer_f(void *arg);

char buffer[128];
pthread_mutex_t mutex1, mutex2;

int main(void)
{
  pthread_t consumer_t, producter_t;
  pthread_mutex_init(&mutex1, NULL);
  pthread_mutex_init(&mutex2, NULL);
  pthread_mutex_lock(&mutex2);
  pthread_create(&producter_t, NULL, (void *) producter_f, NULL);
  pthread_create(&consumer_t, NULL, (void *) consumer_f, NULL);
  pthread_join(consumer_t, NULL);
  pthread_join(producter_t, NULL);
  return 0;
}

void *producter_f(void *arg)
{
  int i = 0;
  while (1) {
    pthread_mutex_lock(&mutex1);
    sprintf(buffer, "data is %d", i);
    printf("produce data:[%s]\n", buffer);
    i=(i++)%10000;
    pthread_mutex_unlock(&mutex2);
  }
}

void *consumer_f(void *arg)
{
  while (1) {
    pthread_mutex_lock(&mutex2);
    printf("consume:total:[%s]\n", buffer);
    pthread_mutex_unlock(&mutex1);
  }
}
