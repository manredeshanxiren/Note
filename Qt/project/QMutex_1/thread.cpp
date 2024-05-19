#include "thread.h"

//定义
int Thread::num = 0;
QMutex Thread::mutex;
Thread::Thread()
{

}

void Thread::run()
{
    for(int i = 0; i < 500000; ++i)
    {
        mutex.lock();
        ++num;
        mutex.unlock();
    }
}
