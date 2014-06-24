#include "threadcpp.h"
#include "pthreadc.h"

#include <errno.h>
#include "errors.h"

#include <time.h>

#include "alarm_list.h"



pthread_mutex_t g_alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_alarm_cond = PTHREAD_COND_INITIALIZER;
alarm_t* g_alarm_list = NULL;
time_t g_current_alarm = 0;


void
alarm_insert(alarm_t* newAlarm)
{
    int status;
    alarm_t *pre, *cur;

    pre = cur = g_alarm_list;

    // Insert new alarm int the right position -- from early to late
    while (cur != NULL)
    {
        if (cur->time >= newAlarm->time)
        {
            pre->next = newAlarm;
            newAlarm->next = cur;
        }

        pre = pre->next;
        cur = cur->next;
    }

    // if current alarm list is empty
    if (cur == NULL)
    {
        g_alarm_list = newAlarm;
        g_alarm_list->next = NULL;
    }

    // if current_alarm is not busy or it's later than the new alarm, update it
    if (g_current_alarm == 0 || newAlarm->time < g_current_alarm)
    {
        g_current_alarm = newAlarm->time;

        // Signal the current_alarm change! to wake up alarm_thread
        status = pthread_cond_signal(&g_alarm_cond);
        if (status != 0)
            err_abort(status, "Signal alarm_cond");
    }
}



void*
alarm_thread(void* arg)
{
    alarm_t* newAlarm;
    struct timespec cond_time;
    time_t now;
    int status, expired;

    // Lock
    status = pthread_mutex_lock(&g_alarm_mutex);
    if (status != 0)
        err_abort(status, "Lock mutex");

    // Loop
    while (1)
    {
        // suggest current_alarm is empty -- inform the insert thread that the alarm thread is not busy
        g_current_alarm = 0;

        // If alarm list is empty, wait for input new alarm
        // iteration: unlock -> wait -> lock
        while (g_alarm_list == NULL)
        {
            // endless wait, suspend the alarm_thread
            status = pthread_cond_wait(&g_alarm_cond, &g_alarm_mutex);
            if (status != 0)
                err_abort(status, "Wait on cond");
        }

        // check the verb again!
        if (g_alarm_list != NULL)
        {
            // fetch the new alarm
            newAlarm = g_alarm_list;
            g_alarm_list = newAlarm->next;

            // update the current time and expire flag
            now = time(NULL);
            expired = 0;
        }

        // if the clock is later(not expire current times)
        if (newAlarm->time > now)
        {
            // update
            cond_time.tv_sec = newAlarm->time;
            cond_time.tv_nsec = 0;
            g_current_alarm = newAlarm->time;

            // suspend alarm_thread until new alarm has been added
            // or wait cond timed out(no new alarm added until currrent alarm reached time)
            // Check verb
            while (g_current_alarm == newAlarm->time)
            {
                // wait, if new alarm reached successful, g_current_alarm will be modified
                status = pthread_cond_timedwait(&g_alarm_cond, &g_alarm_mutex, &cond_time);

                // if timed out -- no new alarm and current alarm reached time
                if (status == ETIMEDOUT)
                {
                    // set flag and break out
                    expired = 1;
                    break;
                }

                if (status != 0)
                    err_abort(status, "Cond timedwait");
            }

            // check the reason for loop ending
            // 1. current alarm reached time OR 2. new alarm added, g_current_alarm != newAlarm->time
            if (!expired)
                // 2. new alarm added, g_current_alarm != newAlarm->time,
                // than insert current alarm again! handle the most near alarm again
                alarm_insert(newAlarm);
        }

        // or the current clock is already expired
        else
            expired = 1;

        // if expired, print current clock information
        if (expired)
        {
            printf("(%d) %s\n", newAlarm->seconds, newAlarm->message);
            free(newAlarm);
        }
    }

    return NULL;
}


int
alarm_list_func()
{
    // buffer and status variables
    int status;
    char line[128];

    // alarm_list pointers
    alarm_t *newAlarm;

    // thread id
    pthread_t thread;

    // create alarm_thread
    status = pthread_create(&thread, NULL, alarm_thread, NULL);
    if (!status)
        err_abort(status, "Creat alarm thread");

    // loop -- read input alarm settings and add them into alarm_list
    while (1) {
        // input prompt
        printf("Alarm> ");

        // read and parse input into alarm struct
        if (fgets(line, sizeof(line), stdin) == NULL)
            exit(0);

        // if inputs only one parameter, require re-input
        if (strlen(line) <= 1)
            continue;

        // get an new alarm
        newAlarm = (alarm_t*)malloc(sizeof(alarm_t));
        if (newAlarm == NULL)
            errno_abort("Allocate an alarm");

        if (sscanf(line, "%d %64[^\n]", &newAlarm->seconds, newAlarm->message) < 2)
        {
            fprintf(stderr, "Bad command\n");
            free(newAlarm);
        }

        // if input formats successful..
        else
        {
            // Lock
            status = pthread_mutex_lock(&g_alarm_mutex);
            if (!status)
                err_abort(status, "Lock mutex");

            // calculate the alarming time depend on alarm time gap
            newAlarm->time = newAlarm->seconds + time(NULL);

            // add new alarm into alarm_list
            alarm_insert(newAlarm);

            // Unlock
            status = pthread_mutex_unlock(&g_alarm_mutex);
            if (!status)
                err_abort(status, "Unlock mutex");
        }
    }

    return 0;
}



















