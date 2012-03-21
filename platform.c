#include "platform.h"
#include <stdio.h>

/* assembly for get_pid function */
asm(
"	.text\n"
"	.align 4\n"
"	.global get_pid\n"
"get_pid:\n"
"        mov  %asr17, %o0\n"
"        srl  %o0, 28, %o0\n"
"        retl\n"
"        and %o0, 0xf, %o0\n"
    );

void initPlatform(void) {
  /* if cpu0 setup status array */
  if(get_pid() == 0) {
    int i;
    *(interuptController + 0x10/4) = 0x0ffff; /* start all cpus */

    /* find which processors are available */
    unsigned short dmmr = *DebugModeMaskReg & 0xffff;

    /* this adds enough of a stall to let all other procs catch up */
    /* TODO: find a way to sync all procs up to a certain point
       activating them seems to reset all global memory so it resets
       the status array :S */
    unsigned numActiveProc = count(dmmr);

    status[0] = ACTIVE; /* set cpu as ACTIVE */

    /* set function pointers */
    for(i=0;i<NUM_PROC;i++) {
      functionP[i].func = (void *)0;
      functionP[i].args = (void *)0;
    }

    for(i=1;i<NUM_PROC;i++) { /* set all others to DEACTIVE */
      if(((dmmr >> i) & 0x1)) {
	status[i] = READY;
      }
      else {
	status[i] = DEACTIVE;
      }
    }
  }
  else {
    /* need to wait for all available processors to get to here */
    while(1) {
      if(functionP[get_pid()].func != (void *)0) {
	/* this means there is a function to run */

	/* set status to active */
	status[get_pid()] = ACTIVE;

	/* call the function */
	functionP[get_pid()].func(functionP[get_pid()].args);

	/* set function pointer back to null */
	functionP[get_pid()].func = (void *)0;
	functionP[get_pid()].args = (void *)0;
	/* set back to ready */
	status[get_pid()] = READY;
      }
    }
  }
  return;
}

void cleanupPlatform(void) {
  if(get_pid() == 0) {
    /* deactivate all processors */
    *(interuptController + 0x10/4) = 0x0;
  }
  return;
}

/* create a new thread of execution */
/* arguments: f = address of function to call
              a = void pointer to arguments
   return: pid of CPU executing function */
int create_thread(void *f, void *a) {
  int i;
  int chosen = 0;
  for(i=1;i<NUM_PROC;i++) {
    if(status[i] == READY) {
      chosen = i;
      break;
    }
  }
  if(!chosen) {
    printf("Could not find a free CPU\n");
    return -1;
  }
  else {
    /*printf("(%d) allocating %d for this function\n", get_pid(), chosen);*/
    /* set function pointer and argument pointer */
    functionP[chosen].func = f;
    functionP[chosen].args = a;

    /* set cpu to active */
    status[chosen] = ACTIVE;
    return chosen;
  }
}

/* wait on thread of execution */
/* arguments: t = pid of cpu to wait for
   return: 0 = success, -1 = thread is deactive */
int join_thread(int t) {
  if(status[t] == DEACTIVE) {
    return DEACTIVE;
  }
  while(status[t] == ACTIVE) { }
  return 0;
}

/* uses the debug mode mask to count number of available processors */
unsigned count(unsigned short dmmr) {
  int i;
  char num = 0;
  for(i=0;i<NUM_PROC;i++) {
    if(((dmmr >> i) & 0x1)) {
      num++;
    }
  }
  return num;
}
