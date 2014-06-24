#include "threadcpp.h"


void generalFunc(int data) {
    std::cout << "Hello, C++11 thread\n" << "generalFunc data is: " << data << std::endl;
    //std::cout << "Hello, C++11 thread\n";
}

int threadcpp() {
    ThreadMain tm;
    std::thread t1(generalFunc, 1);        /*������ͨ����ָ��Ͳ���0��thread����t1*/
    std::thread t2(&ThreadMain::run, &tm);   /*����Ա�������ݸ�thread����t2�� ���Ҵ��ݵ��øú����Ķ����ָ��&tm*/
    std::thread t3(tm);     /*����һ�����������t3*/
    std::thread t4([]() { std::cout << "Hello, C++11 thread\n"; }); /*����lambda���ʽ��thread����t4*/
    /* ����join�����ȴ��߳���ֹ���������߳���ռ��Դ*/
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
