#ifndef ALARM_LIST_H
#define ALARM_LIST_H

#include <pthread.h>
#include <time.h>
#include "errors.h"

typedef struct alarm_tag {
    struct alarm_tag* next;
    int seconds;
    time_t time;
    char message[64];
}alarm_t;

void*
alarm_thread(void* arg);

int
alarm_list_func();

#endif // ALARM_LIST_H
