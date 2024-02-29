#include <stdio.h>
#include <pthread.h>

void *producter_f(void *arg);
void *consumer_f(void *arg);

char buffer[128];
pthread_mutex_t mutex;
pthread_cond_t cond;
int buffer_has_data = 0;

int main(void)
{
  pthread_t consumer_t, producter_t;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
  pthread_create(&producter_t, NULL, (void *) producter_f, NULL);
  pthread_create(&consumer_t, NULL, (void *) consumer_f, NULL);
  pthread_join(consumer_t, NULL);
  pthread_join(producter_t, NULL);
  pthread_cond_destroy(&cond);
  return 0;
}

void *producter_f(void *arg)
{
  int i = 0;
  while (1) {
    pthread_mutex_lock(&mutex);
    while (buffer_has_data)
      pthread_cond_wait(&cond, &mutex);
    if (!buffer_has_data) {
      sprintf(buffer, "data is %d", i);
      printf("produce data:[%s]\n", buffer);
      buffer_has_data = 1;
      i = (i++) % 10000;
      pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mutex);
  }
}

void *consumer_f(void *arg)
{
  while (1) {
    pthread_mutex_lock(&mutex);
    while (!buffer_has_data)
      pthread_cond_wait(&cond, &mutex);
    if (buffer_has_data) {
      printf("consume:total:[%s]\n", buffer);
      buffer_has_data = 0;
      pthread_mutex_unlock(&mutex);
      pthread_cond_signal(&cond);
    }
  }
}
