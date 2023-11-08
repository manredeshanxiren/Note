# Linux进程控制(1)

## 1.进程创建

### 1.1fork函数

在linux中fork函数时非常重要的函数，它从已存在进程中创建一个新进程。新进程为子进程，而原进程为父进程。  

```c
#include <unistd.h>
pid_t fork(void);
返回值：自进程中返回0，父进程返回子进程id，出错返回-1
```

进程调用fork，当控制转移到内核中的fork代码后，内核做：  

其实刚刚fork的时候子进程和父进程都通过自己的页表指向同一块内存空间，只有当我们发生写时拷贝的时候才会

为其分配新的存储数据的位置。

> - 分配新的内存块和内核数据结构给子进程  
> - 将父进程部分数据结构内容拷贝至子进程  
> - 添加子进程到系统进程列表当中  
> - fork返回，开始调度器调度  

![image-20231107192910347](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231107192910347.png)

 当一个进程调用fork之后，就有两个二进制代码相同的进程。而且它们都运行到相同的地方。但每个进程都将可以开始它们自己的旅程，看如下程序：

 ```c
 int main( void )
 {
 	pid_t pid;
 	printf("Before: pid is %d\n", getpid());
 	if ( (pid=fork()) == -1 )perror("fork()"),exit(1);
 	printf("After:pid is %d, fork return %d\n", getpid(), pid);
 	sleep(1);
 	return 0;
 }
 运行结果：
 [root@localhost linux]# ./a.out
 Before: pid is 43676
 After:pid is 43676, fork return 43677
 After:pid is 43677, fork return 0
 ```

   这里看到了三行输出，一行before，两行after。进程43676先打印before消息，然后它有打印after。另一个after
 消息有43677打印的。注意到进程43677没有打印before，为什么呢？如下图所示  

![image-20231107193726528](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231107193726528.png)

所以，fork之前父进程独立执行，fork之后，父子两个执行流分别执行。注意，fork之后，**谁先执行完全由调度器**
**决定。**  

### 1.2fork的返回值

> - 子进程返回0
> - 父进程返回的是子进程的pid

### 1.3写时拷贝

通常，父子代码共享，父子再不写入时，数据也是共享的，当任意一方试图写入，便以写时拷贝的方式各自一份副
本。具体见下图:

![image-20231108113305117](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108113305117.png)

我们从图中也可以看出，其实页表也是有权限的，有些内容操作系统允许我们可以被修改，有些不允许我们修改。

例如我们在C语言中定义的常量字符串，那么在操作系统底层，页表这里的所对应的权限就只有只读，所以我们不能去写对应的内容，而只能去读。

**<font color='cornflowerblue'>进程具有独立性：</font>**进程的内核数据结构是相互独立的，进程的数据和代码，是由**写时拷贝**来帮我们实现数据和代码的独立性。

#### 1.3.1 那么我们为什么需要写时拷贝呢？

> 原因是操作系统是不允许任何不高效和浪费出现的，因为我们的子进程可能，并不会访问我们父进程中的个别的数据或者大部分数据或者代码，那么这部分代码和数据就没有必要去拷贝，拷贝了就是资源的浪费。
>
> 所以写时拷贝本质上是一种资源筛选，当子进程需要修改对应的数据时，操作系统才会拷贝一份，所以写时拷贝本质上是一种按需分配的策略。

#### 1.3.2写时拷贝会不会拷贝代码

> 一个文件生成可执行程序，无非只有两套东西，一个是可执行程序的代码，一个是可执行程序的数据。
>
> 因为我们未来不会去修改代码，虽然代码不可以被修改，但是代码会被整理或者去替换。所以其实代码也会发生写时拷贝，细节后续再谈。

### 1.4 fork的常规用法

> - 一个父进程希望复制自己，使父子进程同时执行不同的代码段。例如，父进程等待客户端请求，生成子
>   进程来处理请求  
> - 一个进程要执行一个不同的程序。例如子进程从fork返回后，调用exec函数 

### 1.5 fork失败的原因

> - 系统中有太多的进程
> - 实际用户的进程数超过了限制
>
> 因为实际在操作系统中，我们创建进程，或者创建子进程，都需要操作系统去帮我们构建程序地址空间，构建页表的相关映射，或者发生写时拷贝，这些操作都是需要内存的。

## 2.进程终止

### 2.1 进程终止的情况分类

> a.**正常**执行完成
>
> ​	1.结果正确
>
> ​	2.结果不正确
>
> b.运行过程中崩溃了(**进程异常**)【信号】：进程因为某些原因(除零，越界等等)，导致进程收到了来自操作系统的信号(`kill -9`)

我们一般来说可以自行查看进程的运行结果来查看运算结果是否正确，但是我们有时候不能仅凭肉眼的观察得程序的运算结果是否正确。

比如我们的`main`函数，我们一般写`main`函数的时候，会`return 0`, 其实这个`return 0`相当于进程的退出码---表示进程正常的执行完了！

那么我们用`0`来表示结果正确，其余的数字表示不同的原因---->供用户进行进程退出健康状态的判定。

---

接下来尝试运行一段程序：//计算1+....+100

```c
#include<stdio.h>    
    
int add_to_top(int top)    
{    
  int sum = 0;    
  for(int i = 0; i < top; ++i)    
  {    
    sum += i;    
  }    
  return sum;    
}    
    
    
int main()    
{    
    
  int ret = add_to_top(100);    
  if(ret == 5050) return 0;    
  else return 123;                                                                                                  
  return 0;    
}    

```

但是我们并没有打印程序的运算结果，那么我们怎么直到程序的运算结果是对(复合我们的预期)还是不对

![image-20231108122753836](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108122753836.png)

那么这个时候我们只需要`echo $?`去查看进程的退出码(也就是我们在main函数最后`return `的值)即可：

```bash
[mi@lavm-5wklnbmaja lesson5]$ echo $?
123
[mi@lavm-5wklnbmaja lesson5]$  
```

我们尝试反复执行一下`echo $?`看看会发生什么？

```bash
[mi@lavm-5wklnbmaja lesson5]$ echo $?
0
[mi@lavm-5wklnbmaja lesson5]$ echo $?
0
[mi@lavm-5wklnbmaja lesson5]$ echo $?
0
[mi@lavm-5wklnbmaja lesson5]$ 
```

<font color='cornflowerblue'>其实`$？`只会保存**最近一次执行的**进程的退出码.</font>

我们将上面的代码修改一下：

```c
#include<stdio.h>    
    
int add_to_top(int top)    
{    
  int sum = 0;  
  //修改的位置添上了等于号
  for(int i = 0; i <= top; ++i)                                                                       
  {    
    sum += i;    
  }    
  return sum;    
}    
    
    
int main()    
{    
      
  int ret = add_to_top(100);    
  if(ret == 5050) return 0;    
  else return 123;    
    
  return 0;    
}
```

再次运行，并查看退出码：

```bash
[mi@lavm-5wklnbmaja lesson5]$ make
gcc -o mytest test.c -std=c99
[mi@lavm-5wklnbmaja lesson5]$ ./mytest 
[mi@lavm-5wklnbmaja lesson5]$ echo $?
0
[mi@lavm-5wklnbmaja lesson5]$ 
```

那么我们只看这么对的错误退出码，但是我们却不知道所对应的原因是什么，所以我们必须要将其转化成对应的原因方便我们对进程的结果进行分析。

我们先来见识一下C语言为我们所提供的进程退出码和对应的原因，C语言中对应的函数叫做`strerror`

```bash
#include<stdio.h>    
#include<string.h>                                                                                   
    
int main()    
{    
  for(int i = 1; i <= 200; ++i)    
  {    
    printf("%d -> %s\n", i, strerror(i));    
  }    
    
  return 0;    
}
```

运行结果: 我们可以看出其实最多C语言也只提供了133退出码

```bash
[mi@lavm-5wklnbmaja lesson5]$ ./mytest 
1 -> Operation not permitted
2 -> No such file or directory
3 -> No such process
4 -> Interrupted system call
5 -> Input/output error
6 -> No such device or address
7 -> Argument list too long
8 -> Exec format error
9 -> Bad file descriptor
10 -> No child processes
11 -> Resource temporarily unavailable
....
134 -> Unknown error 134
135 -> Unknown error 135
136 -> Unknown error 136
137 -> Unknown error 137
....
```

我们看到2号退出码对应的错误原因对于我们来说很常见，我们来尝试复现一下：

---所以我们也可以明白其实ls底层也是C语言去实现的，但是linux中也不是所有的命令都遵循C语言提供的退出码

```bash
[mi@lavm-5wklnbmaja lesson5]$ ls 123.txt
ls: cannot access 123.txt: No such file or directory
[mi@lavm-5wklnbmaja lesson5]$ echo $?
2
[mi@lavm-5wklnbmaja lesson5]$ 
```

我们甚至可以自己提供一个错误码供自己使用：

```c
const char * errorstr[]    
{    
  "success",    
  "error"    
};   
```

###  2.2如何理解进程退出

进程退出的时候，操作系统内部就会少一个进程，操作系统就要释放对应的内核数据结构+代码和数据(如果有独立的)；

### 2.3那么我们该如何操作进程的退出？

#### 2.3.1进程退出的方式

**正常退出**

> - `main`函数`return`。那么其他函数呢？其他函数仅仅代表该函数返回-->进程执行，本质是`main`执行流执行。
>
> - `exit(int code)`函数：其中`code`就是我们的退出码，等价于 main return xxx;
>
>   在代码的任意位置调用该函数都表示进程退出。
>
> - `_exit(int code)`函数：它和`exit`函数近似等价，但是却有一些区别：
>
>   <img src="https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108135123221.png" alt="image-20231108135123221" style="zoom:67%;" />
>
>   

异常退出

> - ctrl + c，信号终止  

## 3.进程等待

### 3.1为什么需要进程等待

> - 之前讲过，子进程退出，父进程如果不管不顾，就可能造成‘僵尸进程’的问题，进而造成内存泄漏  
> - 另外，进程一旦变成僵尸状态，那就刀枪不入，“杀人不眨眼”的kill -9 也无能为力，因为谁也没有办法
>   杀死一个已经死去的进程  
> - 最后，父进程派给子进程的任务完成的如何，我们需要知道。如，子进程运行完成，结果对还是不对，
>   或者是否正常退出  
> - 父进程通过进程等待的方式，回收子进程资源，获取子进程退出信息  

### 3.2什么是进程等待

通过系统调用，获取子进程的退出码或者退出信号的方式，顺便释放内存问题

### 3.3如何进程等待

#### 3.3.1 wait函数

> ```apl
> #include<sys/types.h>
> #include<sys/wait.h>
> pid_t wait(int*status);
> 返回值：
> 成功返回被等待进程pid，失败返回-1。
> 参数：
> 输出型参数，获取子进程退出状态,不关心则可以设置成为NULL
> ```
>
> ```c
> #include<stdio.h>
> #include<string.h>
> #include<unistd.h>
> #include<sys/types.h>
> #include<sys/wait.h>
> #include<stdlib.h>
> int main()
> {
>     pid_t id = fork();
> 
>     if(id == 0)
>     {
>       //子进程    
>       int cnt = 5;    
>       while(cnt)    
>       {    
>         printf("我是子进程，我还活着呢，我还有%dS，我的pid：%d，我的ppid：%d\n", cnt--, getpid(), getppid());    
>       	sleep(1);    
>       }    
>      exit(0);    
>     }    
>  
>     sleep(10);                                                                                                                            
>   pid_t ret_id = wait(NULL);    
> printf("我是父进程，我等待成功了，我的pid：%d，我的ppid：%d\n, ret_id: %d\n",\    
>    getpid(), getppid(), ret_id);    
>     sleep(5);   
> ```
>
> 运行结果：
>
> 前五秒一直是子进程在运行，父进程一直**在等待子进程**；
>
> 我们可以看到子进程持续了一段时间的僵尸状态，随后父进程将其回收掉了。
>
> ![image-20231108153020457](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108153020457.png)

#### 3.3.2 wait_pid函数

 ```apl
 pid_ t waitpid(pid_t pid, int *status, int options);
 返回值：
 当正常返回的时候waitpid返回收集到的子进程的进程ID；
 如果设置了选项WNOHANG,而调用中waitpid发现没有已退出的子进程可收集,则返回0；
 如果调用中出错,则返回-1,这时errno会被设置成相应的值以指示错误所在； 参数：
 pid：
 Pid=-1,等待任一个子进程。与wait等效。
 Pid>0.等待其进程ID与pid相等的子进程。
 status:
 WIFEXITED(status): 若为正常终止子进程返回的状态，则为真。（查看进程是否是正常退出）
 WEXITSTATUS(status): 若WIFEXITED非零，提取子进程退出码。（查看进程的退出码）
 options:
 WNOHANG: 若pid指定的子进程没有结束，则waitpid()函数返回0，不予以等待。若正常结束，则返回该子进
 程的ID
 ```

> - 如果子进程已经退出，调用wait/waitpid时，wait/waitpid会立即返回，并且释放资源，获得子进程退
>   出信息  
> - **如果在任意时刻调用wait/waitpid，子进程存在且正常运行，则进程可能阻塞**  
> - 如果不存在该子进程，则立即出错返回

#### 3.3.3 获取子进程status

> - wait和waitpid，都有一个status参数，该参数是一个输出型参数，由操作系统填充  
>
> - 如果传递NULL，表示不关心子进程的退出状态信息  
>
> - 否则，操作系统会根据该参数，将子进程的退出信息反馈给父进程  
>
> - tatus不能简单的当作整形来看待，可以当作位图来看待，具体细节如下图（只研究status低16比特
>   位）：  
>
> - ![image-20231108173335756](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108173335756.png)
>
>   正常终止：
>
>   代码：
>
>   ```c
>   #include<stdio.h>
>   #include<string.h>
>   #include<unistd.h>
>   #include<sys/types.h>
>   #include<sys/wait.h>
>   #include<stdlib.h>
>   int main()
>   {
>       pid_t id = fork();
>   
>       if(id == 0)    
>       {    
>         //子进程    
>         int cnt = 5;    
>         while(cnt)    
>         {    
>           printf("我是子进程，我还活着呢，我还有%dS，我的pid：%d，我的ppid：%d\n", cnt--, getpid(), getppid());    
>         sleep(1);    
>         }    
>         exit(1);                                                                                                            
>       }   
>   int status = 0;    
>    pid_t ret_id = waitpid(id, &status, 0);    
>   printf("我是父进程，我等待成功了，我的pid：%d，我的ppid：%d, ret_id: %d, child exit code: %d, child exit signal:%d\n",\
>      getpid(), getppid(), ret_id, (status >> 8)&0xFF, status & 0x7F);
>       sleep(5);
>   	return 0;
>   }
>   ```
>
>   **运行结果：**
>
>   正常退出结果错误
>
>   ![image-20231108174658850](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108174658850.png)
>
>   正常退出结果正确
>
>   ![image-20231108174959865](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108174959865.png)
>
>   **异常终止：**
>
>   我们在子进程的代码中，做了一个除零错误，我们发现有了退出信号：8，**异常的时候退出码就不重要了！**
>
>   ![image-20231108175203108](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108175203108.png)
>
>   野指针错误：
>
>   ![image-20231108180243747](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108180243747.png)
>
>   子进程死循环，用`kill -9`结束它：
>
>   ![image-20231108180509113](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108180509113.png)
>
>   

#### 3.3.4 父进程是如何获取子进程的退出信息的？

> 通过`wait \ waitpid`来读取子进程的内核数据结构！
>
> 因为我们操作系统给每个进程的内核数据机构中都维护了两个变量`exit_code` 和`exit_signal`,分别存储进程的退出码和退出信号，那么我们的父进程就可以通过`wait \ waitpid`内部的操作系统提供的接口，我们就可以访问到则两个变量从而拿到子进程的**退出码**和**退出信号**；

#### 3.3.5 子进程在运行的时候父进程在干什么？

> 子进程在运行的时候父进程在**阻塞等待。**
>
> 那么我们可不可以不让父进程阻塞等待呢？
>
> 答案是可以的！操作系统对此也有一个专用的名词，叫做**非阻塞轮询**；
>
> 我们运行如下的程序：
>
> ```c
> #include<stdio.h>
> #include<string.h>
> #include<unistd.h>
> #include<sys/types.h>
> #include<sys/wait.h>
> #include<stdlib.h>
> int main()
> {
>     pid_t id = fork();
>   
> 	if(id == 0)
>     {
>       //子进程
>       int cnt = 5;
>       while(cnt)
>       {
>        printf("我是子进程，我还活着呢，我还有%dS，我的pid：%d，我的ppid：%d\n", cnt--, getpid(), getppid());
>       sleep(1);
>       }
>       exit(0);
>     }
>     //父进程
> 	while(1)
>     {
> 
>       int status = 0;
>       pid_t ret_id = waitpid(id, &status, WNOHANG);// 夯住了
>       if(ret_id < 0)
>       {
>         printf("waitpid_error\n");
>       }
>       else if(ret_id == 0)
>       {    
>         printf("子进程还没有结束，我先干干我自己的事情\n");    
>       }    
>       else    
>       {    
>     
>       printf("我是父进程，我等待成功了，我的pid：%d，我的ppid：%d, ret_id: %d, child exit code: %d, child exit signal:%d\n",getpid(), getppid(), ret_id, (status >> 8)&0xFF, status & 0x7F);        
>         exit(0);                                                                                                               
>       }    
>       sleep(1);    
>     }    
>     sleep(5);   
>     
>  return 0;
> }
> 
> ```
>
> ![image-20231108184420546](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231108184420546.png)

