# Linux进程概念(2)

## Ⅰ. 进程:

### Ⅰ . Ⅰ进程的概念:

> **<font color='orange'>内核关于进程的相关数据结构 + 当前进程的代码和数据;</font>**

### Ⅰ. Ⅱ描述进程-PCB:

> - 进程信息被放在一个叫做进程控制块的数据结构中，可以理解为进程属性的集合。
> - PCB内部的属性和文件存储在磁盘中的属性是没有太大关系的，是重新生成的。
> - 课本上称之为PCB（process control block）， Linux操作系统下的PCB是: task_struct  

#### task_ struct内容分类

> - 标示符: 描述本进程的唯一标示符，用来区别其他进程。
> - 状态: 任务状态，退出代码，退出信号等。
> - 优先级: 相对于其他进程的优先级。
> - 程序计数器: 程序中即将被执行的下一条指令的地址。
> - 内存指针: 包括程序代码和进程相关数据的指针，还有和其他进程共享的内存块的指针。
> - 上下文数据: 进程执行时处理器的寄存器中的数据[休学例子，要加图CPU，寄存器]。
> - I／O状态信息: 包括显示的I/O请求,分配给进程的I／O设备和被进程使用的文件列表。
> - 记账信息: 可能包括处理器时间总和，使用的时钟数总和，时间限制，记账号等。
> - 其他信息。

### Ⅰ. Ⅲ为什么进程管理中需要PCB:

> 1.bash命令行解释器，本质上它也是一个进程;
>
> 2.命令行启动的所有程序，最终都会变成进程，而该进程对应的父进程都是bash(如何做到的);

### Ⅰ. Ⅳ查看进程:

> 进程的信息可以通过 /proc 系统文件夹查看  
>
> ![image-20231101194517936](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231101194517936.png)
>
> 这里显示的就是我们当前操作系统中在运行的进程，那些蓝色的数字就是进程的`pid`:
>
> 当进程存在时，我们可以通过`pid`来进入文件来查看进程相关的属性：
>
> ![image-20231102154929933](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231102154929933.png)
>
> 当我们kill这个进程的时候，当我再次查看`pid`对应的文件：
>
> ![image-20231102155040719](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231102155040719.png)
>
> 大多数进程信息同样可以使用top和ps这些用户级工具来获取  
>
> `ps aux | gep test | grep -v grep`
>
> ![image-20231101194552370](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231101194552370.png)
>
> 那我们如何直接获取进程`pid`呢？这时候我们就需要用到我们接触到的第一个操作系统接口`getpid()`;
>
> `getpid()`：哪个进程去调用它就返回的是哪个进程的`pid`;
>
> `getppid():`哪个进程去调用它就返回的是哪个进程的父进程的`pid`;
>
> 我们运行如下的代码：
>
> ```c
> #include<stdio.h>    
> #include<unistd.h>    
> #include<sys/types.h>    
>     
> int main()    
> {              
>   while(1)    
>   {      
>     printf("myprocess: 我已经是一个进程了，我的PID是：%d,我的父进程PID是：%d\n", getpid(), getppid());        
>     sleep(1);    
>   }                                                                            
> }  
> ```
>
> 运行如下的结果：
>
> ![image-20231102165708920](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231102165708920.png)
>
>  然后我们反复的去运行然后观察：
>
> 我们发现父进程的`PID`一直没有变化，但是进程`PID`却在变化。那么这个没有变化的进程是谁呢？是不是我们每次用命令行运行程序的时候都是他来帮我们创建的呢？
>
> ![image-20231102170042615](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231102170042615.png)
>
> 接下来我们用`ps`命令去寻找一下这个进程：
>
> ![image-20231102170543491](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231102170543491.png)
>
> 我们发现它是`bash`，它是我们的命令行解释器，本质上它也是一个进程；
>
> 命令行启动的所有的程序，最终都会变成进程，而该进程对应的父进程就是`bash`;
>
> 那么`bash`是如何创建子进程的呢？

### Ⅰ. Ⅴ如何创建子进程:

例如:

```c++
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{     
	pid_t ret = fork();
	if(ret == 0)
	{ 
		//子进程
		printf("我是子进程，我的pid是：%d，我的父进程是：%d",getpid(),getppid()); 
		sleep(1);        
	}
	else if(ret > 0)
	{    
		//父进程
		while(1)
		{ 
			printf("我是父进程，我的pid是：%d，我的父进程是：%d"，getpid(),getppid());
			sleep(2);   
		}                          
	}                                                                                            
	return 0;                                                                                  
} 
```

运行结果:

<img src="https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231101194631231.png" alt="image-20231101194631231" style="zoom: 50%;" />

> a.fork之后，执行流会变成2个执行流;
>
> b.fork之后，谁先运行由调度器运行;
>
> c.fork之后，fork之后的**代码共享**，通常我们通过if 和 ifelse来进行执行流分流;

**<font color='red'>原理:</font>**

> - fork做了什么?:
>
> 创建了子进程，只会创建一个子进程对应的pcb，这个pcb内部的大部分内容都是和父进程相同的，并且他们共同指向同一段代码和数据;
>
> - fork如何看待？
>
>   进程在运行的时候，**是具有独立性的;**
>
>   **<font color='cornflowerblue'>父子进程也具有独立性;</font>**
>
>   代码：是只读的；
>
>   数据：当有一个执行流尝试修改数据的时候，OS会自动给我们当前的进程触发**写时拷贝**（操作系统会拷贝一份数据让进程去另一个地方修改，而不会修改原始数据）
>
> - 如何理解fork();两个返回值？
>
>   对于一个函数来说，函数执行return的时候，函数的主体功能就已经实现了。
>
>   fork函数本质上来说是OS为我们所提供的函数！
>
>   因为当执行完fork函数的主体的时候，主进程被调度和子进程也会被执行所以在fork的函数**内部return这段语句被执行了两次**，**所以返回了两个返回值**。

