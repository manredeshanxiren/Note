# Linux进程控制(2)

##  1.进程等待(续)

我们稍微改造一下，之前进程等待的时候，父进程不要阻塞等待的代码，让父进程真正的去运行一些任务。

我们采用函数回调的方式，让父进程在等待子进程的时候也可以去运行自己的一些任务！

```c
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

#define TASK_NUM 10


//预设一批任务
void sync_disk()
{
  printf("这是一个刷新数据的任务\n");
}

void sync_log()
{
  printf("这是一个同步日志的任务\n");
}

void net_send()
{
  printf("这是一个网络发送的任务\n");
}                                                                                                                                                            


//保存相关的任务
typedef void (*func_t)(); //定义了一个函数指针类型
func_t orther_task[TASK_NUM] = {NULL}; 

//装载任务
int Load_Task(func_t fuc)
{
  int i = 0;
  for(; i < TASK_NUM; ++i)
  {
    if(orther_task[i] == NULL) break;
  }

  if(TASK_NUM == i) return -1;
  else orther_task[i] = fuc;

  return 0;
}

//初始化函数指针数组
void Init_Task()
{
  for(int i = 0; i < TASK_NUM; ++i) orther_task[i] = NULL;
  
  Load_Task(sync_disk);  
  Load_Task(sync_log);
  Load_Task(net_send);
}

void Run_Task()
{
  for(int i = 0; i < TASK_NUM; ++i)
  {
    if(orther_task[i] == NULL) continue;
    else orther_task[i]();
  }
}

int main()
{
    pid_t id = fork();
                                                                                                                                                             
    if(id == 0)
    {
      //子进程
      int cnt = 5;
      while(cnt)
      {
        printf("我是子进程，我还活着呢，我还有%dS，我的pid：%d，我的ppid：%d\n", cnt--, getpid(), getppid());
        sleep(1);
      }
      exit(0);
    }

    Init_Task();

    while(1)
    {

      int status = 0;
      pid_t ret_id = waitpid(id, &status, WNOHANG);// 夯住了

      if(ret_id < 0)
      {
        printf("waitpid_error\n");
      }
     else if(ret_id == 0)
      {
        Run_Task();                                                                                                                                          
        sleep(1);
        continue;
      }
      else
      {

      printf("我是父进程，我等待成功了，我的pid：%d，我的ppid：%d, ret_id: %d, child exit code: %d, child exit signal:%d\n",getpid(), getppid(), ret_id, (status >> 8)&0xFF, status & 0x7F);
        exit(0);
      }
      sleep(1); 
    }
  return 0;
}

```

运行结果：

![image-20231109151934831](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109151934831.png)

继续改进，我们之前获取进程退出码的时候是使用`(status >> 8)& 0xFF`的方式来进行获取的，那么实际上C库也给我们提供了两个宏来帮助我们获取进程的退出码：

```c
status:
WIFEXITED(status): 若为正常终止子进程返回的状态，则为真。（查看进程是否是正常退出）
WEXITSTATUS(status): 若WIFEXITED非零，提取子进程退出码。（查看进程的退出码）
```

我们用这两个宏来优化一下我们等待成功，也就是子进程结束的时候的代码：

```c
      else                                                                                       
      {                                                                                            
        //等待成功                                                                                   
        if(WIFEXITED(status))                                                       
        {                                                                                         
          //正常退出                                                                                 
          printf("wait success, child exit code :%d", WEXITSTATUS(status));                           
        }                                                                                             
        else 
        {                                                                                             
          //异常退出                                                                                
          printf("wait success, child exit signal :%d", status & 0x7F);    
        }            exit(0);    
      }   
```

正常退出：

![image-20231109153300831](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109153300831.png)

异常退出：我们尝试在父进程等待的时候杀掉子进程：

![image-20231109153505131](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109153505131.png)

## 2.进程程序替换

我们为什么需要创建子进程？为了让子进程帮我执行特定的任务；

①让子进程执行父进程的一部分代码；

②如果子进程指向一段全新的代码呢？这时候我们就需要进程的程序替换！

也是为什么需要进程的程序替换。

### 2.1 程序替换是如何完成的---单线程版

代码：

```c
#include<stdio.h>    
#include<stdlib.h>    
#include<unistd.h>    
    
int main()    
{    
  printf("begin......\n");    
  printf("begin......\n");    
  printf("begin......\n");    
  printf("begin......\n");    
  printf("begin......\n");    
  execl("/bin/ls", "ls", "-a", "-l", NULL);                                                           
  printf("end........\n");    
  printf("end........\n");    
  printf("end........\n");    
  printf("end........\n");    
  printf("end........\n");    
  return 0;    
}  
```

运行结果：

那么我们可以看到这个进程运行了开始的`begin....`,然后运行了`ls`,但是后面的`end...`却不见了。

这是因为发生了进程的程序的替换，简要的原理就是，操作系统通过提供的地址`/bin/ls`从磁盘中拿出`ls`然后选到指定的文件`ls`,在输入一些参数`-a, -l`，以`NULL`表示结束。

![image-20231109162541803](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109162541803.png)

### 2.2程序替换的原理

操作系统**不动当前进程的内核数据结构**，而是**去磁盘内部拿到要替换的数据和代码**，将我们当前进程的**数据和代码**替换掉。

当进程调用一种exec函数时,该进程的用户空间代码和数据**完全被新程序替换**,从新程序的启动例程开始执行。  

所以进程的程序替换，是没有创建新的进程的。

![image-20231109163349868](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109163349868.png)

①站在进程的角度

操作系统，帮我们在磁盘内部找到我们要替换的数据和代码，替换进程的数据和代码。

②站在程序的角度

这个程序被加载了，所以我们也称`execl`这类函数为`加载器`。

我们在回到一开始，为什么我们程序后面的`end.....`却没有打印出来？

![image-20231109164919209](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109164919209.png)

原因是当我们加载程序替换的时候，**新的数据和代码就进入了进程**，当前进程后续没有没运行的代码就成为了**老代码**，直接被替换了，没有机会执行了。

所以进程的**程序替换是整体替换，而不是局部替换**。

所以我们接下来引入多进程的程序替换

### 2.3引入多进程，使用所有程序替换的接口

例程：

```c
#include<stdio.h>    
#include<stdlib.h>    
#include<unistd.h>    
#include<sys/types.h>    
#include<sys/wait.h>    
    
int main()    
{    
    
  pid_t id = fork();    
  if(id == 0)    
  {    
    //child    
    printf("我是子进程：%d", getpid());    
    execl("/bin/ls", "ls", "-a", "-l", NULL);    
  }    
    
  sleep(2);    
  //father    
  waitpid(id, NULL, 0);    
  printf("我是父进程：%d\n", getpid());                                                                                                                      
    
  return 0;    
}
```

运行结果：

我们看到`execl`之后父进程的内容也被运行了

因为进程的独立性，所以**进程的程序替换只会影响调用程序替换的子进程**。

子进程加载新程序的时候，是需要进行程序替换的，发生**写时拷贝**(子进程执行的可是全新的代码啊，新的代码，所以代码区也可以发生写时拷贝)

![image-20231109172035019](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109172035019.png)

那么对于`execl`这类加载函数，它有没有返回值呢？

> 答案是分情况：
>
> ①替换成功是没有返回值的
>
> ②替换失败是有返回值的`-1`
>
> 原因是：假设替换替换成功了，那么我们该进程中的代码和数据，都会被替换成新的代码和数据，那么我们之前的返回值也就不复存在了，并且我们也不需要返回值了。
>
> 替换失败的情况下，进程之前的代码和数据还是存在的，那么我们的返回值也是存在的，从而可以返回。
>
> **所以我们调用了加载函数之后我们不用去判断它是否加载成功，只需要在函数后面返回异常即可。**

失败的例程：

```c
#include<stdio.h>    
#include<stdlib.h>    
#include<unistd.h>    
#include<sys/types.h>    
#include<sys/wait.h>    
    
int main()    
{    
    
  pid_t id = fork();    
  if(id == 0)    
  {    
    //child    
    printf("我是子进程：%d\n", getpid());                                                                                                                    
    execl("/bin/lsss", "lsss", "-a", "-l", NULL);    
    exit(1);    
  }    
    
  sleep(2);    
  //father    
  int status;    
  waitpid(id,&status, 0);    
  printf("我是父进程：%d, child exit signal:%d\n", getpid(), WEXITSTATUS(status));    
    
  return 0;    
} 
```

![image-20231109173257987](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109173257987.png)

#### 熟悉所有的替换程序接口(7个)

①`int execl(const char *path, const char *arg, ...);`

> 例如：` execl("/bin/ls", "ls", "-a", "-l", NULL);  `
>
> `l`代表list；
>
> `path`：路径，也就是告诉操作系统，你要用来替换的程序，在磁盘的哪个路径,例程里的`/bin/ls`；
>
> `arg`：文件，是你要用来替换的文件名，例程里面的`ls`。
>
> 其中的`...`是可变参数列表,例程中的那些参数`-a , -l`。
>
> 其中最后我们要特别的输入一个`NULL`参数,告诉函数参数结束了；

②`int execv(const char *path, char *const argv[]);`

> 相比较第一个函数的差别就是，第一个函数要求我们一个一个的去传参数，而第二个要求我们直接**用数组**的形式去传，但是原则也是一样的，数组的最后一个元素也要置成`NULL`。
>
> 我们在子进程内部调用的时候是这样的：
>
> 先创建一个数组，将这些参数一个一个放进去，再传给`execv`即可
>
> 那么其实`v`就是`vector`就是数组的意思；
>
> ```c
>       char* const argv[] =    
>       {    
>         "ls",    
>         "-a",    
>         "-l",    
>         NULL    
>       };    
>      // execl("/bin/lsss", "lsss", "-a", "-l", NULL);    
>      execv("/bin/ls", argv);                                                                                                                                 
>       exit(1);    
>     }    
> 
> ```
>
> 运行结果：
>
> ![image-20231109175259752](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109175259752.png)

③`int execlp(const char *file, const char *arg, ...);`

> `p`:当我们指定执行程序的时候，只需要指定程序名即可，系统会自动在**环境变量`PATH`**中查找。
>
> 也就是说我们要用于替换的程序，必须在环境变量`PATH`中，或者说我们在环境变量`PATH`中设置过；
>
> ```c
>     execlp("ls", "ls", "-a", "-l", NULL);    
> ```
>
> ![image-20231109180335737](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109180335737.png)
>
> 那么其中的两个`ls`是不一样的，一个是文件名，一个是参数。

④`int execvp(const char *file, char *const argv[]);`

> `v`:表示参数以数组的形式传入；
>
> `p`:表示在环境变量`PATH`中去寻找用于替换的文件；
>
> ```c
>       char* const argv[] =                                                                                                                                   
>       {    
>      	"ls",    
>       	"-a",    
>      	"-l",    
>         NULL    
>       };    
> 
>       execvp("ls", argv);    
> 
> ```
>
> 运行结果：
>
> ![image-20231109181025149](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109181025149.png)

⑤`int execle(const char *path, const char *arg,  ..., char * const envp[]);`

> `envp[]`:叫做**自定义**环境变量，当我们不想使用系统默认的环境变量的时候，这个时候我们就传递一个`envp`
>
> 比如我们现在要让我们的调用`exec`目录下的`ortherproc`来替换`myproc`的子进程的后续代码
>
> ![image-20231109182618129](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109182618129.png)
>
> 先用`execl`尝试一下：
>
> ```c 
>     execl("./exec/ortherproc", "ortherproc", NULL);  
> ```
>
> 运行结果：
>
> ![image-20231109182932018](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109182932018.png)
>
> 换成的动态的效果再看看：
>
> ![动态效果](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/%E5%8A%A8%E6%80%81%E6%95%88%E6%9E%9C.gif)
>
> 下来我们尝试用`execle`来实现一下：
>
> `proc.c`
>
> ```c
>       char* const envp[] =                                                                                                             
>       {                                                                                                                                
>        "MYENVP=UCanCMe!",    
>         NULL    
>       };    
>                                                                                                                                                              
>       execle("./exec/ortherproc", "ortherproc",envp);  
> ```
>
> `ortherproc.cc`
>
> ```cpp
>   for(int i = 0; i < 5; ++i)      
>   {      
>     cout << "我是另一个程序，我的PID是 ：" << getpid() << endl;      
>     cout << "MYENVP: " << (getenv("MYENVP")==NULL ? "NULL" : getenv("MYENVP")) << endl;           cout << "PATH: " << (getenv("PATH") == NULL ? "NULL" : getenv("PATH")) << endl;      
>     sleep(1);                                                                     
>   }     
> ```
>
> 运行结果：
>
> 我们看到自定环境变量打印出来了，但是操作系统内部的环境变量却不见了，所以我们可以得到一个结论：
>
> **自定义环境变量覆盖了，默认的环境变量；**
>
> ![image-20231109193230574](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109193230574.png)
>
> 我们传**默认的环境变量**试试：
>
> ```c
> extern char ** environ;
>  
> execle("./exec/ortherproc", "ortherproc",NULL , environ);  
> ```
>
> 运行结果：
>
> ![image-20231109193621091](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109193621091.png)
>
> 那么如果我们两个都要呢，那么有一个接口`putenv`给我们提供了一个将自定义环境变量追加到进程的默认环境变量的方法：接下来我们尝试一下
>
> ```c
>       putenv("MYENVP=UCanCMe");    
>       execle("./exec/ortherproc", "ortherproc", NULL, environ);  
> ```
>
> 运行结果：
>
> ![image-20231109201734620](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109201734620.png)
>
> **插播一段**：
>
> 我们知道环境变量具有全局属性，可以被子进程继承下去，那么操作系统是怎么办到的？
>
> 只需要用`execle`的最后一个参数传过去即可！
>
> 那么我们是不是不需要`putenv`也能实现两个都能被子进程读取到呢？
>
> 我们直接把自定义的环境变量`export`到`bash`中试试：
>
> ```
> [mi@lavm-5wklnbmaja lesson6]$ export MYENVP=UCanCMe
> [mi@lavm-5wklnbmaja lesson6]$ echo $MYENVP
> UCanCMe
> ```
>
> 运行结果：
>
> 我们发现是可行的，自定义环境变量-----> bash ----->父进程------>子进程
>
> ![image-20231109202438896](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109202438896.png)

⑥`int execvpe(const char *file, char *const argv[], char *const envp[]);`

> `p`：不需要指定路径，只要在环境变量内部即可；
>
> `v`：参数以数组的形式传入；
>
> `e`：环境变量数组传入；
>
> 使用方法都与上面的类似。

⑦`int execve(const char *filename, char *const argv[], char *const envp[]);`

> 这个接口也不用过多介绍了，使用方法都是一样的。
>
> 那么我们需要注意的是，在linux的man手册中将区域六个接口都放在了`3`号手册，唯独这个却放在了`2`号手册。
>
> 其实操作系统只给我们提供了一个程序替换的接口`execve`,剩下的几个接口都是由这个接口封装出来的。
>
> ![image-20231109204020146](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231109204020146.png)

并且我们程序替换的时候不仅可以替换C语言的，甚至其他的语言都可以替换，我上面的例子也做到了用C++替换，因为这些代码都是交给操作系统来处理的而不是编译器，**所以不论是什么语言都是可以替换的！**