#ifndef BACKOFF_H
#define BACKOFF_H

#define ITERATIONS 10

#include <pthread.h>
#include "errors.h"


void*
lock_forward(void* arg);

void*
lock_backward(void *arg);

int
backoffFunc(int argc, char* argv[]);


#endif // BACKOFF_H
