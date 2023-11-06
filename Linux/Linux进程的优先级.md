

# Linux进程的优先级

## 基本概念

> - cpu资源分配的先后顺序，就是指进程的优先权（priority）  
> - 优先权高的进程有优先执行权利。配置进程优先权对多任务环境的linux很有用，可以改善系统性能  
> - 还可以把进程运行到指定的CPU上，这样一来，把不重要的进程安排到某个CPU，可以大大改善系统整
>   体性能  

## 优先级和权限的比较

> 权限表示的是：能不能的问题；
>
> 优先级表示的是：已经能，但是先后顺序的问题；

## 为什么需要优先级？

> 因为我们的CPU资源和操作系统内部的资源都是有限的！

## 查看系统进程

> `ps -l`
>
> ![image-20231106154450428](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231106154450428.png)
>
> 我们很容易注意到有以下几个信息:
>
> - UID :代表执行者的身份  ，Linux会给每个用户分配一个`UID`用来区分我们这些用户；
> - PID: 代表这个进程的代号 ，就是我们之前所提到的进程的ID。
> - PPID: 代表这个进程是由哪个进程发展衍生而来的，亦即父进程的代号  
> - PRI:英文优先级(`priority`)的缩写，代表这个进程可被执行的优先级，其值越小越早被执行。
> - NI:英文nice的缩写,代表这个进程的nice值 。

## PRI and NI  

> - PRI也还是比较好理解的，即进程的优先级，或者通俗点说就是程序被CPU执行的先后顺序，此值越小
>   进程的优先级别越高  
> - 那NI呢?就是我们所要说的nice值了，其表示进程可被执行的优先级的修正数值  
> - PRI值越小越快被执行，那么加入nice值后，将会使得PRI变为： **PRI(new)=PRI(old)+nice**,这里所谓的`PRI(old)一直等于80`
> -   这样，当nice值为负值的时候，那么该程序将会优先级值将变小，即其优先级会变高，则其越快被执行  
> - 所以，调整进程优先级，在Linux下，就是调整进程nice值  
> - nice其取值范围是**-20至19**（闭区间），一共40个级别。 

## PRI vs NI  

> - 需要强调一点的是，进程的nice值不是进程的优先级，他们不是一个概念，但是进程nice值会影响到进
>   程的优先级变化。  
> - 可以理解nice值是进程优先级的修正修正数据  

尝试去修改以下nice值观察以下优先级的变化：

运行一个这样的C代码：

```c
#include<stdio.h>      
#include<unistd.h>    
int main()    
{    
      
  while(1)    
  {    
    printf(".");    
    fflush(stdout);    
    sleep(1);    
  }    
                                                                                                                    
  return 0;    
} 
```

先`top`,然后按`r`，`r`表示我们修改进程的`nice`值，然后我们先输入进程的`PID`,31976.回车

![image-20231106155913887](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231106155913887.png)

然后我们输入修改后的`nice`值，这里我们随便输入一个 -10；

![image-20231106160012888](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231106160012888.png)

`ps -al`查看一下`pri`这个进程的`PRI`和`NI`值的变化。

![image-20231106160120378](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231106160120378.png)

我们在试试将`nice`值修改成80；

![image-20231106160224999](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231106160224999.png)

我们发现nice值并没有被修改成80，而是被修改成了19，这是因为nice值得边界限制了。

![image-20231106160311968](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231106160311968.png)