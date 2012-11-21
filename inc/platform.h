#ifndef _GUARD_PLATFORM_H
#define _GUARD_PLATFORM_H

/* CPU status */
#define DEACTIVE -1
#define READY     0
#define ACTIVE    1

/* Max number of processors */
#define MAX_PROC 16

/* Leon3MP internal registers */
#ifndef INTER_REG_ADDR
#define INTER_REG_ADDR 0x80000200
#endif

#ifndef DEBUG_REG_ADDR
#define DEBUG_REG_ADDR 0x90000024
#endif

#define InteruptController ((int *)INTER_REG_ADDR)
#define DebugModeMaskReg   ((int *)DEBUG_REG_ADDR)

typedef unsigned int (*functionPointer)(void *);

typedef struct leon3mp_pthread_t {
	volatile functionPointer func;
	volatile void *args;
	volatile int status;
} leon3mp_pthread_t;

volatile leon3mp_pthread_t leon3mp_pthread[MAX_PROC];

void initPlatform(void);
void cleanupPlatform(void);

#endif
