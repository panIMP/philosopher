#include "pthreadc.h"

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

//���������� ,��������
static sem_t mutex;
sem_t S[N];


int state[N];//������������
int phil_num[N] = { 0, 1, 2, 3, 4 };//��ʼ������



int pthreadc()
{
    int i;
    pthread_t thread_id[N];//�����߳�ID����
    sem_init(&mutex, 0, 1);//��ʹ���ź�����״̬Ϊ1�������ź�����һ�����̵��̼߳���й���

    for (i = 0; i<N; i++)//��ʼ��s����
        sem_init(&S[i], 0, 0);//��ʼ��S[i]��ֵΪ��

    for (i = 0; i<N; i++)
    {
        pthread_create(&thread_id[i], NULL, philospher, &phil_num[i]);//������ѧ���̺߳���
        printf("Philosopher %d is ready to grab folks\n", i + 1);
    }

    for (i = 0; i<N; i++)
        pthread_join(thread_id[i], NULL);//�ȴ�thread_id[i]�߳̽���

    return 0;
}


void* philospher(void *num)
{
    while (1)
    {
        int *i = (int*)num;
        sleep(1);//����Ƶ��
        takeFork(*i);//�������
        sleep(0);//������ǰ�߳�ִ�е�ʱ��Ƭ��������ŵ��ȴ�����֮�С�
        putFork(*i);//���²���
    }

    return nullptr;
}


void takeFork(int ph_num)
{
    sem_wait(&mutex);//���ź�����ֵ����һ����1��
    state[ph_num] = HUNGRY;//��ʼ��״̬ΪHUNGRY������
    printf("Philosopher %d want to eat\n", ph_num + 1);
    test(ph_num);//���Ժ���
    sem_post(&mutex);//���ź�����ֵ����һ����1��
    sem_wait(&S[ph_num]);//���ź�����ֵ����һ����1��
    sleep(1);//����Ƶ��
}


//******************�ܹ��Ծ͸ı��ź�����ֵ��״̬**********************************************
void test(int ph_num)
{    /*���state[ph_num]״̬ΪHUNGRY����state[LEFT]��state[RIGHT]��״̬��������EATING*/
    if (state[ph_num] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[ph_num] = EATING;//��state[ph_num]״̬��ΪEATING���Է���
        sleep(2);//����Ƶ��
        printf("Philosopher %d is eating now\n", ph_num + 1);
        sem_post(&S[ph_num]);//���ź�����ֵ����һ����1��

    }
}


//******************************************************************************************
void putFork(int ph_num)
{
    sem_wait(&mutex);//�ź�����1
    state[ph_num] = THINKING;//��state[ph_num]״̬��ΪTHINKING��˼����
    printf("Philosopher %d is thinking now\n", ph_num + 1);
    test(LEFT);
    test(RIGHT);
    sem_post(&mutex);//���ź�����ֵ����һ����1��
}

