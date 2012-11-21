#include "leon3mp_pthread.h"
#include "platform.h"

/* Find a free CPUs to execute function
   Returns -1 on error or 0 on success
 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
		   void *start_routine, void *arg)
{
  int i, chosen = -1;
#ifdef ADD_STALL
  unsigned int s = clock();
#ifdef LEON3
  while((clock() - s) < 207) { }
#elif MICROBLAZE
  while((clock() - s) < 28) { }
#else
#error (Define LEON3 or MICROBLAZE)
#endif
#endif
  for(i=1;i<MAX_PROC;++i) {
    if(leon3mp_pthread[i].status == READY) {
      chosen = i;
      break;
    }
  }

  if(chosen == -1) {
    return -1;
  }

  /* */
  {
    leon3mp_pthread_t *this = (leon3mp_pthread_t *)&(leon3mp_pthread[chosen]);
    this->status = ACTIVE;
    this->func = start_routine;
    this->args = arg;

    *thread = chosen;
    return 0;
  }
  return -1;
}

/* Blocks until thread has completed execution
 */
int pthread_join(pthread_t thread, void **value_ptr)
{
#ifdef ADD_STALL
	unsigned int s = clock();
#ifdef LEON3
	while((clock() - s) < 4787) { }
#elif MICROBLAZE
	while((clock() - s) < 17) { }
#else
#error (Define LEON3 or MICROBLAZE)
#endif
#endif
  if(leon3mp_pthread[thread].status == DEACTIVE) {
    return DEACTIVE;
  }
  while(leon3mp_pthread[thread].status == ACTIVE) { }
  return 0;
}
