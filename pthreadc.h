#ifndef PTHREADC_H
#define PTHREADC_H

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

//�궨���ź�������Ϊ5,�궨��0��ʾ˼��,�궨��1��ʾ��,�궨��2��ʾ��,�궨��(ph_num+4)%N��ʾ��,�궨��(ph_num+1)%N��ʾ��
#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (ph_num+4)%N
#define RIGHT (ph_num+1)%N

 //������������,ȡ�溯��,�Ų溯��,���Ժ���
void* philospher(void *num);
void takeFork(int);
void putFork(int);
void test(int);
int pthreadc();




#endif // PTHREADC_H
