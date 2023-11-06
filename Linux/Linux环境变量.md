

# Linux环境变量

首先我们通过一个小例子来进入环境变量的学习，我们知道我们运行我们自己编写的程序的时候，我们是`./文件名`的方式去运行的，但是我们运行系统自带的程序或指令却不需要加上`./`。这是为什么呢？

首先我们要明白`.`表示的是当前目录，所以`./文件名`的意思就是访问当前目录下的这个文件。

那么我们可不可以做到像运行系统中的指令那样去运行我们自己的文件呢？

答案是可以的，但是这牵扯到系统的**环境变量**，首先我们查看一下当前系统的环境变量：

![image-20231104152232252](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231104152232252.png)

我们只需要一行指令就可以实现我们想要的结果：就是图中橙色框框中的命令，这个命令就是将我们的环境变量更改为我们当前的目录；

![image-20231104152537479](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231104152537479.png)

但是似乎又出问题了当我们尝试使用系统自带的命令时，出问题了！

![image-20231104153211752](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231104153211752.png)

那么这种情况因为我们时命令式的改变环境变量，这个时候我们重启Xshell即可！

那么我们如何去不影响系统本身的命令并且实现上面的效果呢？

①向环境变量中追加我们当前的目录：

![image-20231104154846665](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231104154846665.png)

②我们把我们想要实现效果的文件放到我们已经存在在环境变中的一个目录下：

将这个文件拷贝到我们的默认目录下，这其实相当于我们的<font color='cornflowerblue'>安装</font>！

然后将文件从我们的默认目录下删除的过程，相当于<font color='cornflowerblue'>卸载</font>！

![image-20231104155335539](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231104155335539.png)

这里所谓的PATH就是我们的环境变量：

## 环境变量  

> 基本概念  
>
> - 环境变量(environment variables)一般是指在操作系统中用来指定操作系统运行环境的一些参数  
> - 如：我们在编写C/C++代码的时候，在链接的时候，从来不知道我们的所链接的动态静态库在哪里，但
>   是照样可以链接成功，生成可执行程序，原因就是有相关环境变量帮助编译器进行查找。  
> - 环境变量通常具有某些特殊用途，还有在系统当中通常具有全局特性  

### 常见的环境变量：

> - PATH : 指定命令的搜索路径 (重点) 
> - HOME : 指定用户的主工作目录(即用户登陆到Linux系统中时,默认的目录(重点)
> - SHELL : 当前Shell,它的值通常是/bin/bash

### 查看环境变量

> `echo $NAME`//NAME：当前你的环境变量的名称

### 测试HOME

> 1.用root和普通用户，分别执行 echo $HOME ,对比差异  
>
> ![image-20231105163205931](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105163205931.png)
>
> 其中每个用户的`~`目录就是每个用户的家目录。

### 和环境变量相关的命令

> 1.echo: 显示某个环境变量值  
>
> 2.export: 设置一个新的环境变量  
>
> 3.env: 显示所有环境变量  
>
> 4.unset: 清除环境变量  
>
> 5.set: 显示本地定义的shell变量和环境变量  

### 环境变量的组织方式

>![](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105163833477.png)
>
>每个程序都会收到一张环境表，环境表是一个字符指针数组，每个指针指向一个以’\0’结尾的环境字符串  

### 通过代码如何获取环境变量  

> - 命令行第三个参数  
>
> ```c
>   #include<stdio.h>                                       
>                                          
>   int main(int argc, char* argv[], char* envp[])    
>   {                                                  
>     int i;                               
>     for(i = 0; envp[i]; ++i)    
>     {                                       
>       printf("envp[%d]--->%s\n",i,envp[i]);    
>     }    
>     return 0;    
>   }  
> ```
>
> - 通过第三方变量environ获取  
>
> ```c
> #include <stdio.h>
> int main(int argc, char *argv[])
> {
> 	extern char **environ;
> 	int i = 0;
> 	for(; environ[i]; i++)
>     {
> 		printf("%s\n", environ[i]);
> 	}
> 	return 0;
> }
> ```
>
> ![image-20231105171023137](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105171023137.png)

### 利用环境变量实现一个mypwd

> 代码:
>
> ```c
> #include<stdio.h>      
> #include<stdlib.h>      
> #include<string.h>      
> #include<unistd.h>      
>       
> #define NAME "mi"      
> //int main(int argc, char* argv[])      
> int main()      
> {      
>     char* own = getenv("USER");      
>     if(strcmp(own, NAME) == 0)      
>     {      
>       printf("这个程序已经执行啦.......\n");        
>     }      
>     else      
>     {      
>       printf("当前用户%s是一个非法用法,无法执行\n", own);    
>     }  
> return 0;
> }
> ```
>
> 当我们使用普通用户`mi`来运行这个程序的时候结果是：
>
> 这个程序已经运行啦......
>
> 当我们切换到`root`的时候运行的结果就是：
>
> 当前用户root是一个非法用户，无法执行；
>
> ![image-20231105173406705](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105173406705.png)
>
> 这就是和我们每个用户所对应的环境变量有关！

### 环境变量的小总结

> 环境变量本质是一个**内存级**的一张表(这也是为什么我们在用命令修改了环境变量表重新链接之后又会还原了)，这张表由用户在登录系统的时候，进行给特定用户形成属于自己的环境变量表。
>
> 环境变量中的每一个，都有自己的用途：有的是进行路径查找的，有的是进行身份认证的，有的时候是进行动态库查找的，有的是用来进行确认当前路径等等。
>
> 每一个环境变量都有自己的特定应用场景
>
> 每一个元素都是KV的。
>
> 那么环境变量对应的数据，都是从哪里来的呢？
>
> 系统的相关配置文件中读取出来的！
>
> ![image-20231105173930190](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105173930190.png)

## 环境变量通常是具有全局属性的  

> 首先我们在bash下输入这样一条指令：
>
> ![image-20231105180419628](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105180419628.png)
>
> 我们发现bash似乎把我们这个变量`myval`存储起来了！好像跟环境变量有点关系，因为我们打印环境变量的时候就是`echo $环境变量`。
>
> 但是我们在调用`env`去环境变量表里去查找这个变量的时候我们却找不到。
>
> 我们再试试另一种方法：加上`export`因为我们之前用这个命令向环境变量中添加过`PATH`
>
> ![image-20231105180817527](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105180817527.png)
>
> 这时候我们再次调用`env`指令去环境变量里去找一下，结果是我们找到了这个变量：
>
> ![image-20231105181117251](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105181117251.png)
>
> 接下来我们为了再次确认我们用程序帮我们在**环境变量表**里去寻找一下：
>
> ```c
> #include<stdio.h>      
> #include<stdlib.h>      
> #include<string.h>      
> #include<unistd.h>      
>       
> //#define NAME "mi"      
> //int main(int argc, char* argv[])      
> int main()      
> {      
>    char* s = getenv("HELLO1");      
>    if(s != 0)    
>    {    
>       printf("%s\n",s);    
>    }    
>  return 0;
> }
> ```
>
> 我们发现结果是可以被找到的，但是我们并没有向`main`函数中传入环境变量，可是为什么它却可以找到？
>
> ![image-20231105181711205](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105181711205.png)
>
> 首先我们要理解main也是一个函数，我们通常把他叫做入口函数，其实环境变量不仅可以通过参数的形式传给我们的`main`函数，**而且也可以通过父进程传给子进程的方式传给我们的`main`函数**。
>
> 所以我们可以得出以下结论：
>
> - 环境变量通常具有全局属性，可以被子进程继承下去  
>
> 那么不加`export`的情况`myval=1`中`myval`被称之为**<font color='cornflowerblue'>本地变量</font>**，对于本地变量，只在`shell`内部有效！
>
> 也就是说本地变量不可以被子进程继承！
>
> 那么为什么`echo $myval`可以打印出结果，`echo`也按理来说是一个子进程，为什么它可以打印出来？
>
> 原因是`echo`是一个**<font color='cornflowerblue'>内建命令</font>**。

## 环境变量的意义

> 当我们没有启动操作系统的时候，环境变量就在配置文件里，当我们启动操作系统的时候，特定的进程或者说`bash `就会帮我们读取配置文件，帮我们把环境变量Load到bash内部，然后我们后面执行的所有子进程都和bash有关。因为环境变量是可以交给所有进程的，所以环境变量也可以影响所有进程的。包括我们的头文件查找，库文件查找，指令搜索等等。

## 拓展

### Linux下一些奇怪的命令

> ①`.`：
>
> ​	这个命令就一个`.`等价于`source`,他的意思是将`.`或者`source`后面所跟的配置文件立即生效！
>
> ![image-20231105215048610](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231105215048610.png)
>
> ②`[`
>
> 在Linux中，`[`命令通常用于条件测试。它用于检查文件的存在性、比较数字，以及检查字符串是否为空等。例如，如果你想检查一个文件是否存在，你可以使用`[ -f /path/to/file ]`。如果文件存在，这个命令将返回真（0），否则返回假（非0）。这个命令通常与if语句或while循环一起使用，以便根据条件执行特定的操作。
>
> 这是一个例子：
>
> ```bash
> if [ -f /path/to/file ]
> then
>     echo "File exists."
> else
>     echo "File does not exist."
> fi
> ```
>
> 在这个例子中，如果文件存在，脚本将打印"File exists.“，否则，它将打印"File does not exist.”。请注意，`[`和`]`之间的空格是必需的。如果你忘记了这些空格，你可能会得到一个错误。

## main函数的命令行参数

> 接下来我们写出一个示例代码:
>
> 首先main函数中的`argc`:表示的是参数的数量，`argv`:是一个`char*`类型的数组，存储的是这些参数。
>
> ```c
> #include<stdio.h>    
> #include<string.h>    
> #include<stdlib.h>    
> void Usage(const char* name)    
> {    
>   printf("\nUsage:%s -[a|b|c]\n", name);    
>   exit(0);                                                                                                          
> }    
> int main(int argc, char* argv[])    
> {    
>   if(argc != 2) Usage(argv[0]);    
>     
>   if(strcmp(argv[1],"-a")) printf("打印当前目录下的文件名\n");    
>   else if(strcmp(argv[1], "-b")) printf("打印当前目录下的文件的详细文件\n");    
>   else if(strcmp(argv[1], "-c")) printf("打印当前目录下的文件名(包括隐藏文件)\n");    
>     
>   return 0;    
> }  
> ```
>
> ![image-20231106150142779](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231106150142779.png)

