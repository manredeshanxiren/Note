# Linux基本权限(2)

## 1.目录的权限:

> 对于目录和文件是类似的也有权限的限制：

![目录的权限](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221216203249546.png)

> - `x`可执行权限: 如果目录没有可执行权限, 则无法 cd 到目录中  
>
> - `r`可读权限: 如果目录没有可读权限, 则无法用ls等命令查看目录中的文件内容.
>
> - `w`可写权限: 如果目录没有可写权限, 则无法在目录中<u>**创建文件**</u>, 也无法在目录中<u>**删除文件**</u>  

## 2.默认权限:

> 起始权限:系统设定的
>
> 默认(最终)权限:我们所看到的

首先我们创建一个文件和一个目录，并观察他们的权限属性:

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# ll
total 0
drwxrwxr-x 2 root root 6 Dec 17 12:49 new_dir
-rw-rw-r-- 1 root root 0 Dec 17 12:49 test.txt

```

> 为什么普通文件的默认的权限是从664开始的？
>
> 为什么目录的默认权限是775开始的？
>
> <u>**不同的操作系统默认的权限可能是不同的;**</u>

![umask](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221217132457489.png)

> 普通文件的<u>**起始权限**</u>是从`666`开始的;
>
> 目录文件的<u>**起始权限**</u>是从`777`开始的;

### 2.1 umask掩码:

系统为了更好的控制文件权限,系统会有默认的权限`掩码`的概念！`umask`

前面提到每个操作系统的默认权限都是不同的这就是由`umask`决定的(因为我们的默认权限是由初始权限和`umask`进行如上图的<u>**位运算**</u>的得到的)，那么我们如何查看我们的`umask`权限掩码是多少呢?

再Linux命令行中输入`umask`即可;

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# umask
0002
```

这里的`0002`是什么意思：

> 第一个0表示八进制位;
>
> 剩下的三个数字分别对应文件权限属性中的三个权限:
>
> 所属者权限，所属组权限还有other权限;

运算过程(以目录为例):

![image-20221217144838754](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221217144838754.png)

最终的结果转化成八进制就是 775,这也回答了上面的两个问题为什么文件默认是`664`， 目录是`775`;

### 2.2修改`umask`:

> 语法: umask   八进制数(例如 `0002`)

演示:

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# umask 0003
[root@iZ2ze8prly56kg8in4hqa2Z dir]# umask
0003
[root@iZ2ze8prly56kg8in4hqa2Z dir]# umask 0002
[root@iZ2ze8prly56kg8in4hqa2Z dir]# umask
0002
```

## 3.粘滞位:

> 背景: 在使用Linux的时候,未来可能会有一些共享目录，被所有的普通用户共享，用来保存普通用户产生的临时数据;
>
> 准备:共享的目录通常的都是`root`提供的，文件被所有的人共享的时候，受权限约束，但是拦不住别人删我的文件(即使是other没有rwx权限也可以删除)！！
>
> 为了让大家共享文件，并且不让其他人随便删除别人的文件，引入`粘滞位`
>
> `粘滞位`只能给目录添加;

创建共享文件:

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# mkdir public
[root@iZ2ze8prly56kg8in4hqa2Z dir]# ll
total 0
drwxrwxr-x 2 root root 6 Dec 17 12:49 new_dir
drwxrwxr-x 2 root root 6 Dec 17 15:59 public
-rw-rw-r-- 1 root root 0 Dec 17 12:49 test.txt
[root@iZ2ze8prly56kg8in4hqa2Z dir]# chmod 777 public
[root@iZ2ze8prly56kg8in4hqa2Z public]# touch root1
[root@iZ2ze8prly56kg8in4hqa2Z public]# touch root2
[root@iZ2ze8prly56kg8in4hqa2Z public]# touch root3
```

尝试删除:

```apl
[root@iZ2ze8prly56kg8in4hqa2Z public]# ll
total 0
-rw-r--r-- 1 root root 0 Dec 17 16:21 root1
-rw-rw-r-- 1 root root 0 Dec 17 16:08 root2
-rw-rw-r-- 1 root root 0 Dec 17 16:09 root3
[root@iZ2ze8prly56kg8in4hqa2Z public]# rm root1
rm: remove regular empty file 'root1'? y
[root@iZ2ze8prly56kg8in4hqa2Z public]# ll
total 0
-rw-rw-r-- 1 root root 0 Dec 17 16:08 root2
-rw-rw-r-- 1 root root 0 Dec 17 16:09 root3
```

我们发现用普通用户是可以删除的;

### 3.1 如何添加粘滞位:

> 语法：`chmod +t public`

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# chmod +t public
[root@iZ2ze8prly56kg8in4hqa2Z dir]# ll
total 0
drwxrwxr-x 2 root root  6 Dec 17 12:49 new_dir
drwxrwxrwt 2 root root 45 Dec 17 16:09 public
-rw-rw-r-- 1 root root  0 Dec 17 12:49 test.txt
```

这时候我们使用普通用户尝试去删除`public`中的文件的时候是不能删除的:

```apl
[root@iZ2ze8prly56kg8in4hqa2Z public]# su mi
[mi@iZ2ze8prly56kg8in4hqa2Z public]$ rm root1
rm: remove write-protected regular empty file 'root1'? y
rm: cannot remove 'root1': Operation not permitted
```

### 3.2添加粘滞位之后的效果:

> 前面我们提到如果`other`没有没有`w`权限的话，那么我们就不能删除文件了，但是我们也不能在`public`目录中创建文件了。
>
> 所以我们给`public`添加粘滞位后的效果:
>
> ①每个普通用户都可以在`public`目录中任意的添加文件;
>
> ②那么谁能删除文件呢?
>
> (1)`root`；
>
> (2)文件的所属者
>
> (3)该目录的所属者

------

![image-20221217164547298](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221217164547298.png)
