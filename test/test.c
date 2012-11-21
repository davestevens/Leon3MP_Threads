#include <stdio.h>
#include "platform.h"
#include "leon3mp_pthread.h"

int arg;
pthread_t id[2];

void *func1(void *);
void *func2(void *);

int main(void)
{
  initPlatform();

  /* Create two threads */
  arg = 123;
  if(pthread_create(&id[0], NULL, func1, (void *)0) == -1) {
    fprintf(stderr, "Error creating thread (0)\n");
  }
  if(pthread_create(&id[1], NULL, func2, &arg) == -1) {
    fprintf(stderr, "Error creating thread (1)\n");
  }

  /* Join the two threads */
  {
    int i;
    for(i=0;i<2;++i) {
      pthread_join(id[i], NULL);
    }
  }

  cleanupPlatform();
  return 0;
}

void *func1(void *arg)
{
  printf("func1\n");
  return (void *)0;
}

void *func2(void *arg)
{
  int a = (int)*(int *)arg;
  printf("func2: %d\n", a);
  return (void *)0;
}
