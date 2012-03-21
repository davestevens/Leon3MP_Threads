#include <stdio.h>
#include "platform.h"

void *func1(void *);
void *func2(void *);

int arg;
int id[2] = {-1};

int main(void) {
  int i;

  /* depending on pid either setup system or go into wait mode */
  initPlatform();

  /* 2 threads running at the same time */
  arg = 123;
  id[0] = create_thread(&func1, (void *)arg);
  if(id[0] == -1) {
    printf("could not find a cpu to run this on.\n");
    return -1;
  }
  id[1] = create_thread(&func2, (void *)0);
  if(id[1] == -1) {
    printf("could not find a cpu to run this on.\n");
    return -1;
  }

  /* join the two threads we've started */
  for(i=0;i<2;i++) {
    join_thread(id[i]);
  }

  /* 2 threads running one after another */
  arg = 456;
  id[0] = create_thread(&func1, (void *)arg);
  if(id[0] == -1) {
    printf("could not find a cpu to run this on.\n");
    return -1;
  }
  join_thread(id[0]);

  id[1] = create_thread(&func2, (void *)0);
  if(id[1] == -1) {
    printf("could not find a cpu to run this on.\n");
    return -1;
  }
  join_thread(id[0]);


  /* join all threads */
  /* starting from 1, as 0 is this thread */
  for(i=1;i<NUM_PROC;i++) {
    join_thread(i);
  }

  /* only CPU0 should ever get here, disable all other cpus and exit */
  cleanupPlatform();
  return 0;
}

/* example funcs to call */
void *func1(void *arg) {
  int a = (int)arg;
  printf("func1\n");
  printf("%d\n", a);
  return arg;
}

void *func2(void *arg){
  printf("func2\n");
  return arg;
}
