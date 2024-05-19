#include "thread.h"

Thread::Thread()
{

}

void Thread::run()
{
    for(int i = 0; i < 10; ++i)
    {
        sleep(1);
        //发送一个信号,通知主线程
        emit notify();
    }
}

