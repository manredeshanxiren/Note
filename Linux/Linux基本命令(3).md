























# Linux基本命令(3)

## 1.tar命令:

> - 语法：tar [-cxtzjvf] 文件与目录 ..... 参数  
>
>   注意：有些平台不需要加 `-`加上之后会报错
>
> - 功能：打包/解包，不打开它，直接看内容  

### 1.1常用选项：

> - -c ：建立一个压缩文件的参数指令(create 的意思)；
> - -x ：解开一个压缩文件的参数指令！
> - -t ：查看 tarfile 里面的文件！
> - -z ：是否同时具有 gzip 的属性？亦即是否需要用 gzip 压缩？
> - -j ：是否同时具有 bzip2 的属性？亦即是否需要用 bzip2 压缩？
> - -v ：压缩的过程中显示文件！这个常用，但不建议用在背景执行过程！
> - -f ：使用档名，请留意，在 f 之后要立即接档名喔！不要再加参数！
> - -C ： 解压到指定目录  

### 1.2代码演示：

（1）打包temp/解压temp

```apl
[root@iZ2ze8prly56kg8in4hqa2Z learn]# tar -czf temp.tgz temp
[root@iZ2ze8prly56kg8in4hqa2Z learn]# ll
total 16
-rw-r--r-- 1 root root 9901 Dec 13 15:57 dir
drwxr-xr-x 4 root root   26 Dec 13 20:28 mydir
drwxr-xr-x 7 root root   82 Dec 14 20:51 temp
-rw-r--r-- 1 root root  195 Dec 14 20:51 temp.tgz
-rw-r--r-- 1 root root    0 Dec 12 22:06 test.c
[root@iZ2ze8prly56kg8in4hqa2Z /]# mv temp.tgz learn/mydir/
[root@iZ2ze8prly56kg8in4hqa2Z mydir]# tar -xzf temp.tgz
[root@iZ2ze8prly56kg8in4hqa2Z mydir]# ll
total 4
drwxr-xr-x 3 root root  15 Dec 13 20:28 a
drwxr-xr-x 2 root root  20 Dec 12 21:23 dir
drwxr-xr-x 7 root root  82 Dec 14 20:51 temp
-rw-r--r-- 1 root root 195 Dec 14 20:51 temp.tgz
[root@iZ2ze8prly56kg8in4hqa2Z mydir]# tree temp
temp
├── test.c
├── test.cpp
├── test.py
├── test.sh
└── test.txt

5 directories, 0 files

```

（2）不解压查看压缩包中的东西：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z mydir]# tar -ztvf temp.tgz
drwxr-xr-x root/root         0 2022-12-14 20:51 temp/
drwxr-xr-x root/root         0 2022-12-14 20:48 temp/test.c/
drwxr-xr-x root/root         0 2022-12-14 20:48 temp/test.cpp/
drwxr-xr-x root/root         0 2022-12-14 20:48 temp/test.py/
drwxr-xr-x root/root         0 2022-12-14 20:48 temp/test.sh/
drwxr-xr-x root/root         0 2022-12-14 20:51 temp/test.txt/


[root@iZ2ze8prly56kg8in4hqa2Z mydir]# tar -ztf temp.tgz
temp/
temp/test.c/
temp/test.cpp/
temp/test.py/
temp/test.sh/
temp/test.txt/

```

(3)解包到指定路径：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z mydir]# tar xzf temp.tgz -C ../dir
[root@iZ2ze8prly56kg8in4hqa2Z mydir]# ll ../dir
total 0
drwxr-xr-x 7 root root 82 Dec 14 20:51 temp
```

## 2.bc命令：

> 方便的进行浮点运算

### 2.1代码演示:

```apl
[root@iZ2ze8prly56kg8in4hqa2Z mydir]# bc
bc 1.07.1
Copyright 1991-1994, 1997, 1998, 2000, 2004, 2006, 2008, 2012-2017 Free Software Foundation, Inc.
This is free software with ABSOLUTELY NO WARRANTY.
For details type `warranty'. 
1+1
2
2+2
4
pi+3
3
3.1232144+4.21312
7.3363344
^C
(interrupt) Exiting bc.
```

## 3.uname –r指令:

> 语法： uname [选项]
> 功能： uname用来获取电脑和操作系统的相关信息  
>
> 补充说明： uname可显示linux主机所用的操作系统的版本、硬件的名称等基本信息  

### 3.1常用选项:

> -a或–all 详细输出所有信息，依次为内核名称，主机名，内核版本号，内核版本，硬件名，处理器类
> 型，硬件平台类型，操作系统名称  

### 3.2代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z mydir]# uname -a
Linux iZ2ze8prly56kg8in4hqa2Z 4.18.0-193.14.2.el8_2.x86_64 #1 SMP Sun Jul 26 03:54:29 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux
```

内容含义：

![image-20221214215148977](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221214215148977.png)

## 4热键(2)：

1.`crtl + r` : 搜索之前输入过的命令：

![image-20221214215803469](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221214215803469.png)

2.`⬆ 和 ⬇`: 可以按时间顺序展示之前输入过的命令；



3.输入`history`查看全部的历史命令：

![image-20221214220247229](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221214220247229.png)

3.1`history > cmd.txt` 将历史命令存在cmd.txt中：

![image-20221214220516424](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221214220516424.png)

## 5.关机：

> 语法： shutdown [选项] ** 常见选项： **  

### 5.1常用选项:

> - -h   //将系统的服务停掉之后，立即<u>**关机**</u>
> - -r   //将系统的服务停掉之后，立即<u>**重启**</u>
> - -t sec // -t 后面加秒数，亦即『过几秒后关机』的意思  

## 6.shell命令以及运行原理  

shell是什么：

> 通俗的讲：shell是一个外壳程序，包裹在操作系统外部的软件层，核心功能就是<u>**命令行解释**</u>或者叫做<u>**操作解释**</u>

shell存在的意义：

> a.帮忙进行命令行传递和返回结果
>
> b.保护操作系统

Windows GUI：

> 我们操作windows 不是直接操作windows内核，而是通过图形接口，点击，从而完成我们的
> 操作（比如进入D盘的操作，我们通常是双击D盘盘符.或者运行起来一个应用程序） ； 

![image-20221214225024177](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221214225024177.png)



## 7.file命令：

> - 功能说明：辨识文件类型。
>
> - 语法： file [选项] 文件或目录 .... 

### 7.1常用选项:

> - -c 详细显示指令执行过程，便于排错或分析程序执行的情形。
>
> - -z 尝试去解读压缩文件的内容  

### 7.2代码演示:

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# file my.txt  //文本
my.txt: ASCII text
[root@iZ2ze8prly56kg8in4hqa2Z ~]# file test.c  //C的源程序
test.c: C source, ASCII text
[root@iZ2ze8prly56kg8in4hqa2Z mydir]# file temp.tgz  //tgz的压缩包
temp.tgz: gzip compressed data, last modified: Wed Dec 14 12:51:59 2022, from Unix, original size 10240
```

