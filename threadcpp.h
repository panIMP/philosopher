#ifndef THREADCPP_H
#define THREADCPP_H

#include <thread>
#include <iostream>

class ThreadMain {
public:
    void operator()() {
        run();
    }
    void run() {
        std::cout << "Hello, C++11 thread\n";
    }
};

void generalFunc(int data);

int threadcpp();

#endif // THREADCPP_H
