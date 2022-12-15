# Linux基本命令（1）

[TOC]

## 1.操作系统：

### （1）什么是操作系统：

> **操作系统是一款进行软硬件管理的<u>软件</u>**

### （2）为什么要有操作系统：

> **对软硬件进行合理的管理。以达到为上层用户提供良好的、稳定的、安全的运行环境的目的**

![image-20221211194350210](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221211194350210.png)

## 2. ls命令:

> 语法： ls [选项] [目录或文件]
> 功能：对于目录，该命令列出该目录下的所有子目录与文件。对于文件，将列出文件名以及其他信息  

常用选项：

> - -a 列出目录下的所有文件，包括以 . 开头的隐含文件。
>
> - -d 将目录象文件一样显示，而不是显示其下的文件。 如： ls –d 指定目录
>
> - -i 输出文件的 i 节点的索引信息。 如 ls –ai 指定文件
>
> - -k 以 k 字节的形式表示文件的大小。 ls –alk 指定文件
>
> - -l 列出文件的详细信息。（**文件  =  内容  + 属性**）
>
> - -n 用数字的 UID,GID 代替名称。 （介绍 UID， GID）
>
> - -F 在每个文件名后附上一个字符以说明该文件的类型， “*”表示可执行的普通文件； “/”表示目录； “@”表
>
>   示符号链接； “|”表示FIFOs； “=”表示套接字(sockets)。（目录类型识别）
>
> - -r 对目录反向排序。
>
> - -t 以时间排序。
>
> - -s 在l文件名后输出该文件的大小。（大小排序，如何找到目录下最大的文件）
>
> - -R 列出所有子目录下的文件。 (递归)
>
> - -1 一行只输出一个文件。  

其中有一些指令可以简写：

例如：

> ls -l  ---> ll
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# ll
> total 4
> drwxr-xr-x 2 root root 4096 Dec 11 13:16 dir
> 
> ```
>
> ls -l -a ---> ls -al  / ls -la  (用于显示一些隐藏文件)
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# ls -al
> total 12
> drwxr-xr-x   3 root root 4096 Dec 11 13:16 .
> dr-xr-xr-x. 19 root root 4096 Nov 23 21:43 ..
> drwxr-xr-x   2 root root 4096 Dec 11 13:16 dir
> 
> ```

**其中`.`和 `..`分别是`当前路径`和`上级路径`**

<u>一般在Linux中.开头命名的文件和文件夹都会被隐藏起来；</u>

> 拓展：`stat`命令:用于获取文件的所有属性
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# stat dir
>   File: ‘dir’
>   Size: 4096      	Blocks: 8          IO Block: 4096   directory
> Device: fd01h/64769d	Inode: 1310723     Links: 2
> Access: (0755/drwxr-xr-x)  Uid: (    0/    root)   Gid: (    0/    root)
> Access: 2022-12-11 13:16:45.620810854 +0800
> Modify: 2022-12-11 13:16:44.161761652 +0800
> Change: 2022-12-11 13:16:44.161761652 +0800
>  Birth: -
> 
> ```

这里展示了三种时间：

> `Access time` : 最后一次 **访问** (仅访问，没有改动)文件的时间
> `Modify time`: 最后一次 **文件属性改变** 的时间，包括权限，大小，属性等
> `Change time`: 最后一次修改 **文件** 的时间

## 3. pwd指令：

> 语法: pwd
> 功能：是Print Working Directory的缩写,显示用户当前所在的目录  ;

代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# pwd
/learn/dir
```

注意:

**<u>这里的路径分隔符是`/`而在windows中采用的分隔符是`\`;</u>**

## 4. cd命令:

> 语法: cd 目录名
> 功能：是change directory的缩写； 改变工作目录。将当前工作目录改变到指定的目录下  

linux目录的结构就是一个多叉树形式：(如下图)

![](https://img-blog.csdnimg.cn/20200318225924376.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0NzIzNzcz,size_16,color_FFFFFF,t_70)

常用的指令:

> `cd .. `: 返回上级目录
> `cd /home/litao/linux/ `: 绝对路径
> `cd ../day02/` : 相对路径
> `cd ~`：进入用户家目录
> `cd -`：返回最近访问目录  

绝对路径和相对路径：

​		表示文件位置的最好方式就是使用 **路径**，因为 **路径具有唯一性**。

> 绝对路径：从根目录开始，定位文件的路径，为绝对路径。(准确但麻烦，无论在哪个目录下都有效)
>
> 相对路径：由当前文件所在的路径引起的跟其它文件（或文件夹）的路径关系。(方便，在不同目录下可能会无效)

代码示例：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# cd ..
[root@iZ2ze8prly56kg8in4hqa2Z learn]# cd -
/learn/dir
[root@iZ2ze8prly56kg8in4hqa2Z dir]# cd ~
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cd /
[root@iZ2ze8prly56kg8in4hqa2Z /]# cd /learn/dir/
[root@iZ2ze8prly56kg8in4hqa2Z dir]# ll
total 0
-rw-r--r-- 1 root root 0 Dec 11 13:16 myfail.c
-rw-r--r-- 1 root root 0 Dec 11 19:03 test.c
```

## 5. touch命令：

> 语法:touch [选项]... 文件...
> 功能： touch命令参数可更改文档或目录的日期时间，包括存取时间和更改时间，或者新建一个不存在的文件  

常用选项:

> -a 或--time=atime或--time=access或--time=use只更改存取时间。
> -c 或--no-create 不建立任何文档。
> -d 使用指定的日期时间，而非现在的时间。
> -f 此参数将忽略不予处理，仅负责解决BSD版本touch指令的兼容性问题。
> -m 或--time=mtime或--time=modify 只更改变动时间。
> -r 把指定文档或目录的日期时间，统统设成和参考文档或目录的日期时间相同。
> -t 使用指定的日期时间，而非现在的时间  

代码演示:

> (1)在当前目录创建一个文件:
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# touch test.c
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# ll
> total 0
> -rw-r--r-- 1 root root 0 Dec 11 13:16 myfail.c
> -rw-r--r-- 1 root root 0 Dec 11 19:47 test.c
> ```
>
> (2)在相对路径下创建一个文件:
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# touch ../newdir/test.c
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# ll ../newdir/
> total 0
> -rw-r--r-- 1 root root 0 Dec 11 19:50 test.c
> ```
>
> (3)在绝对路径下创建一个文件:
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# touch /learn/newdir/test2.c
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# ll  /learn/newdir/
> total 0
> -rw-r--r-- 1 root root 0 Dec 11 19:52 test2.c
> -rw-r--r-- 1 root root 0 Dec 11 19:50 test.c
> ```

## 6. tree命令:

> 功能：使当前目录往后的文件夹和文件以树状结构呈现出来

这个命令需要在root用户下安装：

在root权限下输入：`yum install -y tree`

代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# tree
.
├── myfail.c
└── test.c

```

## 7. mkdir 命令:

> 语法： mkdir [选项]  dirname...
> 功能：是make directory 的缩写;在当前目录下创建一个名为 “dirname”的目录  

常用选项:

> -p, --parents 可以是一个路径名称。此时若路径中的某些目录尚不存在,加上此选项后,系统将自动建立
> 好那些尚不存在的目录,即一次可以建立多个目录;  

代码演示:

> (1)在当前路径下创建文件夹:
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# mkdir newfile
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# ll
> total 4
> -rw-r--r-- 1 root root    0 Dec 11 13:16 myfail.c
> drwxr-xr-x 2 root root 4096 Dec 11 20:08 newfile
> -rw-r--r-- 1 root root    0 Dec 11 19:49 test.c
> ```
>
> (2)递归创建一段路径:
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# mkdir -p a/b/c/d/
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# tree
> .
> ├── a
> │   └── b
> │       └── c
> │           └── d
> ├── myfail.c
> ├── newfile
> └── test.c
> ```

## 8. rmdir 和 rm 指令：

> rmdir是一个与mkdir相对应的命令。 mkdir是建立目录，而rmdir是删除命令。
> 语法： rmdir [-p] [dirName]
> 适用对象：具有当前目录操作权限的所有使用者
> 功能：<u>**删除空目录**</u>  

常用选项：

> -p 当子目录被删除后如果父目录也变成空目录的话，就连带父目录一起删除  

代码演示：

> 删除空目录：
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# cd  a/b/c/d/
> [root@iZ2ze8prly56kg8in4hqa2Z d]# rmdir -p d
> rmdir: failed to remove ‘d’: No such file or directory
> [root@iZ2ze8prly56kg8in4hqa2Z d]# cd ..
> [root@iZ2ze8prly56kg8in4hqa2Z c]# rmdir -p d
> ```

<u>**rm命令可以同时删除文件或目录**</u>  

> 语法： rm [-f-i-r-v][dirName/dir]
> 适用对象：所有使用者
> 功能：<u>**删除文件或目录**</u>  

常用选项：

> -f 即使文件属性为只读(即写保护)，亦直接删除
> -i 删除前逐一询问确认
> -r 删除目录及其下所有文件  

代码演示：

> `rm 文件名`  //只能删除文件不能删除文件夹
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# rm test.c
> rm: remove regular empty file ‘test.c’? y
> ```
>
> `rm -r`  //递归删除，目录(包括 **非空目录**)和文件都能删除
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# rm -r a
> rm: descend into directory ‘a’? y
> rm: descend into directory ‘a/b’? y
> rm: remove directory ‘a/b/c’? y
> rm: remove directory ‘a/b’? y
> rm: remove directory ‘a’? y
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# tree
> .
> ├── myfail.c
> └── newfile
> 
> 1 directory, 1 file
> ```
>
> `rm -rf`  // 无视权限强制删除
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# rm -rf myfail.c
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# ll
> total 4
> drwxr-xr-x 2 root root 4096 Dec 11 20:08 newfile
> ```
>
> `rm -rf ./*` //删除当前目录下文件及其所有子文件
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# mkdir -p a/b/c/d/e
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# tree
> .
> ├── a
> │   └── b
> │       └── c
> │           └── d
> │               └── e
> └── newfile
> 
> 6 directories, 0 files
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# rm -rf ./* 
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# tree
> .
> 
> 0 directories, 0 files
> 
> ```

## 9. nano指令：

一个用于编写代码的命令：

安装nano：

> `yum install -y nano`

语法：

> nano  文件名  // 对文件进行编辑

**注**：使用 `nano` 时右侧小键盘无法使用。

代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# nano test.c  //编辑文件
[root@iZ2ze8prly56kg8in4hqa2Z dir]# gcc test.c  //用gcc编译
[root@iZ2ze8prly56kg8in4hqa2Z dir]# ./a.out   //运行a.out
Hello Linux!				//编译结果
```

![image-20221211213637404](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221211213637404.png)

## 10. clear命令:

> 清理屏幕

## 11. whoami 命令：

> 显示当前用户名称

## 12.常用键位：

`tab` :

功能：只能补全（命令， 显示当前目录的文件）;

> 命令：
>
> 如果忘记了某些命令，但记得命令的一部分，可以使用`tab`来帮助我们：
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# y
> yes           ypdomainname  yum   
> ```
>
> 显示当前目录的文件：
>
> <u>**可以用于书写绝对路径的时候辅助书写；**</u>
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z dir]# cd 
> a.out   test.c  
> ```

`ctrl + c`:

> 语法： 用于终止程序；

`alt + enter`: 

> 全屏

------

