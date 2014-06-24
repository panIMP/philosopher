#include "threadcpp.h"


void generalFunc(int data) {
    std::cout << "Hello, C++11 thread\n" << "generalFunc data is: " << data << std::endl;
    //std::cout << "Hello, C++11 thread\n";
}

int threadcpp() {
    ThreadMain tm;
    std::thread t1(generalFunc, 1);        /*传递普通函数指针和参数0给thread对象t1*/
    std::thread t2(&ThreadMain::run, &tm);   /*将成员函数传递给thread对象t2， 并且传递调用该函数的对象的指针&tm*/
    std::thread t3(tm);     /*传递一个函数对象给t3*/
    std::thread t4([]() { std::cout << "Hello, C++11 thread\n"; }); /*传递lambda表达式给thread对象t4*/
    /* 调用join函数等待线程终止，并回收线程所占资源*/
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
