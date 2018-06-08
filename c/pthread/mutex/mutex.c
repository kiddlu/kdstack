#include <stdio.h>
#include <pthread.h>

//mutex: sleep-waiting lock 

//use code block for lock pair
#define PTHREAD_MUTEX_LOCK(m) \
  { \
    pthread_mutex_lock(m);

#define PTHREAD_MUTEX_UNLOCK(m) \
    pthread_mutex_unlock(m); \
  }

int main(void)
{
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  PTHREAD_MUTEX_LOCK(&mutex);
  printf("Locked\n");

  PTHREAD_MUTEX_UNLOCK(&mutex);
  printf("Unlocked\n");

  return 0;
}
