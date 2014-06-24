#ifndef PTHREADC_H
#define PTHREADC_H

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




#endif // PTHREADC_H
