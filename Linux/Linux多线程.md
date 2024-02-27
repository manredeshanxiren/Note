# 多线程

## 1.线程的概念

> 线程是CPU调度的基本单位，进程是系统资源分配的基本实体

- **Linux中线程的概念**

  Linux内核的设计者，复用PCB的结构体，用PCB模拟线程的TCB，很好的复用了进程的设计方案！

  所以说Linux内部没有真正意义上的线程，而是用进程方案模拟的线程。所以我们的每一个线程的PCB都会指向所属进程的地址空间的内核。因为执行流看到的资源是通过进程地址空间看到的，多个LWP看到的是同一个地址空间，所有的进程可能会共享进程的大部分资源！！

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

  a.其中块的大小为4KB

  b.文件系统+编译器的模式：文件(可执行程序+动态库)在磁盘的时候，就是以块的形式存储。

  c.操作系统+内存：内存在实际进行管理的时候，也是要以4KB为单位的

  d.局部性原理的特性：允许我们提前加载正在访问的数据的相邻或者附近的数据。

  e.数据预加载：我们会通过预先加载要访问的数据的附近的数据来减少IO的次数！多加载进来的数据本质：就叫做数据的预加载。

  f.为什么是4KB：IO的基本单位（内核内存+文件系统）都需要提供支持。通过局部性原理，预测未来的命中情况，提高效率。

②地址空间与物理内存之间：

- 虚拟地址(32位为例)：其中是按照10+10+12的方式来划分的；

  其中第一个10位代表一级页表的地址，我们可以通过它在一级页表中寻找找到对应的二级页表，二级页表中对应存储物理地址对应的页框，最后的12位作为对应的偏移量来寻找对应的块。

  这也证明了我们当时C语言学习的不管是什么类型它的地址都只有一个，但是我们对应的类型不可能都只占用一个字节，所以我们访问对应的空间都是基地址+偏移量的方式。

- malloc：我们实际在申请malloc内存的时候，OS只要给用户在虚拟内存上申请即可，当用户程序真正要访问的时候，OS才会申请或者填充页表+申请对应的物理内存。

- 字符型常量如何做到只允许被读取而不允许被修改？假设存在一个`const char* s = "hello wworld"`,s内部保存的是指向字符的虚拟起始地址---*s寻址的时候一定会存在虚拟地址向物理地址的转化----MMU + 查页表的方式 ----对当前的操作进行权限审查 ---- 虽然可以允许被找到，但是如果我们要进行写操作的话，这个操作是非法的 --- MMU会发生异常----OS识别异常 ---- 将异常转化为信号，发送给目标程序 ---- 在从内核转换成用户态的时候，进行信号处理 ----终止程序。

## 3.多线程的优缺点

### 3.1优点

> - 创建一个新线程的代价要比创建一个新进程小得多  
> - 与进程之间的切换相比，线程之间的切换需要操作系统做的工作要少很多  
> - 线程占用的资源要比进程少很多  
> - 能充分利用多处理器的可并行数量  
> - 在等待慢速I/O操作结束的同时，程序可执行其他的计算任务  
> - 计算密集型应用，为了能在多处理器系统上运行，将计算分解到多个线程中实现  
> - I/O密集型应用，为了提高性能，将I/O操作重叠。线程可以同时等待不同的I/O操作  

线程有这么多优点一定越多越好吗?不是，一定要合适（进程/线程CPU的个数/核数一致）

### 3.2缺点

> - 性能损失  
>
>   一个很少被外部事件阻塞的计算密集型线程往往无法与共它线程共享同一个处理器。如果计算密集型
>   线程的数量比可用的处理器多，那么可能会有较大的性能损失，这里的性能损失指的是增加了额外的
>   同步和调度开销，而可用的资源不变。  
>
> - 健壮性降低  
>
>   编写多线程需要更全面更深入的考虑，在一个多线程程序里，因时间分配上的细微偏差或者因共享了
>   不该共享的变量而造成不良影响的可能性是很大的，换句话说线程之间是缺乏保护的。  
>
> - 缺乏访问控制  
>
>   进程是访问控制的基本粒度，在一个线程中调用某些OS函数会对整个进程造成影响  
>
> - 编程难度提高  
>
>   编写与调试一个多线程程序比单线程程序困难得多  

多线程程序中，任何一个线程崩溃了，都会导致进程的崩溃！

为什么会这样？

系统角度：线程是进程的执行分支，线程崩溃了，就是进程崩溃了。

信号的角度：页表转换的时候, MMU识别写入权限的，没有验证通过，MMU异常->OS识别->给进程发信号。

```cpp
#include<iostream>
#include<unistd.h>
#include<pthread.h>

using namespace std;

int a[2];

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
    int cnt = 0;
    char *s = "hello bit";
    while(1)
    {
        cout << "我是线程3，我正在运行" << getpid() << endl;
        sleep(1);

        if(cnt == 5)  *s = 's';

        cnt++;
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

![image-20240224163613503](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240224163613503.png)

## 4.线程的控制

### 4.1线程的创建

Linux下没有真正意义的线程，而是用进程来模拟的线程(LWP)，--- 所以，Linux不会提供直接创建线程的系统调用，他会给我们提供更多轻量级的进程的接口

库：将Linux接口封装，对上给用户提供进行线程控制的接口----->用户级线程库---->pthread库，我们所有的操作系统都需要自带--->原生线程库。

用户视角：只认线程

```cpp
功能：创建一个新的线程
原型
int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *
(*start_routine)(void*), void *arg);
参数
thread:返回线程ID
attr:设置线程的属性，attr为NULL表示使用默认属性
start_routine:是个函数地址，线程启动后要执行的函数
arg:传给线程启动函数的参数
返回值：成功返回0；失败返回错误码
```
错误检查：

> - 传统的一些函数是，成功返回0，失败返回-1，并且对全局变量`errno`赋值以指示错误  
> - `pthreads`函数出错时不会设置全局变量`errno`（而大部分其他POSIX函数会这样做）。而是将错误代码通过返回值返回
> - `pthreads`同样也提供了线程内的`errno`变量，以支持其它使用`errno`的代码。对于`pthreads`函数的错误，建议通过返回值来判定，因为读取返回值要比读取线程内的`errno`变量的开销更小。  

案例一：

```cpp
#include<iostream>
#include<pthread.h>
#include<unistd.h>

using namespace std;


void* threadruning(void* args)
{
    while(true)
    {
        cout << "我是一个线程,我的self_id是" << pthread_self() << endl;

        sleep(1);
    }
}


int main()
{
    pthread_t t1; //pthread_t是一个无符号的长整数

    pthread_create(&t1, nullptr, threadruning, nullptr);

    while(1)
    {
        cout << "我是主线程,我的id是:" << t1 << endl; 
        sleep(1);
    }

    return 0;
}
```

运行结果：

我们发现LWP和我们程序内部打印出来的ID是不一样的。

![image-20240225092038720](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240225092038720.png)

案例二：

```cpp
#include<iostream>
#include<pthread.h>
#include<unistd.h>

#define NUM 10
using namespace std;



void* threadruning(void* args)
{
    char* name = (char*)args;
    while(true)
    {
        cout << "我是" << name <<",我的self_id是" << pthread_self() << endl;

        sleep(1);
    }
}


int main()
{
    pthread_t tids[NUM]; //pthread_t是一个无符号的长整数

    for(int i = 0 ; i < NUM; ++i)
    {
        char tname[64];
        snprintf(tname, 64, "thread-%d", i + 1);
        pthread_create(tids + i, nullptr, threadruning, tname);
    }




    while(1)
    {
        cout << "我是主线程" << endl; 
        sleep(2);
    }

    return 0;
}
```

运行结果：

原因是我们所有的线程是共享这一段缓冲区的，所以我们每个进程看到的都是同一块缓冲区。 

![image-20240225093543580](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240225093543580.png)

改进：我们将`tname`的空间编程动态内存空间即可，相当于每个线程都有自己单独的`tname`

```cpp
//....
int main()
{
    pthread_t tids[NUM]; //pthread_t是一个无符号的长整数

    for(int i = 0 ; i < NUM; ++i)
    {
        char* tname = new char[64];
        snprintf(tname, 64, "thread-%d", i + 1);
        pthread_create(tids + i, nullptr, threadruning, tname);
    }
   //...
```

运行结果：

![image-20240225095436604](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240225095436604.png)

### 4.2线程的退出

> ①主线程退出，那么我们对应的所有的线程也都退出。
>
> ②单个线程如果出现除零，野指针问题导致线程崩溃，进程也会随着崩溃 
>
> ③线程是进程的执行分支，线程出异常，就类似进程出异常，进而触发信号机制，终止进程，进程终止，该进程内的所有线程也就随即退出 

### 4.3线程的等待

> 为什么线程需要等待
>
> - 已经退出的线程，其空间没有被释放，仍然在进程的地址空间内  
> - 创建新的线程不会复用刚才退出线程的地址空间  

```cpp
功能：等待线程结束
原型
int pthread_join(pthread_t thread, void **value_ptr);
参数
thread:线程ID
value_ptr:它指向一个指针，后者指向线程的返回值
返回值：成功返回0；失败返回错误码
```

> 线程终止的三种情况：
>
> - 线程函数执行完毕
> - `pthread_exit(void*)`;
> - 一个线程可以调用`pthread_ cancel`终止同一进程中的另一个线程  

代码示例：

```cpp
#include<iostream>
#include<pthread.h>
#include<unistd.h>

#define NUM 10
using namespace std;



void* threadruning(void* args)
{
    char* name = (char*)args;
    while(true)
    {
        cout << "我是" << name <<",我的self_id是" << pthread_self() << endl; 
        //打印线程的id,在线程自己内部调用：pthread_self

        sleep(3);

        break;
    }

    delete name;

    return nullptr; 
}


int main()
{
    pthread_t tids[NUM]; //pthread_t是一个无符号的长整数

    for(int i = 0 ; i < NUM; ++i)
    {
        char* tname = new char[64];
        snprintf(tname, 64, "thread-%d", i + 1);
        pthread_create(tids + i, nullptr, threadruning, tname);
    }

    for(int i = 0; i < NUM; ++i)
    {
        int n = pthread_join(tids[i], nullptr);
        if(n != 0) cout << "pthread_join error" << endl;
    }

    // while(1)
    // {
    //     cout << "我是主线程" << endl; 
    //     sleep(2);
    // }

    return 0;
}
```

### 4.4线程的取消

> `pthread_cancel(pthread_t)`接口：
>
> 当一个进程开始运行的时候，我们可以调用这个接口，将其取消；
>
> ```cpp
> #include <iostream>
> #include <string>
> #include <unistd.h>
> #include <pthread.h>
> #include <ctime>
> 
> using namespace std;
> 
> void *threadRun(void* args)
> {
>     const char*name = static_cast<const char *>(args);
> 
>     int cnt = 5;
>     while(cnt)
>     {
>         cout << name << " is running: " << cnt-- << " obtain self id: " << pthread_self() << endl;
>         sleep(1);
>     }
> 
>     pthread_exit((void*)11); 
> 
>     // PTHREAD_CANCELED; #define PTHREAD_CANCELED ((void *) -1)
> }
> 
> int main()
> {
>     pthread_t tid;
>     pthread_create(&tid, nullptr, threadRun, (void*)"thread 1");
>      sleep(3);
> 
>      pthread_cancel(tid);
> 
>     void *ret = nullptr;
>     pthread_join(tid, &ret);
>     cout << " new thread exit : " << (int64_t)ret << "quit thread: " << tid << endl;
>     return 0;
> }
> ```

### 4.5线程的分离

> 一个线程如果被分离，就无法join，如果join的话，就会报错。
>
> - 默认情况下，新创建的线程是`joinable`的，线程退出后，需要对其进行`pthread_join`操作，否则无法释放资源，从而造成系统泄漏  
> - 如果不关心线程的返回值，`join`是一种负担，这个时候，我们可以告诉系统，当线程退出时，自动释放线程资源  
>
> ```cpp
> int pthread_detach(pthread_t thread);
> ```
>
> 可以是线程组内其他线程对目标线程进行分离，也可以是线程自己分离:  
>
> ```cpp
> pthread_detach(pthread_self());
> ```
>
> **joinable和分离是冲突的，一个线程不能既是joinable又是分离的。**   
>
> ```cpp
> #include<iostream>
> #include<pthread.h>
> #include<unistd.h>
> #include<cstring>
> 
> using namespace std;
> 
> 
> void* runing(void* args)
> {
>     pthread_detach(pthread_self());
>     while(1)
>     {
>         cout << "我是线程,我正在运行....." << endl;
>         sleep(1);
> 
>         break;
>     }
> 
>     return nullptr;
> }
> 
> 
> int main()
> {
>     pthread_t t1;
> 
>     pthread_create(&t1, nullptr, runing, nullptr);
> 
>     pthread_detach(t1);
>     
>     int n = pthread_join(t1, nullptr);
> 
>     if(n != 0)
>     {
>         cerr <<"error:" << n <<":"<< strerror(n) << endl;
>     }
> 
> 
>     return 0;
> }
> ```
>
> ![image-20240225202252894](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240225202252894.png)

### 4.6线程的用途

> - 合理的使用多线程，能提高CPU密集型程序的执行效率  
> - 合理的使用多线程，能提高IO密集型程序的用户体验（如生活中我们一边写代码一边下载开发工具，就是多线程运行的一种表现）  

## 5.线程ID及进程地址空间布局  

> - `pthread_ create`函数会产生一个线程ID，存放在第一个参数指向的地址中。该线程ID和前面说的线程ID不是一回事  
>
> - 前面讲的线程ID属于进程调度的范畴。因为线程是轻量级进程，是操作系统调度器的最小单位，所以需要一个数值来唯一表示该线程。
>
> - `pthread_ create`函数第一个参数指向一个虚拟内存单元，该内存单元的地址即为新创建线程的线程ID，属于NPTL线程库的范畴。线程库的后续操作，就是根据该线程ID来操作线程的  
>
> - 线程库NPTL提供了`pthread_ self`函数，可以获得线程自身的ID： 
>
>   ```cpp
>   pthread_t pthread_self(void);
>   ```
>
>   `pthread_t`到底是什么类型呢？取决于实现。对于Linux目前实现的NPTL实现而言，`pthread_t`类型的线程ID，本质就是一个进程地址空间上的一个地址.
>
>   **这里体现了每一个线程都有自己独立的栈结构，主线程用的是系统栈。**
>
>   ![image-20240225202809268](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240225202809268.png)
>
> - __thread参数：
>
>    __thread是GCC内置的线程局部存储设施，存取效率可以和全局变量相比。__thread变量每一个线程有一份独立实体，各个线程的值互不干扰。**可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量**。‘
>
>   ```cpp
>   #include<iostream>
>   #include<pthread.h>
>   #include<unistd.h>
>   #include<cstring>
>               
>   using namespace std;
>               
>   __thread int g_val = 100;
>               
>   //int g_val = 100;
>               
>   std::string hexAddr(pthread_t tid)
>   {
>       g_val++;
>       char buffer[64];
>       snprintf(buffer, sizeof(buffer), "0x%x", tid);
>               
>       return buffer;
>   }
>               
>   void *threadRoutine(void* args)
>   {
>       // static int a = 10;
>       string name = static_cast<const char*>(args);
>       int cnt = 5;
>       while(cnt)
>       {
>           sleep(1);
>           cout << name << " g_val: " << g_val++ << ", &g_val: " << &g_val << endl;
>                       
>       }
>       return nullptr;
>   }
>               
>   int main()
>   {
>       pthread_t t1, t2, t3;
>       pthread_create(&t1, nullptr, threadRoutine, (void*)"thread 1"); // 线程被创建的时候，谁先执行不确定！
>       pthread_create(&t2, nullptr, threadRoutine, (void*)"thread 2"); // 线程被创建的时候，谁先执行不确定！
>       pthread_create(&t3, nullptr, threadRoutine, (void*)"thread 3"); // 线程被创建的时候，谁先执行不确定!
>               
>       pthread_join(t1, nullptr);
>       pthread_join(t2, nullptr);
>       pthread_join(t3, nullptr);
>               
>       return 0;
>   }
>               
>   ```
>
>   运行结果：
>
>   ![image-20240225203936942](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240225203936942.png)

## 6.线程的同步和互斥

> - 临界资源：多线程执行流共享的资源就叫做临界资源  
> - 临界区：每个线程内部，访问临界资源的代码，就叫做临界区  
> - 互斥：任何时刻，互斥保证有且只有一个执行流进入临界区，访问临界资源，通常对临界资源起保护作用  
> - 原子性（后面讨论如何实现）：不会被任何调度机制打断的操作，该操作只有两态，要么完成，要么未完成  

### 6.1互斥量mutex

> - 大部分情况，线程使用的数据都是局部变量，变量的地址空间在线程栈空间内，这种情况，变量归属单个线程，其他线程无法获得这种变量  
> - 但有时候，很多变量都需要在线程间共享，这样的变量称为共享变量，可以通过数据的共享，完成线程之间的交互 
> - 多个线程并发的操作共享变量，会带来一些问题  
>
> **两种初始化方式：**
>
> - 局部：
>
>   需要调用对应的初始化函数，并且需要对mutex进行destroy
>
>   ```cpp
>   pthread_mutex_init(&mutex, nullptr);
>   ```
>
> - 全局：
>
>   我们直接使用这个宏进行初始化，而不需要调用对应的初始化函数。同时这种方式不需要对mutex进行destroy
>
>   ```cpp
>   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
>   ```
>
> **细节：**
>
> - 凡是访问同一个临界资源的线程，都要进行加锁保护，而且必须加同一把锁，这个是一个游戏规则，不能有例外
>
> - 每一个线程访问临界区之前，得加锁，加锁本质是给 临界区 加锁，加锁的粒度尽量要细一些
>
> - 线程访问临界区的时候，需要先加锁->所有线程都必须要先看到同一把锁->锁本身就是公共资源->锁如何保证自己的安全？
>
>   -> 加锁和解锁本身就是原子的！
>
> - 临界区可以是一行代码，可以是一批代码，
>
>   a. 线程可能被切换吗？当然可能， 不要特殊化加锁和解锁，还有临界区代码。
>
>   b. 切换会有影响吗？不会，因为在我不在期间，任何人都没有办法进入临界区，因为他无法成功的申请到锁！因为锁被我拿走了！
>
> - 这也正是体现互斥带来的串行化的表现，站在其他线程的角度，对其他线程有意义的状态就是：锁被我申请(持有锁)，锁被我释放了(不持有锁)， 原子性就体现在这里。
>
> - 解锁的过程也被设计成为原子的！
>
> 锁的原理的理解：
>
> - swap和exchange指令，都是将寄存器的值和内存单元中的值进行交换。
>
> - 是调用的线程来执行对应的加锁和解锁代码的。
>
> - 寄存器的硬件只有一套，但是寄存器中的数据是每一个线程都需要的。
>
>   寄存器 != 寄存器的内容(执行流的上下文)；
>
> 从汇编的角度来理解，起始lock和unlock在汇编语句中就只有一句：
>
> `xchgb %al， mutex`(将寄存器al中的值和mutex中的值进行交换)和`movb $1,mutex`（向mutex中放入数字1）；
>
> 假设现在存在两个线程threada和threadb，同时来竞争当前的这个锁mutex(存储在内存中)
>
> 首先mutex初始化为1，`threada`先执行， 先将mutex中的值和寄存器交换，那么这时候mutex中的值为0，寄存器al中的值为1；那么对应的if条件判断就为真，就直接return，`threada`就会去执行对应的代码或者被时间片到了，被调度。那么这时候`threada`来的时候执行同样的寄存器和mutex值交换的时候，因为这时候寄存器和mutex的值都是0，那么交换完成，在if判断的时候为假，就进入了挂起等待。这时候其实已经达到了加锁的目的，就是我们的临界区只能同时有一个线程在运行。当`threada`执行对应的临界区代码，然后unlock，这时候将mutex中的值修改为1，这是的`threada`再次判断的时候就可以去执行对应的临界区代码了。
>
> ![image-20240226121504090](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240226121504090.png)

### 6.3买票案例

- 没有加锁的情况

  ```cpp
  #include<iostream>
  #include<pthread.h>
  #include<unistd.h>
  #include<cstring>
  
  using namespace std;
  
  int ticket = 1000;
  
  
  void* ThreadRoutine(void* args)
  {
      string name = static_cast<char*>(args);
  
      while(true)
      {
  
          if(ticket > 0)
          {
              usleep(1000);
              cout << "我是" << name << "还剩" << ticket-- << "张票" << endl;
          }
  
  
          usleep(500);
      }
  
      return nullptr;
  }
  
  int main()
  {
  
      pthread_t t[4];
  
      int n = sizeof(t) / sizeof(t[0]);
  
      for(int i = 0; i < n; ++i)
      {
          char* date = new char[64];
          snprintf(date, 64, "thread%d", i + 1);
          pthread_create(t + i, nullptr, ThreadRoutine, date);
      }
  
  
      for(int i = 0; i < n; ++i)
      {
          pthread_join(t[i], nullptr);
      }
  
      return 0;
  }
  ```

  运行结果：

  ![image-20240225210501300](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240225210501300.png)

- 加锁

  ```cpp
  #include<iostream>
  #include<pthread.h>
  #include<unistd.h>
  #include<cstring>
  
  using namespace std;
  
  int ticket = 1000;
  
  pthread_mutex_t mutex; // 后面说
  
  void* ThreadRoutine(void* args)
  {
      string name = static_cast<char*>(args);
  
      while(true)
      {
          pthread_mutex_lock(&mutex);
          if(ticket > 0)
          {
              usleep(1000);
              cout << "我是" << name << "还剩" << ticket-- << "张票" << endl;
              pthread_mutex_unlock(&mutex);
          }
          else 
          {
              pthread_mutex_unlock(&mutex);
          }
          usleep(500);
      }
  
      return nullptr;
  }
  
  int main()
  {
  
      pthread_t t[4];
  
      pthread_mutex_init(&mutex, nullptr);
  
      int n = sizeof(t) / sizeof(t[0]);
  
      for(int i = 0; i < n; ++i)
      {
          char* date = new char[64];
          snprintf(date, 64, "thread%d", i + 1);
          pthread_create(t + i, nullptr, ThreadRoutine, date);
      }
  
  
      for(int i = 0; i < n; ++i)
      {
          pthread_join(t[i], nullptr);
      }
  
      pthread_mutex_destroy(&mutex);
  
      return 0;
  }
  ```

  运行结果：

  ![image-20240225210909537](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240225210909537.png)
  
  那么上面的案例是一个全局锁的案例，我们实现一个将锁声明在主函数内部的案例：
  
  这里我们使用了一个`Tdata`的类将线程的name和mutex一并存储在类的属性中，那么我们在传给线程的进入函数的时候只需要将`Tdata`类传入这个函数即可。
  
  ```cpp
  #include<iostream>
  #include<pthread.h>
  #include<unistd.h>
  #include<cstring>
  
  using namespace std;
  
  
  int ticket = 10000;
  
  //pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  
  class Tdata
  {
      public:
      Tdata(char* name, pthread_mutex_t mutex):_name(name),_mutex(mutex)
      {}
  
      ~Tdata()
      {}
  
      public:
      char* _name;
      pthread_mutex_t _mutex;
  };
  
  void* ThreadRun(void* args)
  {
      Tdata* Td = static_cast<Tdata*>(args);
      while(1)
      {
          usleep(100);
          pthread_mutex_lock(&Td->_mutex);
          if(ticket > 0)
          {
              cout << "我是" << Td->_name << "我买票了,还有" << --ticket <<"张票" << endl;
              pthread_mutex_unlock(&Td->_mutex);
          }
          else 
          {
              pthread_mutex_unlock(&Td->_mutex);
              break;
          }
          usleep(100);
      }      
      return nullptr;
  }
  
  
  int main()
  {
  
      pthread_t tids[4];
      pthread_mutex_t mutex;
  
      for(int i = 0; i < 4; ++i)
      {
          char* name = new char[64];
          snprintf(name, 64, "thread:%d", i + 1);
          Tdata* tdata = new Tdata(name, mutex);
          pthread_create(tids + i, nullptr, ThreadRun, (void*)tdata);
      }
  
  
      for(int i = 0; i < 4; ++i)
      {
          pthread_join(tids[i], nullptr);
      }
  
      cout << "all pthread quit successed" << endl;
      while(1)
      {
          sleep(1);
      }
      return 0;
  }
  ```
  
  运行截图：
  
  ![image-20240226115840923](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240226115840923.png)

### 6.4demo版线程封装

```cpp
#pragma once

class Thread
{

public:
    typedef enum
    {
        NEW = 0,
        RUNING,
        EXIT
    }ThreadStatus;

    typedef void* (*func_t)(void*);

    Thread(int mum, func_t func, void* args) :_tid(0), _status(NEW), _func(func),_args(args) 
    {
        char name[128];
        snprintf(name, sizeof name, "thread-%d", _tid);
        _name = name;
    }

    int status() {  return _status; }
    string thread_name(){   return _name; }

    pthread_t threadid()
    {
        if(_status == RUNING)
        {
            return _tid;
        }

        else return 0;
    }

    static void* runHelper(void* args)
    {
        Thread* ts = (Thread*) args; //通过这种方式拿到对象
        (*ts)();
        return nullptr;
    }

    void operator()()
    {
        if(_func != nullptr) _func(_args); //使用仿函数的方式运行对应的线程进入函数
    }

    void run()
    {
        int n = pthread_create(&_tid, nullptr, runHelper, this);
        if(n != 0) exit;
        _status = RUNING;
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);

        if(n != 0)
        {
            cerr << "main thread join error:"  << _name << endl;
            return ;
        }
        _status = EXIT;
    }

    ~Thread()
    {}


public:
    pthread_t _tid;
    string _name;
    func_t _func; //线程未来要执行的回调
    void* _args;
    ThreadStatus _status;
};
```

### 6.5demo版锁的封装

```cpp
#pragma

class Mutex
{
public:
    Mutex(pthread_mutex_t* pthread) : _pthread(pthread)
    {}

    void lock()
    {
        pthread_mutex_lock(_pthread);
    }

    void unlock()
    {
        pthread_mutex_unlock(_pthread);
    }

    ~Mutex()
    {}
private:
    pthread_mutex_t * _pthread;

};


class lockGuard
{
public:

    lockGuard(pthread_mutex_t* mutex) :_mutex(mutex)
    {
        _mutex.lock();
    }

    ~lockGuard()
    {
        _mutex.unlock();
    }

private:
    Mutex _mutex;
};
```

## 7.常见锁的概念

### 7.1死锁

> - 死锁是指在一组进程中的各个进程均占有不会释放的资源，但因互相申请被其他进程所站用不会释放的资源而处于的一种永久等待状态  

### 7.2死锁四个必要条件

> - 互斥条件：一个资源每次只能被一个执行流使用  
> - 请求与保持条件：一个执行流因请求资源而阻塞时，对已获得的资源保持不放  
> - 不剥夺条件:一个执行流已获得的资源，在末使用完之前，不能强行剥夺  
> - 循环等待条件:若干执行流之间形成一种头尾相接的循环等待资源的关系  

### 7.3避免死锁

> - 破坏死锁的四个必要条件
> - 加锁顺序一致
> - 避免锁未释放的场景
> - 资源一次性分配

## 8.Linux线程同步

### 8.1条件变量

> - 当一个线程互斥地访问某个变量时，它可能发现在其它线程改变状态之前，它什么也做不了  
> - 例如一个线程访问队列时，发现队列为空，它只能等待，只到其它线程将一个节点添加到队列中。这种情况就需要用到条件变量  

**条件变量的接口**

> - 条件变量函数 初始化
>
>   ```cpp
>   int pthread_cond_init(pthread_cond_t *restrict cond,const pthread_condattr_t *restrictattr);
>   参数：
>   cond：要初始化的条件变量
>   attr：NULL
>   ```
>
> - 销毁
>
>   ```cpp
>   int pthread_cond_destroy(pthread_cond_t *cond)
>   ```
>
> - 等待条件满足
>
>   ```cpp
>   int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex);
>   参数：
>   cond：要在这个条件变量上等待
>   mutex：互斥量，后面详细解释
>   ```
>
> - 唤醒等待
>
>   ```cpp
>   int pthread_cond_broadcast(pthread_cond_t *cond);  //全局唤醒
>   int pthread_cond_signal(pthread_cond_t *cond); //单独唤醒
>   ```

**demo**

```cpp
#include<iostream>
#include<pthread.h>
#include<unistd.h>


using namespace std;

const int num = 5;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //全局变量的锁
pthread_cond_t cond = PTHREAD_COND_INITIALIZER; //条件变量的初始化


void* active(void* args)
{
    char* name = (char*) args;

    while(1)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        cout << "我是线程：" << name << endl;
        pthread_mutex_unlock(&mutex);

    }
}

int main()
{

    pthread_t tids[num];

    for(int i = 0 ; i < num; ++i)
    {
        char* name = new char[64];
        snprintf(name, 64, "thread-%d", i +  1);
        pthread_create(tids + i, nullptr, active, (void*)name);
    }

    pthread_cond_signal(&cond);
    //pthread_cond_broadcast(&cond);

    for(int i = 0 ; i < num; ++i)
    {
        pthread_join(tids[i], nullptr);
    }

    return 0;
}
```

**生产者消费者模型**

基于`blockqueue`实现的多生产者消费者模型

三种关系：

- 生产者和生产者--- 互斥关系

- 消费者和消费者---互斥关系
- 生产者和消费者---同步关系和互斥关系

两个角色：

- 生产者
- 消费者

一个交易场所：

- 通常是缓冲区

细节：

- 一定要保证，在任何时候，都是符合条件，才进行生产
- 要让线程进行休眠等待，不能持有锁等待！
- 当线程醒来的时候，注定了继续从临界区内部继续运行！因为我是在临界区被切走的！
- 注定了当线程被唤醒的时候，继续在`pthread_cond_wait`函数出向后运行，又要重新申请锁，申请成功才会彻底返回

为何这样的方式高效？

因为这样的`blockqueue`的方式允许生产消费的步调可以不一致，也就是说提供了一种特定的缓冲区。

从某种程度上来说允许了多个消费者的并行，多个生产者的并行。

```cpp
//main.cc
#include<iostream>
#include<queue>
#include<pthread.h>
#include<mutex>
#include<unistd.h>
#include<ctime>

using namespace std;

#include"blockqueue.hpp"
#include"Task.hpp"

void* Consumer(void* args)
{
    BlockQueue<Task> *q = static_cast<BlockQueue<Task> *>(args);

    while(true)
    {
        // 1.从q中拿出任务
        Task t;
        q->pop(&t);

        t(); // 内部写仿函数用于计算任务

        cout << pthread_self() << "| consumer data : " << t.formatArgs() << t.formatRes() << endl;
    }


}

void* Product(void* args)
{
    BlockQueue<Task> *q = static_cast<BlockQueue<Task> *>(args);
    string opers = "+-*/%";
    while(true)
    {

        int x = rand() % 20 + 1;
        int y = rand() % 10 + 1;
        char op = opers[rand() % opers.size()];
        Task t(x, y, op);

        q->push(t);

        cout << pthread_self() << "| Product Task: " << t.formatArgs() << "?"<< endl;
    }

}

int main()
{
    srand((uint64_t)time(nullptr) % getpid());

    BlockQueue<Task>* bq = new BlockQueue<Task>(); 

    pthread_t c[2], p[3];

    pthread_create(&c[0], nullptr, Consumer, bq);
    pthread_create(&c[1], nullptr, Consumer, bq);
    pthread_create(&p[0], nullptr, Product, bq);
    pthread_create(&p[1], nullptr, Product, bq);
    pthread_create(&p[2], nullptr, Product, bq);


    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);
    pthread_join(p[2], nullptr);

    delete bq;

    return 0;
}

//Task.hpp

class Task
{
public:
    Task()
    {}

    Task(int x, int y, char op):_x(x), _y(y), _op(op), _result(0), _exitcode(0)
    {}

    void operator()()
    {
        switch(_op)
        {
            case '+':
                _result = _x + _y;
                break;
            case '-':
                _result = _x - _y;
                break;
            case '*':
                _result = _x * _y;
                break;
            case '/':
                {
                    if(_y == 0) 
                    _exitcode = -1; //表示除零错误
                    else 
                    _result = _x /_y;
                }    
                break;                  
            case '%':
                {
                    if(_y == 0) 
                    _exitcode = -1; //表示除零错误
                    else 
                    _result = _x % _y;
                }  
            default:   
                break;            
        }
    }
    string formatArgs()
    {
        return to_string(_x) + _op + to_string(_y) + "=";
    }

    string formatRes()
    {
        return to_string(_result) + '(' + to_string(_exitcode) + ')';
    }
    ~Task()
    {

    }

private:
    int _x;        //运算数
    int _y;        //运算数
    char _op;      //操作符
    int _result;   //存储计算结果
    int _exitcode; //退出码
};

//blockqueue.hpp
template<class T>
class BlockQueue
{
public:
    BlockQueue(int cap = 5):_cap(cap)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_ConsumerCond, nullptr);
        pthread_cond_init(&_ProductCond,nullptr);
    }
    bool isfull(){  return _q.size() == _cap; }
    bool isempty(){ return _q.empty(); }

    void push(const T data)
    {
        pthread_mutex_lock(&_mutex);
        while(isfull()) //用while的原因是可以保证防止被误唤醒的情况
        {
            pthread_cond_wait(&_ConsumerCond, &_mutex);
        }

        _q.push(data);

        pthread_cond_signal(&_ProductCond);
        pthread_mutex_unlock(&_mutex);
    }

    void pop(T* out)
    {
        pthread_mutex_lock(&_mutex);
        while(isempty())
        {
            pthread_cond_wait(&_ProductCond, &_mutex);
        }

        *out = _q.front();
        _q.pop();
        pthread_cond_signal(&_ConsumerCond);

        pthread_mutex_unlock(&_mutex);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_ConsumerCond);
        pthread_cond_destroy(&_ProductCond);
    }

private:
    queue<T> _q;
    int _cap;
    pthread_mutex_t _mutex;
    pthread_cond_t _ConsumerCond;
    pthread_cond_t _ProductCond;

};
```

### 8.2同步的概念与竞态条件

> - 同步：在保证数据安全的前提下，让线程能够按照某种特定的顺序访问临界资源，从而有效避免饥饿问题，叫做同步  
> - 竞态条件：因为时序问题，而导致程序异常，我们称之为竞态条件。在线程场景下，这种问题也不难理解

## 9.POSIX信号量

> POSIX信号量和SystemV信号量作用相同，都是用于同步操作，达到无冲突的访问共享资源目的。 但POSIX可以用于线程间同步。  
>
> - 用于描述临界资源中资源数目的
> - 每一个线程，在访问对应的资源的时候，线程申请信号量，申请成功，表示该线程允许使用该资源，申请不成功，目前无法使用该资源！
> - 信号量的工作机制：信号量机制类似于我们看电影买票，是一种资源的预定机制。
> - 信号量已经是资源的计数器了，申请信号量成功，本身就表明资源可用！申请信号量失败的本身表明资源不可用---本质就是把判断转化为信号量的申请行为！

### 9.1接口

- 初始化信号量

  ```cpp
   #include <semaphore.h>
    int sem_init(sem_t *sem, int pshared, unsigned int value);
    参数：
    pshared:0表示线程间共享，非零表示进程间共享
    value：信号量初始值
  ```

- 销毁信号量

  ```
  int sem_destroy(sem_t *sem);
  ```

- 等待信号量

  ```cpp
  功能：等待信号量，会将信号量的值减1
  int sem_wait(sem_t *sem); //P()
  ```

- 发布信号量

  ```cpp
  功能：发布信号量，表示资源使用完毕，可以归还资源了。将信号量值加1。
  int sem_post(sem_t *sem);//V()
  ```

### 9.2基于环形队列的生产消费模型  

> - 环形队列采用数组模拟，用模运算来模拟环状特性  
>
>   ![image-20240227195538957](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240227195538957.png)
>
> - 环形结构起始状态和结束状态都是一样的，不好判断为空或者为满，所以可以通过加计数器或者标记位来判断满或者空。另外也可以预留一个空的位置，作为满的状态  
>
>   ![image-20240227195552171](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240227195552171.png)
>
> - 细节：
>
>   a.因为只有为空和为满的情况，cp才会指向同一个位置
>
>   b.其他情况，cp可以并发运行
>
>   c.我们要保证游戏规则，同时也要保证满的时候的策略问题
>
>   d.空的时候，生产者先运行，满的时候，消费者先运行。
>
>   e.不能让生产者套圈消费者，不能让消费者超过生产者。
>

## 7.进程vs线程

### 7.1进程和线程

> - 进程是资源分配的基本单位  
>
> - 线程是调度的基本单位  
>
> - 线程共享进程数据，但也拥有自己的一部分数据：
>
>   a.线程ID(LWP)
>
>   **b.一组寄存器:线程的上下文**,具有动态切换的概念。
>
>   **c.栈**：线程在处理临时数据和临时空间的时候，必须有自己的栈帧结构。**因为我们线程在执行对应的入口函数的时候，要生成对应的临时变量**，那么这时候线程就需要自己独立的栈帧，但是因为我们线程都是公用一个地址空间，一个进程地址空间只有一个栈，那么如果我们每个线程都将自己的临时数据和临时变量放入到这个栈就会造成混乱OS分辨不清楚，所以线程在处理临时数据，临时变量的时候需要自己独立的栈结构。
>
>   d.信号屏蔽字
>
>   e.调度优先级:私有属性
>
>   f. `errno`
>
> - 线程所共享的资源：
>
>   进程的多个线程共享 同一地址空间,因此Text Segment、Data Segment都是共享的,如果定义一个函数,在各线程中都可以调用,如果定义一个全局变量,在各线程中都可以访问到,除此之外,各线程还共享以下进程资源和环境:  
>   a.**文件描述符**：主线程打开文件，对应的线程也是可见的。
>
>   b.每种信号的处理方式(SIG_IGN, SIG_DFL或者自定义的信号处理函数)
>
>   c.当前的工作目录
>
>   d.用户ID和组ID



