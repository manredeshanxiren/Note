# Linux命名管道

## 1.概念

> - 管道应用的一个限制就是只能在具有共同祖先（具有亲缘关系）的进程间通信  
> - 如果我们想在不相关的进程之间交换数据，可以使用FIFO文件来做这项工作，它经常被称为命名管道  
> - 命名管道是一种特殊类型的文件  

## 2.创建一个命名管道

> - 命名管道可以从命令行上创建，命令行方法是使用下面这个命令  ：
>
>   `mkfifo filename`
>
>   ![image-20231128174723565](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231128174723565.png)
>
>     见见猪跑：
>
>   向管道写入：`echo hello > fifo`:我们发现程序卡住了，细节是我们查看fifo的文件的大小的时候，它却是0，因为我们知道管道是内存级文件，他的内容不会刷新到磁盘，所以我们看到的文件大小就是0，当我们`cat < fifo`也就是将fifo中的内容重定向输出到cat中打印出了`hello`。
>
> ![image-20231128193005159](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231128193005159.png)

那么上面的操作，是通过一个管道文件，让两个无关的进程实现的进程间的通信。那么我们知道进程间的通信就是要让两个进程看到同一个文件，那么这里我们是如何做到同一个文件呢？答案是路径，文件的唯一性就是由路径表示的！

---

那么我们到底是如何使用命名管道实现两个进程间的通信的：

> ①我们需要创建一个管道文件
>
> 这里我们需要用到一个系统接口`mkfifo`
>
> ![image-20231128204130327](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231128204130327.png)
>
> ```c
> #include<iostream>
> #include<sys/stat.h>
> #include<sys/types.h>
> #include<string.h>
> #include<cerrno>
> 
> #include "comm.hpp"
> 
> 
> using namespace std;
> int main()
> {
>  //1.创建管道文件，我们今天只需要创建一次
>  int n  = mkfifo(filename.c_str(),mode);
>  if(n != 0) //创建失败
>  {
>      cout << errno << " : "<< strerror(errno)<<  endl;
>      return 1;
>  }
> 
>  return 0;
> }
> ```
>
> 运行一下：
>
> 我们发现确实创建了文件：但是文件的权限却不是我们想要的`0666`，其实我们一下就能看出来原因是我们的`umask`不为零影响到了结果
>
> ![image-20231128204311535](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231128204311535.png)
>
> 我们查看一个操作系统的接口：`umask`
>
> ![image-20231128204715405](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231128204715405.png)
>
> ```c++
> #include<iostream>
> #include<sys/stat.h>
> #include<sys/types.h>
> #include<string.h>
> #include<cerrno>
> 
> #include "comm.hpp"
> 
> 
> using namespace std;
> int main()
> {
>  //1.创建管道文件，我们今天只需要创建一次
>  umask(0); //只影响当前进程的umask
>  int n  = mkfifo(filename.c_str(),mode);
>  if(n != 0) //创建失败
>  {
>      cout << errno << " : "<< strerror(errno)<<  endl;
>      return 1;
>  }
> 
>  return 0;
> }
> ```
>
> 运行一下看看：
> ![image-20231128204930740](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231128204930740.png)
>
> ②让读写端进程分别按照自己的需求打开文件
>
> ```c++
>     //1.不需要创建管道文件，我只需要打开对应的文件即可！
>     int wfd = open(filename.c_str(), O_WRONLY);
>     if(wfd < 0)
>     {
>         cerr << errno << " : "<< strerror(errno)<<  endl;
>         return 1;
>     }
>     
>     
>     
>     //1.创建管道文件，我们今天只需要创建一次
>     umask(0); //只影响当前进程的umask
>     int n  = mkfifo(filename.c_str(),mode);
>     if(n != 0) //创建失败
>     {
>         cout << errno << " : "<< strerror(errno)<<  endl;
>         return 1;
>     }
> ```
>
> ③开始通信
>
> ```c++
>    //3.开始通信
>     char buffer[NUM];
>     while(true)
>     {
>         buffer[0] = 0; //初始化第一个位置为\0
>         //读文件
>         ssize_t n = read(rfd, buffer, sizeof(buffer) - 1);
> 
>         if(n > 0)
>         {
>             //读成功了
>             buffer[n] = 0;
>             printf("%s\n", buffer);
>             fflush(stdout);
>         }
>         else if(n == 0)
>         {
>             //写端关闭
>             cout << "client quit, me too" << endl;
>             break;
>         }
>         else
>         {
>             //错误
>             cout << errno << " : "<< strerror(errno)<<  endl;
>             return 1;
>         }
>     }
> 
> 
> 
>     //2.开始通信
>     char buffer[NUM];
> 
>     while(true)
>     {
>         cout << "请输入要写入的信息#";
>         char* msg = fgets(buffer, sizeof(buffer), stdin);
>         assert(msg);
>         (void)msg;
> 
>         buffer[strlen(buffer) - 1] = 0;
> 
> 
>         //当我们写入quit的时候退出写端
>         if(strcasecmp(buffer, "quit") == 0) break;
> 
>         ssize_t n = write(wfd, buffer, strlen(buffer));
>         assert(n >= 0); 
>         (void)n;
>     }
> ```
>
> 运行结果：
>
> ![image-20231128223524668](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231128223524668.png)

## 3.匿名管道与命名管道的区别  

> - 匿名管道由pipe函数创建并打开。  
> - 命名管道由mkfifo函数创建，打开用open  
> - FIFO（命名管道）与pipe（匿名管道）之间唯一的区别在它们创建与打开的方式不同，一但这些工作完成之后，它们具有相同的语义  

## 4.我们实现一个即时的输入输出

就是我们输入管道的时候不用回车，而是选用输入一个字符就从管道输出：

我们要在`client.cc`端改变其向文件写入的方式，我们读取到字符c中，然后没输入一个字符就同步；

```c++
    while(true)
    {

        system("stty raw");  // 使终端驱动处于一次一字符模式
        char c = getchar();
        system("stty cooked");// 使终端驱动回到一次一行模式 

        ssize_t n = write(wfd, &c, sizeof(char));
        assert(n >= 0); 
        (void)n;
    }
```

运行结果：

![打印结果](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/%E6%89%93%E5%8D%B0%E7%BB%93%E6%9E%9C.gif)

## 5.完整代码

client.cc

```c++
#include<iostream>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<cerrno>
#include<cassert>
#include"comm.hpp"


using namespace std;
int main()
{
    //1.不需要创建管道文件，我只需要打开对应的文件即可！
    int wfd = open(filename.c_str(), O_WRONLY);
    if(wfd < 0)
    {
        cerr << errno << " : "<< strerror(errno)<<  endl;
        return 1;
    }

    //2.开始通信
    char buffer[NUM];

    while(true)
    {

        // cout << "请输入要写入的信息#";
        // char* msg = fgets(buffer, sizeof(buffer), stdin);
        // assert(msg);
        // (void)msg;

        // buffer[strlen(buffer) - 1] = 0;

        system("stty raw");  // 使终端驱动处于一次一字符模式
        char c = getchar();
        system("stty cooked");// 使终端驱动回到一次一行模式 


        // //当我们写入quit的时候退出写端
        // if(strcasecmp(buffer, "quit") == 0) break;

        ssize_t n = write(wfd, &c, sizeof(char));
        assert(n >= 0); 
        (void)n;
    }

    return 0;
}
```

server.cc

```c++
#include<iostream>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<cerrno>

#include "comm.hpp"


using namespace std;
int main()
{
    //1.创建管道文件，我们今天只需要创建一次
    umask(0); //只影响当前进程的umask
    int n = mkfifo(filename.c_str(),mode);
    if(n != 0) //创建失败
    {
        cout << errno << " : "<< strerror(errno)<<  endl;
        return 1;
    }

    cout << "创建文件成功！"<< endl;

    //2.打开文件
    int rfd = open(filename.c_str(), O_RDONLY);
    if(rfd < 0) //打开错误
    {
        cout << errno << " : "<< strerror(errno)<<  endl;
        return 2;
    }

    cout << "打开管道成功" << endl;

    //3.开始通信
    char buffer[NUM];
    while(true)
    {
        buffer[0] = 0; //初始化第一个位置为\0
        //读文件
        ssize_t n = read(rfd, buffer, sizeof(buffer) - 1);

        if(n > 0)
        {
            //读成功了
            printf("%c", buffer[0]);
            fflush(stdout);
        }
        else if(n == 0)
        {
            //写端关闭
            cout << "client quit, me too" << endl;
            break;
        }
        else
        {
            //错误
            cout << errno << " : "<< strerror(errno)<<  endl;
            return 1;
        }
    }


    //4.关闭文件
    close(rfd);

    unlink(filename.c_str());
    return 0;
}
```

comm.hpp

```c++
#pragma once

#include <iostream>
#include <string>


using namespace std;


#define NUM 65535 //管道缓冲区大小


string filename = "./fifo"; //文件名
mode_t mode = 0666; //打开方式

```

