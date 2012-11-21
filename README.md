#Leon3MP PThread Implemenation#

Small subset of the PThread library for use with the Leon3MP processor to allow Simultaneous Multi Threading with an common API.

Only two PThread opeations are implemented at the moment;
pthread_create()
pthread_join()


##Example Usage##
```C
#include "platform.h"
#include "leon3mp_pthread.h"

...

initPlatform();
/*
  CPU0 initialises leon3_pthread data structure and starts all other processors
  CPUX waits for function to be passed
 */

...
/*
  Usage of pthread_create and pthread_join functions for thread management
 */
...

cleanupPlatform();
/*
  CPU0 deactivates all processors
 */

```

Tested using upto 8 CPU Leon3MP system on Xilix ML605

##Compile##
To compile test.c from the test directory:
sparc-elf-gcc test.c ../src/platform.h ../src/leon3mp_pthread.c -I../inc


##Notes##
Make sure that assembly/crt0.s is being used otherwise when CPU0 starts all other CPUs the global memory will be reset.
(sparc-elf-gcc seems to use different directories dependent on configuration; eg. using soft-float flag broke this usage)
See assembly/README.md for more info.
