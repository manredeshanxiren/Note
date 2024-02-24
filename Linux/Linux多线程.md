# 多线程

## 1.线程的概念

> 线程是CPU调度的基本单位，进程是系统资源分配的基本实体

- **Linux中线程的概念**

  Linux内核的设计者，复用PCB的结构体，用PCB模拟线程的TCB，很好的复用了进程的设计方案！

  所以说Linux内部没有真正意义上的线程，而是用进程方案模拟的线程。所以我们的每一个线程的PCB都会指向所属进程的地址空间的内核。

  对此有很多优势：复用代码和结构，实现简单，执行效率高。

运行一个简单的程序观察一下：

注意这个程序编写的时候`makefile`文件编译命令那行要加上`-lpthread`参数.

```cpp
#include<iostream>
#include<unistd.h>
#include<pthread.h>

using namespace std;

void* T1(void* args)
{
    while(1)
    {
        cout << "我是线程1,我正在运行" << getpid() << endl;
        sleep(1);
    }
}

void* T2(void* args)
{
    while(1)
    {
        cout << "我是线程2，我正在运行" << getpid() << endl;
        sleep(1);
    }
}

void* T3(void* args)
{
    while(1)
    {
        cout << "我是线程3，我正在运行" << getpid() << endl;
        sleep(1);
    }
}

int main()
{
    pthread_t t1, t2, t3;
    pthread_create(&t1, nullptr, T1, nullptr);
    pthread_create(&t2, nullptr, T3, nullptr);
    pthread_create(&t3, nullptr, T3, nullptr);

    while(1)
    {
        sleep(1);
    }
    return 0;
}

```

我们发现`ps -aL`命令将显示系统上所有用户的所有进程和线程的信息,我们可以看到线程的pid都是相同的，但是对应的LWP也就是对应的在`pthread`库中的线程ID,但是我们发现有个进程的LWP和对应的PID相同，那么我们可以看出这就是对应的进程，操作系统也是通过进程的`pid`来确认对应的进程的。

![image-20240224131840783](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240224131840783.png)

## 2.一些前置知识

①物理内存和磁盘之间

- 内存管理的本质：将磁盘中的特定的4KB的块(数据内容)，放入到哪一个物理内存的4KB空间(数据保存的空间)。
- 如果过多的IO，注定了过多的寻址，过多的机械运动，导致效率低下。所以OS在和磁盘这样的设备进行IO交互的时候，绝对不是按照字节单位，而是按照块单位