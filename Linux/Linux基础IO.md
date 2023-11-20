# Linux基础IO

## 1.C库FILE结构体的模拟实现

### 1.1MY_FILE结构体

因为我们简单实现：包含参数

- ①文件描述符
- ②输出缓冲区
- ③刷新方式的标志位
- ④缓冲区的当前大小

```c
typedef struct _MY_FILE    
{    
  int fd;    
  char outputbuffer[MAX];    
  int flag; //flash method    
  int current; // flash size    
} MY_FILE;  
```

### 1.2myfopen的实现

主要分四步(也是只进行主题功能的实现)：

- ①文件打开方式的获取
- ②尝试打开文件(通过操作系统提供的接口)
- ③构建MY_FILE结构体
- ④初始化MY_FILE结构体
- ⑤返回MY_FILE结构体

```c
MY_FILE *myfopen(const char *path, const char *mode)
  {
    //1.文件的打开方式
    int flag = 0;
    if(strcmp(mode,"w") == 0)       flag |= (O_CREAT | O_TRUNC | O_WRONLY);
    else if(strcmp(mode,"r") == 0)  flag |= O_RDONLY;
    else if(strcmp(mode, "a") == 0) flag |= (O_CREAT | O_WRONLY| O_APPEND);
    else 
    {
      //其余情况
      //"a+" "w+" 等等
    }
  
    //2.尝试打开文件
    mode_t m = 0666;
    int fd = 0;
    if(flag & O_CREAT) fd = open(path, flag, m);
    else fd = open(path, flag);
    if(fd < 0) return NULL;
   //3.构建MY_FILE结构体
    MY_FILE* fp = (MY_FILE*)malloc(sizeof(MY_FILE));                                                                                                                                          
    if(fp == NULL) 
    {
      close(fd);
      return NULL;
    }
    
    //4.初始化MY_FILE结构体
    fp->fd = fd;
    fp->flag = 0;
    fp->flag |= BUFF_LINE;
    memset(fp->outputbuffer, '\0', sizeof(fp->outputbuffer));
  
    //返回MY_FILE结构体
    return fp;
  }
```

### 1.3myfclose的实现

myfclose主要完成以下四个任务：

- ①清空缓冲区
- ②关闭文件
- ③释放堆区的空间
- ④指针值空

```c
  int myfclose(MY_FILE *fp)
  {
    //1.清空缓冲区
    myfflush(fp);
    //2.关闭文件
    close(fp->fd);
    //3.释放堆区的空间
    free(fp);
    //4.指针置空
    fp = NULL;
    return 0;
  } 
```

### 1.4 myfwrite 的实现

分以下四点：

- ①判断缓冲区是否已满，满了就直接刷新缓冲区

- ②根据缓冲区的空间进行数据拷贝

- ③分两种情况向缓冲区中写入：

  a.缓冲区中的空间大于当前要写入缓冲区的空间大小

  b.缓冲区中的空间小于当前要写入缓冲区的空间大小

- ④根据刷新方式进行刷新：

  a.全刷新

  b.行刷新

```c
size_t myfwrite(const void *ptr, size_t size, size_t nmemb,MY_FILE *stream)
{
  //1.如果缓冲区满了就直接刷新缓冲区
  if(stream->current == NUM) myfflush(stream);

  //2.根据缓冲区的空间进行数据拷贝
  size_t user_size = size * nmemb;  //要导入缓冲区的空间大小
  size_t my_size = NUM - stream->current; //缓冲区中剩余的大小
  
  size_t writen = 0;    //写入缓冲区的字节数

  //如果缓冲区的剩余空间够写入
  if(my_size >= user_size)
  {
    memcpy(stream->outputbuffer + stream->current, ptr, user_size);
    stream->current += user_size;
    writen = user_size;
  }
 else
  {
    memcpy(stream->outputbuffer + stream->current, ptr, my_size);                                                                                
    stream->current += my_size;
    writen = my_size;
  }

  //计划刷新缓冲区
  //全刷新
  if(stream->flag & BUFF_ALL)
  {
    if(stream->current == NUM) myfflush(stream);
  }
  else if(stream->flag & BUFF_LINE)
  {
    if(stream->outputbuffer[stream->current - 1] == '\n') 
    {
      myfflush(stream);
    }
  }
  else
  {
    //..
  }

  //返回的是写入成功的字节数
  return writen;
}
```

### 1.5myfflush函数的实现

写入后将当前缓冲区占用空间置零即可！

```c
int myfflush(MY_FILE* fp)
{
  assert(fp); 
  write(fp->fd, fp->outputbuffer, fp->current);  
  fp->current = 0;
  return 0;
}
```

到这里我们就可以实现一些简答你的IO:

![image-20231116161713750](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231116161713750.png)

### 1.6那么我们为什么需要缓冲区？

原因是IO在计算机中是非常占用时间的，所以我们只能减少IO的次数，以提高计算机的运行效率，我们将多次的数据存储在一个缓冲区中，然后一次IO多次的数据，达到了提高计算机运行效率的目的。

那么IO的本质就是和外设做交互，和显示器做交互

## 2.关于缓冲区

我们之前所谈的缓冲区都是：用户级缓冲区。

我们接下来建立一个完整的理解:用户层+内核的理解

实际上我们的工作模式是：

当我们每次打开一个文件的时候，是我们的进程在访问我们的文件，进程需要有文件访问描述符表和打开的文件，文件自带缓冲区。磁盘，我们访问文件的时候一定要有路径。有路径就可以标识磁盘中的某一个位置从而访问到我们的文件。我们使用的C语言，实际上它是一个C标准库中有一个叫`libc.a/so`的。那么这些库中给我提供了很多方法。我们平时写的代码中会涉及到`fwrite/fputs/fprintf...`这类的函数。然而这些的函数都是需要一个叫`FILE*`参数帮我们去做的。而这个`FILE*`参数是当我们调用`open`接口的时候，由库给我们创建的。实际上缓冲区是进程帮我们在我们堆空间帮我们申请的。那么我们可以理解为缓冲区在我们的C库里面，也可以认为在我们的进程的堆空间内部是一样的。我们这里就放在库里面去理解。我们这里所说的一系列函数`fwrite/fputs/fprintf...`实际上是我们的拷贝函数，他们帮我们把对应的数据拷贝到我们的缓冲区里。然后在结合打开的文件的类型，以及我们对于缓冲区的使用情况的刷新策略。然后定期的把我们的数据，从我们的缓冲区里面结合操作系统提供的`write()`系统调用接口，来帮我们完成将缓冲区中的信息写入到我们的文件内部。所以其实`write()`的工作其实也是拷贝函数！。那么之前`fwrite/fputs/fprintf...`这一系列函数是帮我们把信息**从用户到语言的库的缓冲区**内部。那么`write()`函数的是从语言的库的缓冲区到我们的操作系统的内核。所以当我们的数据到我们的缓冲区之后：**操作系统就会有自己的刷新策略了**，并且操作系统的刷新策略一定要更加的复杂。

![image-20231117124420926](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20231117124420926.png)

那么最后总结一下：

我们将一个信息从输入计算机到刷入到我们的硬件上，**至少**要经过三步：

- ①在C/C++上先刷新到我们的语言缓冲区内部
- ②通过系统调用将数据拷贝到我们的内存
- ③将数据从内存拷贝到我们的外设(磁盘)

如果我想强制刷新我们的内核怎么办？

### 2.1fsync接口

这个接口的作用是：函数把 fd 对应的文件数据以及文件属性信息（inode等信息）写入到磁盘，并且等待写磁盘操作完成后而返回。意思就是我们强制让操作系统刷新缓冲区，并且把数据写入到磁盘。

我们在myfflush函数的内部加上：

```c
int myfflush(MY_FILE* fp)      
{            
  assert(fp);       
  write(fp->fd, fp->outputbuffer, fp->current);        
  fp->current = 0;      
             
  fsync(fp->fd);    
  return 0;    
} 
```

当我们给写入文件的信息不加`\n`的时候：

![动态效果](C:/Users/jason/Desktop/动态效果.gif)

我们设定为5秒一次刷新：

![动态效果1](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/%E5%8A%A8%E6%80%81%E6%95%88%E6%9E%9C1.gif)

---

**那么我们平时使用的`printf`实际上不管我们让它输入的是什么格式整数浮点数，其实最终都转换成了字符串的形式！**那么格式转化是谁做的呢？在哪里做的呢？实际上也是`printf`帮我们做的，它内部有很多的参数，叫格式控制块：`%d%s`等等！

### 2.2printf的执行思路

我们只需要完成对应的以下四点即可：

- ①先获取对应的变量

- ②定义缓冲区，对变量转成字符串

  a.fwrite(stdout, str);

- ③将字符串拷贝的`stdout->buffer`，即可

- ④结合刷新策略显示即可

### 2.3scanf的执行思路

实际上scanf是先将我们输入的信息存储到一个叫`stdin->buffer`。然后对我们的`buffer`内容进行格式化，然后将格式化的内容写入到我们的变量中。

- ①调用`read(0, stdin->buffer, num)`从0号文件读取到我们的`stdin->buffer`内部。
- ②紧接着扫描我们输入的字符串：碰到空格就将字符串分割成两个子串，*ap = atoi(str1); *bp = atoi(str2);

## 3.完整代码

mystdio.c

```c
#include"mystdio.h"    
    
    
#define NUM 1024    
    
    
    
int myfflush(MY_FILE* fp)    
{    
  assert(fp);    
  write(fp->fd, fp->outputbuffer, fp->current);    
  fp->current = 0;    
    
  fsync(fp->fd);                                                                      
  return 0;    
}    
    
MY_FILE *myfopen(const char *path, const char *mode)    
{    
  //1.文件的打开方式    
  int flag = 0;    
  if(strcmp(mode,"w") == 0)       flag |= (O_CREAT | O_TRUNC | O_WRONLY);    
  else if(strcmp(mode,"r") == 0)  flag |= O_RDONLY;    
  else if(strcmp(mode, "a") == 0) flag |= (O_CREAT | O_WRONLY| O_APPEND);    
  else    
  {    
    //其余情况    
    //"a+" "w+" 等等    
  }    
    
  //2.尝试打开文件    
  mode_t m = 0666;    
  int fd = 0;    
  if(flag & O_CREAT) fd = open(path, flag, m);    
  else fd = open(path, flag);    
  if(fd < 0) return NULL;    
 //3.构建FILE结构体
  MY_FILE* fp = (MY_FILE*)malloc(sizeof(MY_FILE));
  if(fp == NULL) 
  {
    close(fd);
    return NULL;
  }
  
  //4.初始化FILE结构体
  fp->fd = fd;
  fp->flag = 0;
  fp->flag |= BUFF_LINE;
  memset(fp->outputbuffer, '\0', sizeof(fp->outputbuffer));

  //返回MY_FILE结构体
  return fp;
}

size_t myfwrite(const void *ptr, size_t size, size_t nmemb,MY_FILE *stream)
{
  //1.如果缓冲区满了就直接刷新缓冲区
  if(stream->current == NUM) myfflush(stream);

  //2.根据缓冲区的空间剩余清空进行数据拷贝
  size_t user_size = size * nmemb;  //要导入缓冲区的空间大小
  size_t my_size = NUM - stream->current; //缓冲区中剩余的大小
  
  size_t writen = 0;    //写入缓冲区的字节数

  //如果缓冲区的剩余空间够写入
  if(my_size >= user_size)
  {
    memcpy(stream->outputbuffer + stream->current, ptr, user_size);
    stream->current += user_size;
    writen = user_size;
  }
  else
  {
    memcpy(stream->outputbuffer + stream->current, ptr, my_size);
    stream->current += my_size;
    writen = my_size;
  }

  //计划刷新缓冲区
  //全刷新
  if(stream->flag & BUFF_ALL)
  {
    if(stream->current == NUM) myfflush(stream);
  }
  else if(stream->flag & BUFF_LINE)
  {
    if(stream->outputbuffer[stream->current - 1] == '\n') 
     myfflush(stream);
    }                                                                                 
  }
  else
  {
    //..
  }

  //返回的是写入成功的字节数
  return writen;
}

int myfclose(MY_FILE *fp)
{
  //1.清空缓冲区
  myfflush(fp);
  //2.关闭文件
  close(fp->fd);
  //3.释放堆区的空间
  free(fp);
  //4.指针置空
  fp = NULL;
  return 0;
}
```

mystdio.h

```c
#pragma once    
    
#include<stdio.h>    
#include<sys/types.h>    
#include<sys/stat.h>    
#include<fcntl.h>    
#include<string.h>    
#include<stdlib.h>    
#include<unistd.h>    
#include<assert.h>    
    
#define MAX 1024    
#define BUFF_NONE 0x1    
#define BUFF_LINE 0x2    
#define BUFF_ALL  0x4    
    
    
typedef struct _MY_FILE    
{    
  int fd;    
  char outputbuffer[MAX];                                                             
  int flag; //flash method    
  int current; // flash size    
} MY_FILE;    
    
    
    
    
int myfflush(MY_FILE* fp);    
MY_FILE *myfopen(const char *path, const char *mode);    
size_t myfwrite(const void *ptr, size_t size, size_t nmemb,MY_FILE *stream);    
int myfclose(MY_FILE *fp); 
```

main.c

```c
#include"mystdio.h"    
    
    
#define MYFILE "log.txt"    
    
int main()    
{    
    
  MY_FILE* fp = myfopen(MYFILE, "w");    
    
  const char* str = "hello my write";    
    
  //操作file    
  int cnt = 500;                                                                      
  while(cnt)    
  {    
    char buffer[1024];    
    snprintf(buffer, sizeof(buffer), "%s:%d", str, cnt--);    
   // printf("%s", buffer);    
    size_t size = myfwrite(buffer,strlen(buffer), 1, fp);    
    printf("我写入了%lu个字节\n", size);    
    
    if(cnt % 5 == 0) myfflush(fp);    
    sleep(1);    
  }    
    
    
    
  myfclose(fp);    
}
```

