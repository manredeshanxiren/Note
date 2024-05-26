#  Linux动态库和静态库

## 1.了解一下库

虽然我们目前没有使用过第三方库，但是我们一直在使用一些C/C++内部的标准库，那么我们来在目录中查看一下这些库：

![](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121142313288.png)

包括我们甚至可以找到我们经常使用的`stdio.h`标准库，并且可以使用vim编辑器进行查看的！

![image-20231121142639688](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121142639688.png)

所以我们可以理解到：

> ①系统已经预装了C/C++的头文件和库文件，头文件提供方法说明，库提供方法实现，头和库是有对应关系的，是要组合在一起使用的。
>
> ②头文件是在预处理阶段就引入的，**链接的本质其实就是链接库！**

另外我们也可以理解一些现象了：

> ① 所以我们在vs2019，2022下安装开发环境---安装编译器软件，其实就是安装要开发的语言配套的库和头文件
>
> ②我们在使用编译器的时候，都会有语法的自动提醒功能，需要先包含头文件。**语法提醒本质:**编译器或者编辑器，它会自动的将用户输入的内容，不断的在被包含的头文件中进行搜索，**自动提醒**功能依赖于头文件的。
>
> ③我们在写代码的时候，我们的环境怎么知道我们的代码中有那些地有语法报错，那些地方定义变量有问题？不要太小看编译器，编译器有命令行模式，也就是我们在Linux中使用gcc命令那样，也有其他的自动化模式帮我们不断在进行语法检查。

## 2.为什么要有库

> 库可以帮助我们提高开发效率，我们学习的时候提倡去造轮子，帮我们深入理解库的原理和实现方法。
>
> 到开发中，我们更提倡用轮子！
>
> 当然我们也可以选择将以提供源代码的方式，来提高开发效率，但是这对我们未来是由一些其他问题的比如我们不想让使用者看到我们的代码，比如我们的公司向另一个公司提供软件服务，我们只需要将打包好的库给他们使用即可，而不会把源码直接给他们！

## 3.写一写----从库的书写者的角度

库分为**静态库(.a)**和**动态库(.so)**

**另外一般云服务器，默认只会存在动态库，不会存在静态库，静态库需要独自安装！**

### 3.1库的命名规则：

> 例如：`libstdc++.so.6`
>
> 去掉前面的`lib`和后面的第一个`.`后面包括`.`的内容结果就是库的名称了！
>
> 所以例子的这个库的名称使`stdc++`;

## 4.用一用----站在使用者的角度去使用我们写的库

### 4.1静态库

我们先简单实现一个加减函数的库：

我们先写四个文件：

```c
//myadd.c  
#include"myadd.h"

int myadd(int a, int b)
{
  return a + b;
}
```

```c
//myadd.h
#pragma once
#include<stdio.h>

int myadd(int a, int b);
```

```c
//mysub.c
#include"mysub.h"
int mysub(int a, int b)
{
  return a - b;
}
```

```c
//  mysub.h
#pragma once

#include<stdio.h>

int mysub(int a, int b);
```

首先我们之前的做法就是在写一个主函数的文件将他们一块编译：

但是这是我们只有这么两个源文件的时候看起来还可以，但是当我们的项目非常庞大的时候，并且我们需要分享给别人使用我们的库的时候，就非常的不方便了！

![image-20231121165901487](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121165901487.png)

我们将.c源文件都让其生成.o文件，也就是未链接前的文件：

利用ar命令生成静态库：`ar -rc libmymath.a myadd.o mysub.o`

![image-20231121174308280](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121174308280.png)

当我们把`libmymath.a`文件挪动到我们的`ortherperson`目录下的时，我们尝试编译的时候：

发现提示我们找不到头文件

![image-20231121174153962](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121174153962.png)

所以我们这样的方式是不对的！

我们先在`ortherperson`下创建两个文件夹`lib`和`include`分别存放生成的静态库和对应的头文件；

正确的是这样：`gcc -o mytest main.c -I./include -L./lib -lmymath`

我们来解释一下:

> - `-I`:其中`I`表示的`inlcude`的意思，也就是头文件的意思后面直接跟上我们头文件的路径
> - `-L`:其中`L`表示的`Lib`的意思，就是我们的库的意思，后面直接跟上我们静态库的路径
> - `-l`:这个参数后面紧跟我们的静态库的名字，就是我们前面命名规则说的那样，去掉前面的`lib`和`.`后面的内容就是我们的静态库的名字！

![image-20231121174529404](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121174529404.png)

那么我们接下来尝试一下把我们的库放到语言库中直接去调尝试一下：

```bash
[root@lavm-5wklnbmaja ortherperson]# cp lib/libmymath.a /lib64
[root@lavm-5wklnbmaja ortherperson]# cp include/*.h /usr/include
```

尝试去编译：

我们发现编译成功，并且运行成功！

![image-20231121180401518](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121180401518.png)

所以我们这里就可以引入第三方库的使用：

> ①首先我们需要指定的头文件和库文件
>
> ②如果没有默认安装到系统gcc，g++默认的搜索路径下，用户必须指明对应的选项，告知编译器：
>
> ​	a.头文件在哪；
>
> ​	b.库文件在哪；
>
> ​	c.库文件具体是谁
>
> ③将我们下载下来的库和头文件，拷贝到系统默认路径下---- 在Linux下安装库！！那么卸载呢？对任何软件而言，安装和卸载的本质就是拷贝到系统特定的路径。
>
> ④如果我们安装的库是第三方的(语言，操作系统系统接口) 库，我们要正常使用，即便是已经全部安装到了系统中，gcc/g++必须要用-l指明具体库的名称！
>
> 理解现象：
>
> ​	无论我们是从网络中未来直接下载好的库，或者是源代码(编译方法) --- make install 安装的命令 --cp，安装到系统中，我们安装大部分指令，库等等都是需要sudo的或者超级用户操作！

### 4.2动态库

我们删除掉之前的只留下最开始的源文件和头文件：

我们重新生成.o文件：

```bash
[mi@lavm-5wklnbmaja mylib]$ gcc -c -fPIC -o myadd.o myadd.c
[mi@lavm-5wklnbmaja mylib]$ gcc -c -fPIC -o mysub.o mysub.c
```

其中：fPIC：产生位置无关码(position independent code)  

![image-20231121185348586](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121185348586.png)

再生成动态库：

```bash
gcc -shared -o libmymath.so myadd.o mysub.o
```

![image-20231121185748618](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121185748618.png)

接下来我们尝试链接动态库：

```bash
gcc -o main main.c -Iinclude -Llib -lmymath
```

链接成功了，但是我们运行的时候却报错了，报错说无法找到我们的动态库文件！但是我们的链接却通过了。原因是我们确实把动态库的位置告诉了编译器，但是却没告诉操作系统！

![image-20231121190107295](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121190107295.png)

那么这里为什么没有成功呢？这里其实就是静态库和动态库的区别了，对于静态库**将用户使用的二进制代码直接拷贝到可执行目标文件的程序中**。但是我们的静态库不会。

## 5.动态库配置问题

运行时，OS是如何查找我们的动态库的？**三种方法**

### 5.1环境变量

> 动态库需要再一个环境变量`LD_LIBRARY_PATH  `中去寻找我们的动态库!(告诉操作系统动态库的位置)。
>
> 我们只需要把我们当前的动态库的路径添加到这个环境变量中即可：
>
> ![image-20231121192516738](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121192516738.png)
>
> 我们ldd查看一下链接的情况并且运行一下：
>
> ![image-20231121192541623](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121192541623.png)
>
> 当然这种方案我们不是很推崇是一种临时方案，原因是我们知道环境变量我们如果没有再配置文件修改的话，我们每次重新登录或者重启bash都会导致环境变量的初始化。

### 5.2软链接方案

> ```bash
> sudo ln -s /learn/lesson11/ortherperson/lib/libmymath.so /lib64/libmymath.so
> ```
>
> 这个时候ldd我们再去查看的时候：
>
> ![image-20231121193409826](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121193409826.png)
>
> 并且我们这时候再去运行发现是没有任何问题的：
>
> ![image-20231121193534934](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231121193534934.png)
>
> 并且对于软链接来说它不是临时的，就算我们重启bash也不会影响，因为他是真正写入到磁盘内部的，而不是在内存中！

### 5.3配置文件方案

> 首先我们查看一下系统的默认配置文件:
>
> ```bash
> [mi@lavm-5wklnbmaja lesson11]$ ls /etc/ld.so.conf.d/
> kernel-3.10.0-957.el7.x86_64.conf  mariadb-x86_64.conf
> ```
>
> 我们在这个目录下添加以一个我们的配置文件：`xupt_lib.conf`
>
> ```bash
> //创建配置文件
> [mi@lavm-5wklnbmaja lesson11]$ sudo touch /etc/ld.so.conf.d/xupt_lib.conf
> //像配置文件中写入我们的动态库所在的绝对路径
> [mi@lavm-5wklnbmaja lib]$ sudo vim /etc/ld.so.conf.d/xupt_lib.conf
> ```
>
> 我们ldd查看一下：
>
> ![image-20231122150326397](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231122150326397.png)
>
> 发现似乎还是没有链接上，其实我们还少一步生效配置文件：
>
> ```bash
> [mi@lavm-5wklnbmaja ortherperson]$ sudo ldconfig
> ```
>
> 运行一下看看：
>
> ![image-20231122150654301](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231122150654301.png)
>
> 尝试运行：
>
> ![image-20231122150722572](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231122150722572.png)

## 6.动态库加载过程的理解

静态链接形成的可执行程序中本身就有静态库中对应的方法实现!

首先我们的静态库存储在硬盘中，当多个可执行文件都要使用到这个库的时候，他就会加载到每个可执行程序内部。会让每个可执行程序的变大。其次因为它加载到了可执行程序内部，所以我们在外部删除这个静态库的时候我们的程序依然可以正常运行。所以主流会多使用动态库多一些：

---

### 6.1 动态库加载问题和周边问题：

链接：将可执行程序内部的外部符号，替换称为库中的具体地址。

只要我们把库加载到内存，映射到进程的地址空间中之后，我们的代码执行库中的方法，就依旧还是在自己的地址空间内进行函数跳转即可！

并且为什么动态库节省资源，原因是当多个进程都要使用到同一个动态库的时候，这个动态库只需要**在内存加载一份**，每个进程只需要在自己的进程地址空间中进行映射即可！因为我们曾经在链接动态库的时候程序内部已经存储了相关数据和函数的地址信息！

![image-20231122154713731](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231122154713731.png)

有时候我们的库很大，我们的程序并不会用到其中所有的接口，所以我们的操作系统也没必要把库中所有的信息都加载到内存中。

### 6.2库中地址的理解

动态库必定面临一个问题：

不同的进程，运行的程度不同，需要使用的第三方库是不同的，**注定了每一个共享空间中空闲位置是不确定的**！

在程序翻译链接，形成可执行程序的时候，程序内部有没有地址？

答案是有的：动态库中的地址，**绝对不能确定**，所以不能使用绝对地址！动态库中的所有地址，都是偏移量(相对地址)，默认从0开始。

当一个库，真正的被映射进地址空间的时候，它的起始地址才会被真正的确定！当我们映射的时候是将进程地址空间的内部的代码区**包含库的相对地址+我们的动态库的初始地址**，一块存储到进程地址空间对应的共享代码空间的位置！并且这个地址一旦被确定是不变的！所以我们经过这种方式就可以去访问动态库中的方法了！

所以动态库在我们的进程地址空间中随意加载，与我们在地址空间中加载的位置毫无关系！

所以这时候我们也可以理解之前，链接动态库的时候我们生成的汇编文件的时候带了一个参数`-fPIC`,被称作与位置无关码：也就是对应我们这里的相对地址，偏移量！

接下来我们验证一下：

我们重新生成一个非无关码的汇编文件：用于生成对应的静态库文件。

```bash
[mi@lavm-5wklnbmaja mylib]$ gcc -o myadd_static.o -c myadd.c
[mi@lavm-5wklnbmaja mylib]$ gcc -o mysub_static.o -c mysub.c
//生成对应的静态库
[mi@lavm-5wklnbmaja mylib]$ ar -rc libmymath.a myadd_static.o mysub_static.o 
```

将两个库拷贝到我们的`ortherperson/lib`目录下：

![image-20231122171305250](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231122171305250.png)

接下来我们有一个问题，当静态库和动态库同时存在的情况下，我们的可执行文件会优先使用哪个呢？**答案肯定是动态库**

我们试验一下：重新编译文件并运行

![image-20231122171700679](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231122171700679.png)

我们ldd查看一下可执行程序的链接情况：很显然是我们的动态库！

![image-20231122171733025](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231122171733025.png)

那如果我们呢执意要静态链接呢？只需要再加一个参数`-static`

```bash
[mi@lavm-5wklnbmaja ortherperson]$ gcc -o mytest-s main.c -I./include -L./lib -lmymath -static
```

当我们尝试编译的时候报错了：这是因为我们没有安装对应的库：`sudo yum install -y glibc-static`

![image-20231122183954762](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231122183954762.png)

我们分别在三种情况下编译：
①动态库和静态库都存在的情况生成:`mytest-d`

②动态库和静态库都存在的情况下加-static参数生成:`mytest-s`

③删除动态库只留下静态库再生成：`mytest-u`

ldd查看三个文件：

![image-20231122184419491](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231122184419491.png)

所以我们可以得出结论：

> ①动态库和静态库同时存在，系统默认使用动态库
>
> ②编译器，在链接的时候，如果库提供了动和静，优先动，没有动，才使用静！

