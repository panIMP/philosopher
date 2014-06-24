#include "pthreadc.h"

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

//宏定义信号量个数为5,宏定义0表示思考,宏定义1表示饿,宏定义2表示吃,宏定义(ph_num+4)%N表示左,宏定义(ph_num+1)%N表示右
#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+4)%N
#define RIGHT (ph_num+1)%N

 //声明操作函数,取叉函数,放叉函数,测试函数
void* philospher(void *num);
void takeFork(int);
void putFork(int);
void test(int);
int pthreadc();

//声明互斥锁 ,声明数组
static sem_t mutex;
sem_t S[N];


int state[N];//定义整型数组
int phil_num[N] = { 0, 1, 2, 3, 4 };//初始化数组



int pthreadc()
{
    int i;
    pthread_t thread_id[N];//声明线程ID数组
    sem_init(&mutex, 0, 1);//初使化信号量的状态为1，并且信号量在一个进程的线程间进行共享

    for (i = 0; i<N; i++)//初始化s数组
        sem_init(&S[i], 0, 0);//初始化S[i]的值为零

    for (i = 0; i<N; i++)
    {
        pthread_create(&thread_id[i], NULL, philospher, &phil_num[i]);//创建哲学家线程函数
        printf("Philosopher %d is ready to grab folks\n", i + 1);
    }

    for (i = 0; i<N; i++)
        pthread_join(thread_id[i], NULL);//等待thread_id[i]线程结束

    return 0;
}


void* philospher(void *num)
{
    while (1)
    {
        int *i = (int*)num;
        sleep(1);//调整频率
        takeFork(*i);//申请叉子
        sleep(0);//放弃当前线程执行的时间片，把自身放到等待队列之中。
        putFork(*i);//放下叉子
    }

    return nullptr;
}


void takeFork(int ph_num)
{
    sem_wait(&mutex);//给信号量的值减上一个“1”
    state[ph_num] = HUNGRY;//初始化状态为HUNGRY（饿）
    printf("Philosopher %d want to eat\n", ph_num + 1);
    test(ph_num);//测试函数
    sem_post(&mutex);//给信号量的值加上一个“1”
    sem_wait(&S[ph_num]);//给信号量的值减上一个“1”
    sleep(1);//调整频率
}


//******************能够吃就改变信号量的值和状态**********************************************
void test(int ph_num)
{    /*如果state[ph_num]状态为HUNGRY并且state[LEFT]和state[RIGHT]的状态都不等于EATING*/
    if (state[ph_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[ph_num] = EATING;//将state[ph_num]状态置为EATING（吃饭）
        sleep(2);//调整频率
        printf("Philosopher %d is eating now\n", ph_num + 1);
        sem_post(&S[ph_num]);//给信号量的值加上一个“1”

    }
}


//******************************************************************************************
void putFork(int ph_num)
{
    sem_wait(&mutex);//信号量减1
    state[ph_num] = THINKING;//将state[ph_num]状态置为THINKING（思考）
    printf("Philosopher %d is thinking now\n", ph_num + 1);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);//给信号量的值加上一个“1”
}

