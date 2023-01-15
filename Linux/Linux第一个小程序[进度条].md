# Linux第一个小程序[进度条]

## Ⅰ. `\r`和`\n`:

> C语言中有很多字符:简单来说分为两种`a"可显字符"` a b c 等等，`b"控制字符"` \r \n 等等;
>
> `\r`:回车(光标来到行首)
>
> `\n`:换行

## Ⅱ. 行缓冲区:

> ```c
> #include <stdio.h>
> int main()
> {
> 	printf("hello Makefile!");
> 	sleep(3);
> 	return 0;
> }
> ```
>
> ①当我们执行这段代码的时候我们发现，程序一开始什么都没有显示，而是等待了3秒之后才显示的`hello Makefile!`
>
> ```c
> #include <stdio.h>
> int main()
> {
> 	printf("hello Makefile!\n");
> 	sleep(3);
> 	return 0;
> }
> ```
>
> ②当我们执行这段代码的时候我们发现，程序先是打印出了`hello Makefile!`然后等待了3秒之后程序才执行结束。
>
> ```c
> #include <stdio.h>
> int main()
> {
> 	printf("hello Makefile!");
> 	fflush(stdout);
> 	sleep(3);
> 	return 0;
> }
> ```
>
> ③当我们执行这段代码的时候，我们发现这个程序执行的现象和第②段代码执行的结果相同。

为什么会出现以上的情况呢？

这是因为有一个行缓冲区的概念,当我们用printf输出的时候，程序先是把将要输出的结果临时存储在行缓冲区中，当刷新行缓冲区的时候才会将输出的结果打印到屏幕上。那么上面代码中的`\n`和`fflush(stdout)`都具有可以刷新行缓冲区的作用。

其中`\n`的作用就是换行，所以会执行后，会产生刷新行缓冲区的作用。

而`fflush(srdout)`的功能本来就是刷新行缓冲区的。

## Ⅲ.进度条的代码实现:

实现效果:

![进度条](D:\笔记\Note\数据结构\进度条.gif)

代码:

proc.c文件:

```c
#include"proc.h"
void process()
{
	char bar[SIZE];
  	char Bar[SIZE];
 	const char* arr = "|/-\\";
  	memset(bar,'\0', sizeof(bar));
    memset(Bar,' ',sizeof(Bar));
  	int i = 0;
    srand((unsigned int)time(NULL));
 	fflush(stdout);
 	while(i <= 100)
 	{
 		printf("[%d%%][%c]\033[42;32m%s\033[0m\r",i,arr[i%4],bar);
 		fflush(stdout);
 	    bar[i++] = ' ';
 	    usleep(100000);
 	 }
 		printf("\n");
 	}  
```

proc.h文件:

```c
#pragma once
  
   
   #include <stdio.h>
   #include <string.h>
   #include <unistd.h>
   #include <time.h>
   #include <stdlib.h>                                                                                                        
   #define SIZE 101
   extern void process();
```
