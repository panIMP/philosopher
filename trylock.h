#ifndef TRYLOCK_H
#define TRYLOCK_H

#include <pthread.h>
#include "errors.h"
#include <time.h>

#define SPIN 10000000

int
tryLock();

#endif // TRYLOCK_H
