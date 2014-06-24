#include "backoff.h"


pthread_mutex_t mutex[3] =
{
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
};


int backoff = 1;
int yield_flag = 0;


void*
lock_forward(void* arg)
{
    int i, iterate, backoffs;
    int status;

    for (iterate = 0; iterate < ITERATIONS; ++iterate)
    {
        backoffs = 0;

        for (i = 0; i < 3; ++i)
        {
            if (i == 0)
            {
                status = pthread_mutex_lock(&mutex[i]);
                if (status != 0)
                    err_abort(status, "Lock mutex 0");
            }
            else
            {
                if (backoff)
                    status = pthread_mutex_trylock(&mutex[i]);
                else
                    status = pthread_mutex_lock(&mutex[i]);

                if (status == EBUSY)
                {
                    backoffs++;
                    DPRINTF(("[forward locker backing off at %d]\n", i));
                    for (; i >= 0; i--)
                    {
                        status = pthread_mutex_lock(&mutex[i]);
                        if (status != 0)
                            err_abort(status, "Back off");
                    }
                }
                else
                {
                    if (status != 0)
                        err_abort(status, "Lock mutex");

                    DPRINTF(("forward locker got %d\n", i));
                }
            }

            if (yield_flag)
            {
                // need specific explaination of the function
                if (yield_flag > 0)
                    sched_yield();
                else
                    sleep(1);
            }
        }

        printf("Lock forward got all locks, %d backoffs\n", backoffs);
        pthread_mutex_lock(&mutex[2]);
        pthread_mutex_lock(&mutex[1]);
        pthread_mutex_lock(&mutex[0]);
        sched_yield();
    }

    return NULL;
}


void*
lock_backward(void *arg)
{
    int i, iterate, backoffs;
    int status;

    for (iterate = 0; iterate < ITERATIONS; ++iterate)
    {
        backoffs = 0;

        for (i = 2; i >= 0; i--)
        {
            if (i == 2)
            {
                status = pthread_mutex_lock(&mutex[i]);
                if (status != 0)
                    err_abort(status, "First lock");
            }
            else
            {
                if (backoff)
                    status = pthread_mutex_trylock(&mutex[i]);
                else
                    status = pthread_mutex_lock(&mutex[i]);

                if (status == EBUSY)
                {
                    backoffs++;
                    DPRINTF(("[backward locker backing off at %d]\n", i));

                    for (i = 0; i < 3; i++)
                    {
                        status = pthread_mutex_unlock(&mutex[i]);
                        if (status != 0)
                            err_abort(status, "Backoff");
                    }
                }
                else
                {
                    if (status != 0)
                        err_abort(status, "Lock mutex");
                    DPRINTF(("backward locker got %d\n", i));
                }
            }

            if (yield_flag)
            {
                if (yield_flag > 0)
                    sched_yield();
                else
                    sleep(1);
            }
        }

        printf("Lock backward got all locks, %d backoffs\n", backoffs);
        pthread_mutex_lock(&mutex[0]);
        pthread_mutex_lock(&mutex[1]);
        pthread_mutex_lock(&mutex[2]);
        sched_yield();
    }

    return NULL;
}


int
backoffFunc(int argc, char* argv[])
{
    pthread_t forward, backward;
    int status;

    if (argc > 1)
        backoff = atoi(argv[1]);

    if (argc > 2)
        yield_flag = atoi(argv[2]);

    status = pthread_create(&backward, NULL, lock_backward, NULL);
    if (status != 0)
        err_abort(status, "create lock_backward");

    status = pthread_create(&forward, NULL, lock_forward, NULL);
    if (status != 0)
        err_abort(status, "create lock_forward");

    pthread_exit(NULL);

    return 0;
}






































