#include "cond.h"
#include <time.h>


my_struct_t data =
{
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    0
};


int hibernation = 1;


void* wait_thread(void* arg)
{
    int status;

    // Sleep first, than change and signal
    sleep(hibernation);

    // Lock mutex
    status = pthread_mutex_lock(&(data.mutex));
    if (status != 0)
        err_abort(status, "Lock mutex");

    // Set value -- changes 0 --> 1
    data.value = 1;

    // Signal -- value has been changed
    status = pthread_cond_signal(&(data.cond));
    if (status != 0)
        err_abort(status, "Signal condition");

    // Unlock mutex
    status = pthread_mutex_unlock(&(data.mutex));
    if (status != 0)
        err_abort(status, "Unlock mutex");

    return NULL;
}


int cond_func(int argc, char* argv[])
{
    pthread_t wait_thread_id;
    int status;
    timespec timeout;

    if (argc > 1)
        hibernation = atoi(argv[1]);

    status = pthread_create(&wait_thread_id, NULL, wait_thread, NULL);
    if (status != 0)
        err_abort(status, "Create thread");

    /*
     * Wait on the condition varable for 2 seconds, or until signaled by the wait_thread,
     * Normally wait_thread should signal, if you raise "hibernation" above 2 seconds, it will time out.
    */
    timeout.tv_sec = time(NULL) + 2;
    timeout.tv_nsec = 0;

    // Lock
    status = pthread_mutex_lock(&(data.mutex));
    if (status != 0)
        err_abort(status, "Lock mutex");

    // Verb tests!
    while(data.value == 0)
    {
        // every wait lasts for  a period of time
        status = pthread_cond_timedwait(&(data.cond), &(data.mutex), &timeout);
        if (status == ETIMEDOUT)
            printf("Condition wait timed out.\n");
        else if (status != 0)
            err_abort(status, "Wait on condition");
    }

    // Check again -- cond_func thread wake up
    if (data.value != 0)
        printf("Condition was signaled.\n");

    // Unlock
    status = pthread_mutex_unlock(&(data.mutex));
    if (status != 0)
        err_abort(status, "Unlock mutex");

    return 0;
}




































































