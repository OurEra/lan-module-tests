#include <stdio.h>
#include <pthread.h>

void *producter_f(void *arg);
void *consumer_f(void *arg);

char buffer[128];
pthread_mutex_t mutex;

int main(void)
{
  pthread_t consumer_t, producter_t;
  pthread_mutex_init(&mutex, NULL);
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
    pthread_mutex_lock(&mutex);
    sprintf(buffer, "data is %d", i);
    printf("produce data:[%s]\n", buffer);
    i=(i++)%10000;
    pthread_mutex_unlock(&mutex);
  }
}

void *consumer_f(void *arg)
{
  while (1) {
    pthread_mutex_lock(&mutex);
    printf("consume:total:[%s]\n", buffer);
    pthread_mutex_unlock(&mutex);
  }
}
