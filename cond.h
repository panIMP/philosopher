#ifndef COND_H
#define COND_H

#include <pthread.h>
#include "errors.h"


typedef struct _MY_STRUCT_TAG {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
}my_struct_t;


int cond_func(int argc, char* argv[]);


#endif // COND_H
