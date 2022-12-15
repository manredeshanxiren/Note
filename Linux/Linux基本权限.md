# Linux权限(1)

主要内容：

重点讲解了权限的概念，用户的分类，文件的属性，文件的权限，用户的切换，如何修改文件属性，角色的划分等；

## 1. Linux权限的概念  

> 权限是用来限制人的，访问的对象天然可能没有这种“属性” ;
>
> 权限：一件事情是否允许被谁“做”;
>
> 权限 = 人 + 事物属性;

### 1.1 Linux上的用户分类(人):

> - root超级用户：只有一个，在Linux中可以干任何事情;
> - 普通用户：可以有多个，但是有众多限制;

#### 1.1.1如何切换用户:

这里我们要用到`su`命令：

>命令：` su  [用户名]`   //同一个bash下进行身份切换  ----> <u>**还在原来的目录**</u>
>
>命令：`su -`    //切换到root，并且重新登录root  ----> **<u>回到家目录</u>**
>
>功能：切换用户。

```apl
[root@iZ2ze8prly56kg8in4hqa2Z learn]# su mi  //切换到用户mi
[mi@iZ2ze8prly56kg8in4hqa2Z learn]$ exit	 //ctrl + d 退出
```

`如果知道root的密码，那么就可以变成当前Linux内的任何一个普通用户`

1.1.2 对单个指令进行提权：

> 命令:  sudo  command  ---

```apl
[root@iZ2ze8prly56kg8in4hqa2Z learn]# chmod 000 test.c   //关掉test.c的所有权限
[root@iZ2ze8prly56kg8in4hqa2Z learn]# su mi
[mi@iZ2ze8prly56kg8in4hqa2Z learn]$ cat test.c
cat: test.c: Permission denied
[mi@iZ2ze8prly56kg8in4hqa2Z root]$ sudo cat test.c
#include<stdio.h>
int main()
{
	printf("hello world");
	return 0;
}
```

### 1.3 文件的类型和访问权限 (事物属性):

我们在一个目录下执行`ll`命令的时候会出现：

![](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221215125644046.png)

这里面包含了文件的多个属性,接下来我们就来介绍一下这些分别代表什么:

> - r---> 可读
>
> - w---> 可写
>
> - x---> 可执行

![image-20221215131445325](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221215131445325.png)

#### 1.3.1文件类型:

> **d：文件夹**
> **-：普通文件**
> l：软链接（类似Windows的快捷方式）
> b：块设备文件（例如硬盘、光驱等）
> p：管道文件
> c：字符设备文件（例如屏幕,键盘等）
> s：套接口文件    

Linux中不用文件后缀区分文件类型，而是用文件属性中第一列的第一个字符来区分文件类型:

但是不代表编译器，或者其他软件不需要后缀;

例如：

> ①当我们把`gcc`编译出来的`a.out`文件的后缀修改成`.txt`的时候还是可以运行的
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# gcc test.c 
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# ll
> total 20
> -rwxr-xr-x 1 root root 12744 Dec 15 13:34 a.out
> drwxr-xr-x 3 root root    18 Dec 14 21:12 dir
> drwxr-xr-x 5 root root    69 Dec 14 22:04 mydir
> drwxr-xr-x 7 root root    82 Dec 14 20:51 temp
> -rw-r--r-- 1 root root    68 Dec 15 13:34 test.c
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# mv a.out a.txt
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# ll
> total 20
> -rwxr-xr-x 1 root root 12744 Dec 15 13:34 a.txt
> drwxr-xr-x 3 root root    18 Dec 14 21:12 dir
> drwxr-xr-x 5 root root    69 Dec 14 22:04 mydir
> drwxr-xr-x 7 root root    82 Dec 14 20:51 temp
> -rw-r--r-- 1 root root    68 Dec 15 13:34 test.c
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# ./a.txt
> hello world
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# 
> ```
>
> ②当我们把`test.c`的后缀改成`.txt`的时候却不可以被`gcc`识别了:
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# mv test.c test.txt
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# ll
> total 20
> -rwxr-xr-x 1 root root 12744 Dec 15 13:34 a.out
> drwxr-xr-x 3 root root    18 Dec 14 21:12 dir
> drwxr-xr-x 5 root root    69 Dec 14 22:04 mydir
> drwxr-xr-x 7 root root    82 Dec 14 20:51 temp
> -rw-r--r-- 1 root root    68 Dec 15 13:34 test.txt
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# gcc test.txt 
> test.txt: file not recognized: File format not recognized
> collect2: error: ld returned 1 exit status
> ```

#### 1.3.2角色划分：

> ①文件拥有者
>
> ②文件的other
>
> ③文件所属组：
>
> 文件可以属于一个组内的所有成员，这种管理方法有利于企业的开发;

#### 1.3.3文件权限值的表示方法:

#### 1.3.3.1   字符表示方法:

![image-20221215170643940](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221215170643940.png)

####  1.3.3.2  八进制数值表示方法:

![image-20221215170632515](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221215170632515.png)

### 1.4如何修改权限：

#### 1.4.1修改文件的权限:

 chmod 命令

>功能： 设置文件的访问权限
>格式： chmod [参数] 权限 文件名  

常用选项：

> - R -> 递归修改目录文件的权限
> - 说明：只有文件的拥有者和root才可以改变文件的权限  

chmod命令权限值的格式:

> - +:向权限范围增加权限代号所表示的权限
> - -:向权限范围取消权限代号所表示的权限
> - =:向权限范围赋予权限代号所表示的权限
> - 用户符号：
> - u：拥有者
> - g：拥有者同组用
> - o：其它用户
> - a：所有用户  

代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# ll
total 8
-rw-r--r-- 1 root root 59 Dec 13 19:22 my.txt
---------- 1 root root 68 Dec 15 13:34 test.c
[root@iZ2ze8prly56kg8in4hqa2Z ~]# chmod u+r test.c //给所属者加上可读权限
[root@iZ2ze8prly56kg8in4hqa2Z ~]# ll
total 8
-rw-r--r-- 1 root root 59 Dec 13 19:22 my.txt
-r-------- 1 root root 68 Dec 15 13:34 test.c
[root@iZ2ze8prly56kg8in4hqa2Z ~]# chmod g+rw test.c //给所属组加上读写权限
[root@iZ2ze8prly56kg8in4hqa2Z ~]# ll
total 8
-rw-r--r-- 1 root root 59 Dec 13 19:22 my.txt
-r--rw-r-- 1 root root 68 Dec 15 13:34 test.c

[root@iZ2ze8prly56kg8in4hqa2Z ~]# chmod o+rwx test.c //给other加上读，写，执行权限
[root@iZ2ze8prly56kg8in4hqa2Z ~]# ll
total 8
-rw-r--r-- 1 root root 59 Dec 13 19:22 my.txt
-r--rw-rwx 1 root root 68 Dec 15 13:34 test.c

[root@iZ2ze8prly56kg8in4hqa2Z ~]# chmod u-rwx,g-rwx,o-rwx test.c  //去掉所有权限
[root@iZ2ze8prly56kg8in4hqa2Z ~]# ll
total 8
-rw-r--r-- 1 root root 59 Dec 13 19:22 my.txt
---------- 1 root root 68 Dec 15 13:34 test.c

[root@iZ2ze8prly56kg8in4hqa2Z ~]# chmod 000 test.c       //去掉所有权限
[root@iZ2ze8prly56kg8in4hqa2Z ~]# ll
total 8
-rw-r--r-- 1 root root 59 Dec 13 19:22 my.txt
---------- 1 root root 68 Dec 15 13:34 test.c

```

#### 1.4.2修改文件的所有者:

chown命令:

> - 功能：修改文件的拥有者
> - 格式： chown [参数] 用户名 文件名  

代码演示：

```
[root@iZ2ze8prly56kg8in4hqa2Z ~]# chown mi test.c 
[root@iZ2ze8prly56kg8in4hqa2Z ~]# ll
total 8
-rw-r--r-- 1 root root 59 Dec 13 19:22 my.txt
---------- 1 mi   root 68 Dec 15 13:34 test.c
```

#### 1.4.3修改文件所属组:

chgrp命令:

> - 功能：修改文件或目录的所属组
> - 格式： chgrp [参数] 用户组名 文件名  

常用选项：

> -  -R 递归修改文件或目录的所属组  

代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# ll
total 8
-rw-r--r-- 1 root root 59 Dec 13 19:22 my.txt
---------- 1 mi   root 68 Dec 15 13:34 test.c
[root@iZ2ze8prly56kg8in4hqa2Z ~]# chgrp mi test.c //将test.c的文件所属组修改为mi
[root@iZ2ze8prly56kg8in4hqa2Z ~]# ll
total 8
-rw-r--r-- 1 root root 59 Dec 13 19:22 my.txt
---------- 1 mi   mi   68 Dec 15 13:34 test.c

```

