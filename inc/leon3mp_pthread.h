/* Leon3MP limited PThread implementation */

#ifndef __GUARD_LEON3MP_PTHREAD_H
#define __GUARD_LEON3MP_PTHREAD_H

typedef unsigned int pthread_t;
typedef unsigned int pthread_attr_t;

int pthread_create(pthread_t *, const pthread_attr_t *, void *, void *);
int pthread_join(pthread_t, void **);

#endif
