# Linux文件系统

## 我们先问几个问题：

①你真的理解文件原理和操作了吗?

②是不是只有C/C++有文件操作呢？有没有一种统一的视角去看所有的语言的文件操作呢？

③操作文件的时候，第一件事，都是打开文件，打开文件是做什么呢？如何理解呢？

## 一些现有的结论

①文件 = 内容 + 属性 --->针对文件的操作，对内容的操作，对属性的操作。

②当文件没有被操作的时候，文件一般在什么位置？**磁盘**

③当我们对文件进行操作的时候，文件需要在哪里？**内存**，冯诺依曼体系结构决定了。

④当我们对文件进行操作的时候，文件需要提前被load到内存，load是内容还是属性？至少得有属性。

⑤当我们对文件进行操作的时候，文件需要提前被load到内存，是不是只有我们一个人在load呢？不是，内存中一定存在着大量不同的文件的属性。

⑥所以综上所述，打开文件就是将文件的核心的属性和一些内容load到内存，OS内部一定会同时存在大量的被打开的文件，那么操作系统要不要管理这些被打开的文件呢？**先描述再组织！**

先描述：先构建在内存中的文件结构体(对象) **struct XXX{就可以从磁盘来， struct file * next}**；

a.每一个被打开的文件，都要在OS内对应文件对象的struct结构体，可以将所有的struct file结构体用某种数据结构链接起来----，在OS内部，对被打开的文件进行管理，就被转换成了对链表的增删查改。

结论：文件被打开，OS要为被打开的文件，创建对应的内核数据结构，

```c
struct file
{
    //各种属性
    //各种链接关系
}
```

⑦文件其实可以被分为两大类：**磁盘文件**和**内存文件**(被我们打开的文件)。

⑧文件被打开，是谁在打开？OS，是谁让操作系统打开的呢？**进程**

⑨我们之前的所有的文件操作，都是进程和被打开文件的关系。

⑩进程和被打开文件的关系：`struct struct_task`和`struct file`的关系

## 快速回忆C文件操作

①`fopen`函数:用于打开文件的函数

![image-20231112175415441](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112175415441.png)

一些参数：

![image-20231112184405352](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112184405352.png)

②`perror`函数:用于将错误码装化成对用的`strerror`

![image-20231112175639963](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112175639963.png)

③`fclose`函数

![image-20231112180051947](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112180051947.png)

④`fputs`函数：向文件中写入

![image-20231112180231175](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112180231175.png)

写一个程序尝试向文件中写入字符串：

```c

```

运行结果：

![image-20231112181203458](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112181203458.png)

查看文件大小的命令:`du -参数 文件名`, 参数是单位，比如m，k等，例如我们刚刚这个文件就是1m

```bash
[mi@lavm-5wklnbmaja lesson7]$ du -m log.txt
1	log.txt
```

⑤`snprintf`函数：向字符串中写入(**C语言的格式化支持的很好**)

1.第一个函数是向屏幕输出

2.第二函数是向**文件**输出

3.第三个函数是向**字符串**(缓冲区)输出

4.第四个函数是向**字符串**(缓冲区)中输出n个字符

![image-20231112182333353](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112182333353.png)

尝试使用一下：

```c
#include<stdio.h>    
    
#define LOG "log.txt"    
    
int main()    
{    
  //w：默认写方式打开文件，如果文件不存在，就创建它    
  //默认如果只是打开，文件内容会自动被清空    
  //同时，每次进行写入的时候，都会从最开始开始写入     
  FILE *fp = fopen(LOG, "w");    
  if(fp == NULL)    
  {    
    perror("fopen"); // fopen:xxxx    
    return 0;    
  }    
    
  //正常进行文件操作    
  const char* msg = "hello xupt, hello xiyou";                                                                                                                 
  int cnt = 5;    
  while(cnt)    
  {    
    fprintf(fp, "%s : %d : xupt \n", msg, cnt);    
   // fputs(msg, fp);    
    cnt--;    
  }    
    
  fclose(fp);    
  return 0;    
}  
```



运行结果：

![image-20231112182923960](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112182923960.png)

**`snprintf`的使用：**

```c
#include<stdio.h>    
    
#define LOG "log.txt"    
    
int main()    
{    
  //w：默认写方式打开文件，如果文件不存在，就创建它    
  //默认如果只是打开，文件内容会自动被清空    
  //同时，每次进行写入的时候，都会从最开始开始写入     
  FILE *fp = fopen(LOG, "w");    
  if(fp == NULL)    
  {    
    perror("fopen"); // fopen:xxxx    
    return 0;    
  }    
    
  //正常进行文件操作    
  const char* msg = "hello xupt, hello xiyou";    
  int cnt = 5;    
  while(cnt)    
  {    
    char buffer[256];    
    snprintf(buffer, sizeof(buffer), "%s : %d\n", msg , cnt);                                                                                                  
    fputs(buffer, fp);    
   // fprintf(stdout, "%s : %d : xupt \n", msg, cnt);    
   // fputs(msg, fp);    
    cnt--;    
  }    
    
  fclose(fp);    
  return 0;    
}  
```

运行结果：

![image-20231112184036214](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112184036214.png)

⑥`stdout`

那么我们观察到它们的类型都是我们对应的`FILE`，**因为在Linux系统中一切皆文件**，接下来我们尝试用`fprintf`函数向`stdout`中输出一些字符串看看

![image-20231112183052514](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112183052514.png)

代码：

```c
#include<stdio.h>    
    
#define LOG "log.txt"    
    
int main()    
{    
  //w：默认写方式打开文件，如果文件不存在，就创建它    
  //默认如果只是打开，文件内容会自动被清空    
  //同时，每次进行写入的时候，都会从最开始开始写入     
  FILE *fp = fopen(LOG, "w");    
  if(fp == NULL)    
  {    
    perror("fopen"); // fopen:xxxx    
    return 0;    
  }    
    
  //正常进行文件操作    
  const char* msg = "hello xupt, hello xiyou";    
  int cnt = 5;    
  while(cnt)    
  {    
    fprintf(stdout, "%s : %d : xupt \n", msg, cnt);                                                                                                            
   // fputs(msg, fp);    
    cnt--;    
  }    
    
  fclose(fp);    
  return 0;    
} 
```

运行结果：

直接向屏幕输出了字符串

![image-20231112183450307](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112183450307.png)

⑦`fgets`函数

把文件中的信息写入到缓冲区中

代码：

```c
#include<stdio.h>    
    
#define LOG "log.txt"    
    
int main()    
{    
  //w：默认写方式打开文件，如果文件不存在，就创建它    
  //1.默认如果只是打开，文件内容会自动被清空    
  //2.同时，每次进行写入的时候，都会从最开始开始写入     
  //a:不会清空文件，每次都从文件的结尾开始写入     
  //r:以读的形式，打开文件    
  FILE *fp = fopen(LOG, "r");    
  if(fp == NULL)    
  {    
    perror("fopen"); // fopen:xxxx    
    return 0;    
  }    
    
    
  while(1)    
  {    
    char line[128];    
    if(fgets(line, sizeof(line), fp) == NULL) break;    
    else printf("%s", line);    
  }    
   fclose(fp);
  return 0;
}
```

运行结果：

![image-20231112185104125](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112185104125.png)

那么到此结束，开始系统的文件操作

## 系统的文件接口

①`open`

`pathname`:表示要打开的文件路径+文件名

`flag`:代表打开文件的方式

![image-20231112185909086](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112185909086.png)

返回值：

返回一个文件描述符，-1表示错误，然后回设置`error`

![image-20231112190103226](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112190103226.png)

参数：

这些参数都是一些宏定义，所以函数参数的类型是int，他们分别对应一些含义，这只是其中一部分。

这些参数被称为标志位，类似于之前介绍的位图，**每个参数对应一个比特位**。

我们如果要传多个标志位：直接将他们`|`运算一下即可。

![image-20231112190508804](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112190508804.png)

![image-20231112190641570](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112190641570.png)

OS一般如何让用户给自己传递标志位的？

1.我们怎么做的？

2.系统怎么做的？我们用一个比特位表示一个标记位，那么一个int类型就可以表示32个标志位(位图)。

接下来一个小demo1实际操作演示一下：

```c
#include<stdio.h>

#define ONE 0x1
#define TWO 0x2
#define THREE 0x4
#define FOUR 0x8
#define FIVE 0x10
    
void Print(int flag)    
{    
  if(flag & ONE) printf("hello 1\n");    
  if(flag & TWO) printf("hello 2\n");    
  if(flag & THREE) printf("hello 3\n");    
  if(flag & FOUR) printf("hello 4\n");    
  if(flag & FIVE) printf("hello 5\n");    
}    
    
int main()    
{    
  Print(ONE);   //打印1 
  printf("-------------------------------\n");    
  Print(TWO);    //打印2
  printf("-------------------------------\n");    
  Print(THREE);    //打印3
  printf("-------------------------------\n");    
  Print(FOUR);    //打印4
  printf("-------------------------------\n");    
  Print(FIVE);    //打印5                                                                                                                                      
  printf("-------------------------------\n");    
  Print(ONE | TWO | THREE | FOUR | FIVE);   //打印12345  
  printf("-------------------------------\n");    
  Print(ONE | THREE | FIVE); //打印135
  return 0;    
}
```

运行结果：

![image-20231112192748966](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112192748966.png)

那么其实`flag`就是如此工作的。

尝试使用一下：

```c
#include<stdio.h>    
#include<errno.h>    
#include<string.h>    
#include<stdlib.h>    
#include<sys/types.h>    
#include<sys/stat.h>    
#include<unistd.h>    
    
#include<fcntl.h>    
//系统方案    
    
#define LOG "log.txt"    
    
int main()    
{    
  int fd = open(LOG, O_WRONLY);    
    
  printf("fd : %d errno : %d errnostring: %s\n", fd, errno, strerror(errno));                                                                                  
    
  close(fd);    
  return 0;    
}  
```

我们查看结果却返回了-1，原因是：open函数不会创建文件，没有的指定文件的时候就会报错！

![image-20231112194839982](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112194839982.png)

我们创建了一个之后或者我们在参数中加上`O_CREAT`

```c
  int fd = open(LOG, O_WRONLY | O_CREAT); 
```

![image-20231112194856013](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112194856013.png)

但是我们去尝试查看这个文件的时候：

![image-20231112195241507](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112195241507.png)

那么我们可以使用`open`的一个重载：`int open(const char *pathname, int flags, mode_t mode);`

第三个参数`mode`是我们可以给予文件的默认权限：比如我们给个`0666`:

```c
  int fd = open(LOG, O_WRONLY | O_CREAT, 0666); 
```

![image-20231112195528363](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112195528363.png)

但是我们发现文件`log.txt`的权限却不是0666，原因其实是因为文件的权限还受`umask`的影响：**默认权限&(~mask)**

我们查看一下`umask`是否是2：果然如此我们把它改成0000在运行一下程序看看

![image-20231112195724141](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112195724141.png)

是我们想要的结果了!

![image-20231112195825596](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112195825596.png)

**其实我们已经可以猜到：**<u>各种语言中的文件操作其实内部都是封装了操作系统给的接口，每个语言都进行了自己的个性化，所以每个语言的文件操作都不尽相同，但是底层都是封装了操作系统给的接口！</u>

②`write`：操作系统提供的，向文件写入的接口

`fd`：文件描述符

`buf`：缓冲区

`count`: 写入缓冲区的大小

那么`write`的功能就是：将缓冲区`buf`中的count个字节的信息，写入到open返回的`fd`的对应的文件

返回值：表示实际写入了多少字节，写入失败返回`-1`

![image-20231112200426541](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112200426541.png)

代码：

```c
#include<string.h>    
#include<stdlib.h>    
#include<sys/types.h>    
#include<sys/stat.h>    
#include<unistd.h>    
    
#include<fcntl.h>    
//系统方案    
    
#define LOG "log.txt"    
    
int main()    
{    
  int fd = open(LOG, O_WRONLY | O_CREAT, 0666);    
    
  printf("fd : %d errno : %d errnostring: %s\n", fd, errno, strerror(errno));    
    
    
  //写入    
  const char * msg = "hello xupt";    
  int cnt = 5;    
  while(cnt)    
  {    
    char line[128];    
    snprintf(line, sizeof(line), "%s , %d \n", msg, cnt);    
    
    write(fd, line, strlen(line));    
                                                                                                                                                               
    cnt--;    
  } 
```

运行结果：

![image-20231112201547388](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112201547388.png)

那么其实这里涉及到一个问题：就是我是否要往` write(fd, line, strlen(line));`中的`strlen(line)` + 1，结论是不需要的。因为`\0`是C语言的标准，操作系统内部没有`\0`的标准。所以不需要去+1；

另外：我们这里的调用`int fd = open(LOG, O_WRONLY | O_CREAT, 0666);`**,不会对文件做清空**，如果我们要打开的时候清空，只需要加上参数`O_TRUNC`;

就是这样：`int fd = open(LOG, O_WRONLY | O_CREAT | O_TRUNC, 0666);` 等价于我们的`fopen(filename, "w");`

我们的：`fopen(filename, "a");` 等价于`int fd = open(LOG, O_WRONLY |O_APPEND | O_CREAT, 0666);` 

③`read`:将文件中的信息**整体**读取到缓冲区buf中；

![image-20231112203754608](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112203754608.png)

代码：

```c
#include<sys/stat.h>    
#include<unistd.h>    
    
#include<fcntl.h>    
//系统方案    
    
#define LOG "log.txt"    
    
int main()    
{    
  //int fd = open(LOG, O_WRONLY | O_CREAT | O_TRUNC, 0666);    
  int fd = open(LOG, O_RDONLY, 0666);    
    
  printf("fd : %d errno : %d errnostring: %s\n", fd, errno, strerror(errno));    
    
    
//读取    
    
  char buf[1024];    
    
  ssize_t n = read(fd, buf, sizeof(buf) - 1); //使用操作系统进行IO的时候，要注意\0问题    
  if(n > 0)    
  {    
    buf[n] = '\0';    
    
    printf("%s", buf);    
  }    
   return 0;
}
```

运行结果：

![image-20231112204423201](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231112204423201.png)

----

## 总结

接下来我们可以回答开始的三个问题：

①你真的理解文件原理和操作了吗?

<u>文件原理和操作是需要从操作系统的角度去学习，我们从语言角度学习的远远不够。</u>

②是不是只有C/C++有文件操作呢？有没有一种统一的视角去看所有的语言的文件操作呢？

<u>并不是，所有的编程语言都有自己的文件操作，其实他们都是对操作系统文件操作的封装。所以我们从操作系统的角度去统一认识文件操作。</u>

③操作文件的时候，第一件事，都是打开文件，打开文件是做什么呢？如何理解呢？

打开文件就是将文件的属性和一些内容加载到内存，创建对应的`struct_file`；

