#include "platform.h"
#include "leon3mp_pthread.h"
#include "asm.h"

/* Activate all CPUs
   CPU0 sets up global state table and starts other CPUs
   CPUX waits for functions to execute
 */
void initPlatform(void)
{
  unsigned int pid = get_pid();
  if(pid == 0) {
    int i;

    /* Initalised global status and pointers */
    for(i=0;i<MAX_PROC;++i) {
      leon3mp_pthread[i].status = DEACTIVE;
      leon3mp_pthread[i].func   = (void *)0;
      leon3mp_pthread[i].args   = (void *)0;
    }

    /* Loop through, start CPUs in sequence, wait for them to become READY */
    {
      unsigned short dmmr = *DebugModeMaskReg & 0xFFFF;
      unsigned int walk;
      for(walk=1;walk<MAX_PROC;++walk) {
	/* Check if bit walk is 1 in dmmr */
	if((dmmr >> walk) & 0x1) {
	  /* Start it */
	  *(InteruptController + 0x10/4) = (1 << walk);
	  /* Wait for status to be updated */
	  while(leon3mp_pthread[walk].status != READY) { }
	}
      }
    }

    /* Set global status */
    leon3mp_pthread[0].status = ACTIVE;
  }
  else {
    /* Wait for a function to be passed */
    leon3mp_pthread_t *this = (leon3mp_pthread_t *)&(leon3mp_pthread[pid]);
    this->status = READY;
    do {
      if(this->func != (void *)0) {
	/* There is a function to be executed */

	/* Set global status */
	this->status = ACTIVE;

	/* Execute the function */
	this->func((void *)this->args);

	/* Set function and argument pointers back to NULL */
	this->func = (void *)0;
	this->args = (void *)0;

	/* Set global status */
	this->status = READY;
      }
    } while(1);
  }
  return;
}

/* Only called by CPU0
   Halt other CPUs in the system
 */
void cleanupPlatform(void)
{
  if(get_pid() == 0) {
    /* Deactivate all processors */
    /* This causes the Leon3MP system to hit a breakpoint */
    *(InteruptController + 0x10/4) = 0x0;
  }
}
