# Linux进程概念(3)

## Ⅰ. 进程状态

> 阻塞：进程因为等待某种条件就绪，而导致的一种不推进的状态--进程卡住了--阻塞一定是在等待某种资源--为什么阻塞？进程要通过等待的方式，等具体的资源被别人是用完成之后的，再被自己使用。(用于理解：**pcb指针放在对应硬件的struct队列中进行等待**）
>
> 挂起：当内存中**阻塞的进程过多**时，这时候操作系统就会用自己的算法**将进程的代码和数据交换到磁盘**时，这个过程叫挂起；
>
> 

### Ⅰ. Ⅰ Linux的进程状态

> - 为了弄明白正在运行的进程是什么意思，我们需要知道进程的不同状态。一个进程可以有几个状态（在 Linux内核里，进程有时候也叫做任务）
>
>   状态在kernel源代码的定义：
>
>   ```livescript
>   /*
>   * The task state array is a strange "bitmap" of
>   * reasons to sleep. Thus "running" is zero, and
>   * you can test for combinations of others with
>   * simple bit tests.
>   */
>   static const char * const task_state_array[] = {
>   "R (running)", /* 0 */
>   "S (sleeping)", /* 1 */
>   "D (disk sleep)", /* 2 */
>   "T (stopped)", /* 4 */
>   "t (tracing stop)", /* 8 *///追踪暂停，相当于打断点
>   "X (dead)", /* 16 *///瞬时状态很难去查看
>   "Z (zombie)", /* 32 */
>   };
>   ```
>
>   - R运行状态（running）：它并不表示进程一直在运行，它表明进程要么是在运行要么就是在运行队列里。
>
>     比如我们运行一下这个程序：
>
>     ```c
>     #include<stdio.h>                                                                     
>     int main()    
>     {    
>       while(1)    
>       {    
>         printf("I am running?\n");    
>       }    
>       return 0;    
>     }  
>     ```
>
>     运行情况我们通过ps命令查看这个进程的状态：
>   
>     ![image-20231103155817075](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103155817075.png)
>   
>     我们看到这里的状态不是R,却是S+;
>   
>     但是当我们把C代码中的打印函数注释了之后再运行一下看看，进程的状态却变成R：
>   
>     ![image-20231103163959259](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103163959259.png)
>   
>     那么为什么会这样呢？
>   
>     原因是我们再程序中添加了`printf`，意思是像屏幕上输出一些字符，因为我们知道cpu是非常快的而相对来说屏幕打印是比较慢的，那么向屏幕上输出字符的时候这个情况下，这时候我们去查看进程的状态的时候，其实大部分时间进程都是再等待屏幕打印字符，这段时间其实是进入了阻塞状态中的一种而这种状态就是我们接下来要介绍的S状态(睡眠状态)：
>   
>   - S睡眠状态（sleeping）：意味着进程在等待某事件（例如：向屏幕打印，等待键盘的输入等）完成（这里的睡眠也可以称为**<font color='red'>可中断睡眠（interruptible sleep）</font>**
>   
>     另一个例子我们让程序去打印我们输出的字符：
>   
>     ```c
>     #include<stdio.h>    
>         
>     int main()    
>     {    
>       while(1)    
>       {    
>           int n;    
>           scanf("%d", &n);    
>           printf("%d\n", n);    
>       }    
>       return 0;    
>     }  
>     ```
>   
>     我们用ps命令查看它的状态：因为它一直都在等待键盘的输入所以进入了S的转状态；
>   
>     ![image-20231103170137877](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103170137877.png)
>   
>   - D磁盘休眠状态（Disk Sleep）：**当进程的功能是向磁盘中读写数据的时候需要在内存中进行等待也就是（阻塞）当内存中阻塞的进程很多时这时候操作系统就会杀进程，但是对于这种进程操作系统会做特殊的处理将这个进程处理为D状态（这种状态无法被操作系统杀死）**，有时候也叫<font color='red'>**不可中断睡眠状态**</font>（uninterruptible sleep），在这个状态的进程通常会等待IO的结束。
>   
>     接下来我们运行这段程序：
>   
>     ```c
>     #include<stdio.h>    
>     #include<unistd.h>    
>     int main()    
>     {    
>       int n = 0;    
>       while(1)    
>       {    
>         printf("I am running? %d\n ", n++);    
>         sleep(1);                                                       
>       }    
>       return 0;    
>     } 
>     ```
>   
>     运行的效果：
>   
>     ![image-20231103200042615](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103200042615.png)
>   
>     接下来我们需要用到`kill`命令：使用`kill -l`查看`kill`指令的参数：
>   
>     命令18和19分别就是继续和停止进程的指令；
>   
>     那么我们如何使用这些指令呢？格式如下：
>   
>     `kill -18 PID` 或者`kill -19 PID`
>   
>     ![image-20231103200351904](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103200351904.png)
>   
>     接下来我们尝试去使用一下这个命令看会产生什么效果，首先我们找到当前运行的进程的`pid`,然后再使用`kill -19`:暂停进程，这时候我们再去查看进程的状态：
>   
>     ![image-20231103201029386](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103201029386.png)
>   
>     首先我们可以观察到两点：
>   
>     ①进程确实被暂停了；
>   
>     ②进程的状态从`S+`变成了`T`,不仅字母变了而且还少了加号；
>   
>     接下来我们再继续进程再次查看进程的状态：
>   
>     我们可以观察到：进程继续跑了，但是状态圈变成了`S`而不是`S+`;
>   
>     ![image-20231103201307320](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103201307320.png)
>   
>   - T停止状态（stopped）：可以通过发送 SIGSTOP 信号给进程来停止（T）进程。这个被暂停的进程可 以通过发送 SIGCONT 信号让进程继续运行。
>   
>     那么我们现在有一个疑问就是状态里的`+`有什么作用？
>   
>     **<font color='orange'>这里的`+`其实表示的是该进程是在前台运行的，后台运行是没有`+`的；</font>**
>   
>     并且这个时候我们再去`ctrl + c`也无法停止这个进程！
>   
>     ![image-20231103201752937](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103201752937.png)
>   
>     那么我们怎么杀掉这个进程呢？
>   
>     我们使用`kill`命令中的编号为`-9`的命令来杀掉这个进程就可以了。
>   
>     ![image-20231103202157343](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103202157343.png)
>
> <font color='cornflowerblue'>我们为什么要创建进程？</font>
>
> ​		因为我们需要进程帮我们办事：
>
> ​		①需要关注结果：那我们如何拿到进程执行的结果？**进程的退出码(可以是函数的返回值)**，那么我们如何得到进程的退出码？通过shell命令`echo $?`
>
> ​		例如这里我们让主函数返回3，然后再查看结果：
>
> ​		![image-20231103204516129](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103204516129.png)
>
> ![image-20231103204605905](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231103204605905.png)
>
> 那么进程都结束了我们是怎么拿到这个结果的呢？
>
> Linux当进程退出的时候，一般进程不会立即彻底退出，而是要维持一个状态叫做Z，也叫做僵尸状态-----方便后续OS读取孩子进程退出的结果！
>
> ​		②不需要关注结果
>
> - Z(zombie)-僵尸进程
>
>   > - 僵死状态（Zombies）是一个比较特殊的状态。当进程退出并且父进程（使用wait()系统调用,后面讲） 没有读取到子进程退出的返回代码时就会产生僵死(尸)进程
>   > - 僵死进程会以终止状态保持在进程表中，并且会一直在等待父进程读取退出状态代码。
>   > - 所以，只要子进程退出，父进程还在运行，但父进程没有读取子进程状态，子进程进入Z状态
>
> 僵尸进程
>
> > - 进程的退出状态必须被维持下去，因为他要告诉关心它的进程（父进程），你交给我的任务，我办的怎 么样了。可父进程如果一直不读取，那子进程就一直处于Z状态？是的！
> > - 维护退出状态本身就是要用数据维护，也属于进程基本信息，所以保存在task_struct(PCB)中，换句话说，Z状态一直不退出，PCB一直都要维护？是的！
> > - 那一个父进程创建了很多子进程，就是不回收，是不是就会造成内存资源的浪费？是的！因为数据结构 对象本身就要占用内存，想想C中定义一个结构体变量（对象），是要在内存的某个位置进行开辟空间！
> > - 内存泄漏?是的！

----

最后我们再演示一个例子：

```c
#include <stdio.h>    
#include <unistd.h>    
    
int main()    
{    
    
  int ret = fork();    
    
  if(ret == 0)    
  {    
    while(1)    
    {    
      printf("我是子进程，我的PID是 %d, 我的PPID是%d\n", getpid(), getppid());    
      sleep(1);    
    }    
   // sleep(1);    
  }    
  else    
  {    
    int i = 10;                                                                            
    while(i--)    
    {    
      printf("我是父进程，我的PID是 %d, 我的PPID是 %d\n", getpid(), getppid());    
      sleep(1);    
    }    
   // sleep(1);    
  }    
  return 0;    
}  
```

我们运行这个程序并用如下的指令观察进程的状态：

```bash
while :; do ps axj | head -n1 && ps axj | grep myproc | grep -v grep; sleep 1; echo "--------------------"; done
```

运行结果：

![image-20231104134321240](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231104134321240.png)

我们可以看到父进程在运行了10秒钟之后，终止了。但是我们的子进程还在！

那么这个子进程就是我们接下来要介绍的**孤儿进程**

### Ⅰ. Ⅱ 孤儿进程

> - 父进程如果提前退出，那么子进程后退出，进入Z之后，那该如何处理呢？  
> - 父进程先退出，子进程就称之为“孤儿进程”  
> - 孤儿进程被1号init进程领养，当然要有init进程回收喽。  



