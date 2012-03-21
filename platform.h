#ifndef __GUARD_PLATFORM_H
#define __GUARD_PLATFORM_H

/* status of cpu */
#define DEACTIVE  -1
#define READY     0
#define ACTIVE    1

/* total number of processors */
#define NUM_PROC  16

#define DebugModeMaskReg ((int *)0x90000024)
#define interuptController ((int *)0x80000200)

typedef int (*functionPointer)(void *);

typedef struct {
  functionPointer func;
  void *args;
} function;

volatile int status[NUM_PROC];
volatile function functionP[NUM_PROC];

void initPlatform(void);
void cleanupPlatform(void);
int create_thread(void *, void *);
int join_thread(int);
unsigned count(unsigned short);

#endif
