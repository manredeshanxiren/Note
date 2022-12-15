# Linux基本指令(2)

## 1.man指令：

> - Linux的命令有很多参数，我们不可能全记住，我们可以通过查看联机手册获取帮助。访问Linux手册页的命令是
> - man 语法: man [选项] 命令  

安装man手册：

> `yum install -y man-pages`  //root
>
> `sudo yum install -y man-pages` // 普通用户

1.1常用选项：

> - -k 根据关键字搜索联机帮助
> - num 只在第num章节找
> - -a 将所有章节的都显示出来，比如 man printf 它缺省从第一章开始搜索知道就停止，用a选项，当按下q退出，他会继续往后面搜索，直到所有章节都搜索完毕。
> - ![image-20221212215929315](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221212215929315.png)
> - 解释一下,`man`手册分为8章：
> - 1 是普通的命令
> - 2 是系统调用,如open,write之类的(通过这个，至少可以很方便的查到调用这个函数，需要加什么头文
> - 件)
> - 3 是库函数,如printf,fread4是特殊文件,也就是/dev下的各种设备文件
> - 5 是指文件的格式,比如passwd, 就会说明这个文件中各个字段的含义
> - 6 是给游戏留的,由各个游戏自己定义
> - 7 是附件还有一些变量,比如向environ这种全局变量在这里就有说明
> - 8 是系统管理用的命令,这些命令只能由root使用,如ifconfig  

1.2代码演示：

```apl
man fork
```

![image-20221212214816105](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221212214816105.png)

------

## 2.cp命令：

> - 语法： cp [选项] 源文件或目录 目标文件或目录
> - 功能: 复制文件或目录
> - 说明: cp指令用于复制文件或目录，如同时指定两个以上的文件或目录，且最后的目的地是一个已经存在的目录，则它会把前面指定的所有文件或目录复制到此目录中。若同时指定多个文件或目录，而最后的目的地并非一个已存在的目录，则会出现错误信息  

2.1常用选项：

> - -f 或 --force 强行复制文件或目录， 不论目的文件或目录是否已经存在
> - -i 或 --interactive 覆盖文件之前先询问用户
> - -r递归处理，将指定目录下的文件与子目录一并处理。若源文件或目录的形态，不属于目录或符号链接，则一律视为普通文件处理
> - -R 或 --recursive递归处理，将指定目录下的文件及子目录一并处理  

2.2代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z dir]# cp test.c .. //将test.c拷贝到上级目录
[root@iZ2ze8prly56kg8in4hqa2Z dir]# cd ..
[root@iZ2ze8prly56kg8in4hqa2Z learn]# ll
total 0
drwxr-xr-x 2 root root 20 Dec 12 21:23 dir
drwxr-xr-x 2 root root  6 Dec 12 21:23 mydir
-rw-r--r-- 1 root root  0 Dec 12 22:06 test.c
```

------

## 3.mv命令：

> - mv命令是move的缩写，可以用来移动文件或者将文件改名（move (rename) files），是Linux系统下常用的命令，经常用来备份文件或者目录。
>
> - 语法: mv [选项] 源文件或目录 目标文件或目录
> - 功能:
>
> （1）<u>**重命名或者剪切：**</u>视mv命令中第二个参数类型的不同（是目标文件还是目标目录）， mv命令将文件重命名或将其移至一个新的目录中。
>
> （2）<u>**重命名：**</u>当第二个参数类型是文件时， mv命令完成文件重命名，此时，源文件只能有一个（也可以是源目录名），它将所给的源文件或目录重命名为给定的目标文件名。
>
> （3）<u>**剪切：**</u>当第二个参数是已存在的目录名称时，源文件或目录参数可以有多个， mv命令将各参数指定的源文件均移至目标目录中。  

3.1常用选项：

> - -f ： force 强制的意思，如果目标文件已经存在，不会询问而直接覆盖
>
> - -i ：若目标文件 (destination) 已经存在时，就会询问是否覆盖！  

3.2代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z learn]# mv ../dir . //将dir从上级路径移动到当前路径
[root@iZ2ze8prly56kg8in4hqa2Z learn]# ll
total 0
drwxr-xr-x 2 root root 20 Dec 12 21:23 dir
drwxr-xr-x 2 root root  6 Dec 12 21:23 mydir
-rw-r--r-- 1 root root  0 Dec 12 22:06 test.c
[root@iZ2ze8prly56kg8in4hqa2Z learn]# mv dir mydir //把dir移动到 mydir中
[root@iZ2ze8prly56kg8in4hqa2Z learn]# ll mydir
total 0
drwxr-xr-x 2 root root 20 Dec 12 21:23 dir
```

------

## 4.cat命令：

> 语法： cat [选项] [文件]
> 功能： 查看目标文件的内容  

4.1常用选项：

> - -b 对非空输出行编号
> - -n 对输出的所有行编号
> - -s 不输出多行空行  

4.2代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# nano test.txt   //编辑文本内容
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt 	  //用cat命令查看文本内容
123456789    //文本内容
abcdefghi	 //文本内容

```

## 5.echo命令：

> `echo "string" `  //将string输出到屏幕上
>
> `echo "string" > mylog.txt`  //<u>**输出重定向**</u>，将本来输出到屏幕上的string 输出到文件里，如果目标文件不存在，会自动创建；
>
> `echo "string" >> mylog.txt`   //追加重定向

拓展：

> 输出重定向 : `>`
>
> 输入重定向:`<`
>
> 追加重定向:`>>`

所谓输出重定向是一种覆盖写入(准备写入之前，会清空文件)：

```apl
//输出重定向
[root@iZ2ze8prly56kg8in4hqa2Z ~]# echo "hello" > test.txt 
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt
hello
[root@iZ2ze8prly56kg8in4hqa2Z ~]# echo "hello1" > test.txt 
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt
hello1
[root@iZ2ze8prly56kg8in4hqa2Z ~]# echo "hell02" > test.txt 
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt
hell02

//追加重定向
[root@iZ2ze8prly56kg8in4hqa2Z ~]# echo "hell02" > test.txt 
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt
hell02
[root@iZ2ze8prly56kg8in4hqa2Z ~]# echo "hell02" >> test.txt 
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt
hell02
hell02
[root@iZ2ze8prly56kg8in4hqa2Z ~]# echo "hell0" >> test.txt 
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt
hell02
hell02
hell0
[root@iZ2ze8prly56kg8in4hqa2Z ~]# echo "hell0" >> test.txt 
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt
hell02
hell02
hell0
hell0
[root@iZ2ze8prly56kg8in4hqa2Z ~]# echo "hell0" >> test.txt 
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt
hell02
hell02
hell0
hell0
hell0
```

## 6.more命令:

当我们用`cat`来查看一个长文件的时候就会这样：

重定向输入1000个 hello 到test.txt中：

```apl
cnt=0; while [ $cnt -le 1000 ]; do echo "hello $cnt"; let cnt++; done > test.txt
```

这时我们用`cat test.txt` 查看文件内容的话，那么我们会发现会全部输出出来非常的不利于查看

![image-20221213155651666](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221213155651666.png)

所以我们来介绍more命令：

> 语法： more [选项][文件]
> 功能： more命令，功能类似 cat

6.1常用选项:

> -n 对输出的所有行编号
> q 退出more  

代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# more test.txt
hello 0
hello 1
hello 2
hello 3
hello 4
hello 5
hello 6
hello 7
hello 8
hello 9
hello 10
hello 11
hello 12
hello 13
hello 14
hello 15
hello 16
hello 17
hello 18
hello 19
hello 20
hello 21
hello 22
hello 23
hello 24
hello 25
hello 26
hello 27
hello 28
hello 29
--More--(2%)
```

## 7.less命令:

> - less 工具也是对文件或其它输出进行**<u>分页显示</u>**的工具，应该说是linux正统查看文件内容的工具，功能极其强大。
> - less 的用法比起 more 更加的有弹性。在 more 的时候，我们并没有办法向前面翻， 只能往后面看
> - 但若使用了 less 时，就可以使用 [pageup][pagedown] 等按键的功能来往前往后翻看文件，更容易用来查看一个文件的内容！
> - 除此之外，在 less 里头可以拥有更多的搜索功能，不止可以向下搜，也可以向上搜  

> 语法： less [参数] 文件
> 功能：
> less与more类似，但使用less可以随意浏览文件，而more仅能向前移动，却不能向后移动，而且less在查看之前
> 不会加载整个文件  

7.1常用选项：

> - -i 忽略搜索时的大小写
> - -N 显示每行的行号
> - /字符串：向下搜索“字符串”的功能
> - ?字符串：向上搜索“字符串”的功能
> - n：重复前一个搜索（与 / 或 ? 有关）
> - N：反向重复前一个搜索（与 / 或 ? 有关）
> - q:quit  

## 8.head命令：

> 语法： head [参数]... [文件]...
> 功能：
> head 用来显示档案的开头至标准输出中，默认head命令打印其相应文件的开头10行。

8.1常用选项：

> -n<行数> 显示的行数  

8.2代码演示:

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# head test.txt
hello 0
hello 1
hello 2
hello 3
hello 4
hello 5
hello 6
hello 7
hello 8
hello 9
```

## 9.tail命令：

> tail 命令从指定点开始将文件写到标准输出.使用tail命令的-f选项可以方便的查阅正在改变的日志文件,tail -
> f filename会把filename里最尾部的内容显示在屏幕上,并且不但刷新,使你看到最新的文件内容.
> 语法： tail[必要参数][选择参数][文件]
> 功能： 用于显示指定文件末尾内容，不指定文件时，作为输入信息进行处理。常用查看日志文件。  

9.1常用选项:

> -f 循环读取
> -n<行数> 显示行数  

9.2代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# tail test.txt
hello 991
hello 992
hello 993
hello 994
hello 995
hello 996
hello 997
hello 998
hello 999
hello 1000
```

## 10.管道：

> | 管道的核心意义就是可以级联多条命令，让命令和命令组合，来完成批量化文本处理的任务

要理解管道我们从物理的角度来看管道有入口有出口，可以用来传输。所以在Linux就是用来传输资源的；

例子：

> (1)假设我们需要输出一个长文本的第500-520行
>
> ```apl
> cat test.txt | head -520 | tail -20
> ```
>
> ![image-20221213162518574](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221213162518574.png)
>
> (2)假设我们需要输出一个长文本的第500-520行的行数：
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z ~]# cat test.txt | head -520 | tail -20 | wc -l
> 20
> 
> ```

## 11.事件相关的命令：

10.1date命令：

1.在显示方面，使用者可以设定欲显示的格式，格式设定为一个加号后接数个标记，其中常用的标记列表如下  

> - %H : 小时(00..23)
> - %M : 分钟(00..59)
> - %S : 秒(00..61)
> - %X : 相当于 %H:%M:%S
> - %d : 日 (01..31)
> - %m : 月份 (01..12)
> - %Y : 完整年份 (0000..9999)
> - %F : 相当于 %Y-%m-%d  

2.在设定时间方面  

> - date -s //设置当前时间，只有root权限才能设置，其他只能查看。
> - date -s 20080523 //设置成20080523，这样会把具体时间设置成空00:00:00
> - date -s 01:01:01 //设置具体时间，不会对日期做更改
> - date -s “01:01:01 2008-05-23″ //这样可以设置全部时间
> - date -s “01:01:01 20080523″ //这样可以设置全部时间
> - date -s “2008-05-23 01:01:01″ //这样可以设置全部时间
> - date -s “20080523 01:01:01″ //这样可以设置全部时间  

3.时间戳  

> - 时间->时间戳： date +%s
>
> - 时间戳->时间： date -d@1508749502
>
> - Unix时间戳（英文为Unix epoch, Unix time, POSIX time 或 Unix timestamp）是从1970年1月1日（UTC/GMT的
>
>   午夜）开始所经过的秒数，不考虑闰秒  

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# date -d@0
Thu Jan  1 08:00:00 CST 1970
```

尝试将0s的时间戳转换成时间却是早晨八点这是因为北京处于东八区，格林威治零点的时候，北京就是八点；

## 12.cal命令：

> 命令格式： cal [参数] [月份] [年份]
> 功能： 用于查看日历等时间信息，如只有一个参数，则表示年份(1-9999)，如有两个参数，则表示月份和年份  

12.1命令格式:

> - -3 显示系统前一个月，当前月，下一个月的月历
> - -j 显示在当年中的第几天（一年日期按天算，从1月1号算起，默认显示当前月在一年中的天数）
> - -y 显示当前年份的日历  

12.2代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cal -3
    November 2022         December 2022         January 2023    
Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa  Su Mo Tu We Th Fr Sa
       1  2  3  4  5               1  2  3   1  2  3  4  5  6  7
 6  7  8  9 10 11 12   4  5  6  7  8  9 10   8  9 10 11 12 13 14
13 14 15 16 17 18 19  11 12 13 14 15 16 17  15 16 17 18 19 20 21
20 21 22 23 24 25 26  18 19 20 21 22 23 24  22 23 24 25 26 27 28
27 28 29 30           25 26 27 28 29 30 31  29 30 31            
                                                                
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cal -j
       December 2022       
Sun Mon Tue Wed Thu Fri Sat
                335 336 337
338 339 340 341 342 343 344
345 346 347 348 349 350 351
352 353 354 355 356 357 358
359 360 361 362 363 364 365
                           
[root@iZ2ze8prly56kg8in4hqa2Z ~]# cal -y
                               2022                               

       January               February                 March       
Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa
                   1          1  2  3  4  5          1  2  3  4  5
 2  3  4  5  6  7  8    6  7  8  9 10 11 12    6  7  8  9 10 11 12
 9 10 11 12 13 14 15   13 14 15 16 17 18 19   13 14 15 16 17 18 19
16 17 18 19 20 21 22   20 21 22 23 24 25 26   20 21 22 23 24 25 26
23 24 25 26 27 28 29   27 28                  27 28 29 30 31      
30 31                                                             
        April                   May                   June        
Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa
                1  2    1  2  3  4  5  6  7             1  2  3  4
 3  4  5  6  7  8  9    8  9 10 11 12 13 14    5  6  7  8  9 10 11
10 11 12 13 14 15 16   15 16 17 18 19 20 21   12 13 14 15 16 17 18
17 18 19 20 21 22 23   22 23 24 25 26 27 28   19 20 21 22 23 24 25
24 25 26 27 28 29 30   29 30 31               26 27 28 29 30      
                                                                  
        July                  August                September     
Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa
                1  2       1  2  3  4  5  6                1  2  3
 3  4  5  6  7  8  9    7  8  9 10 11 12 13    4  5  6  7  8  9 10
10 11 12 13 14 15 16   14 15 16 17 18 19 20   11 12 13 14 15 16 17
17 18 19 20 21 22 23   21 22 23 24 25 26 27   18 19 20 21 22 23 24
24 25 26 27 28 29 30   28 29 30 31            25 26 27 28 29 30   
31                                                                
       October               November               December      
Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa   Su Mo Tu We Th Fr Sa
                   1          1  2  3  4  5                1  2  3
 2  3  4  5  6  7  8    6  7  8  9 10 11 12    4  5  6  7  8  9 10
 9 10 11 12 13 14 15   13 14 15 16 17 18 19   11 12 13 14 15 16 17
16 17 18 19 20 21 22   20 21 22 23 24 25 26   18 19 20 21 22 23 24
23 24 25 26 27 28 29   27 28 29 30            25 26 27 28 29 30 31
```

------

拓展：

sort命令：对文件数据进行排序；

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# sort my.txt
11111
333333
333333
44444
444444
666666
77777
777777
99999
```

利用sort指令和管道对数据进行去重：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# sort my.txt | uniq | wc -l
8
[root@iZ2ze8prly56kg8in4hqa2Z ~]# sort my.txt | uniq
11111
333333
44444
444444
666666
77777
777777
99999

```

------

## 13.find命令：

> 语法： find pathname -options
> 功能： 用于在文件树种查找文件，并作出相应的处理（可能访问磁盘）

13.1常用选项(其中之一):

> - -name 按照文件名进行查找

13.2代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z learn]# find /learn -name test.c
/learn/mydir/dir/test.c
/learn/test.c
```

拓展：

`which`命令:

> which指令会在环境变量$PATH设置的目录里查找符合条件的文件。
>
> ```apl
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# which which
> alias which='(alias; declare -f) | /usr/bin/which --tty-only --read-alias --read-functions --show-tilde --show-dot'
> 	/usr/bin/which
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# which pwd
> /usr/bin/pwd
> [root@iZ2ze8prly56kg8in4hqa2Z learn]# which test.c
> /usr/bin/which: no test.c in (/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin)
> ```

`whereis`命令:

> 在系统默认路径下搜索指定名称的文件，程序，或者指定的归档文件等(如压缩包等等)；

```apl
[root@iZ2ze8prly56kg8in4hqa2Z learn]# whereis dir
dir: /usr/bin/dir /usr/share/man/man1/dir.1.gz /usr/share/info/dir /usr/share/info/dir.old
[root@iZ2ze8prly56kg8in4hqa2Z learn]# whereis ls
ls: /usr/bin/ls /usr/share/man/man1/ls.1.gz /usr/share/man/man1p/ls.1p.gz
[root@iZ2ze8prly56kg8in4hqa2Z learn]# whereis mydir
mydir:
```

`alias`命令：

> 用于给指令起别名  有点类似于C语言中的宏；
>
> 也可用于查看别名；

```apl
[root@iZ2ze8prly56kg8in4hqa2Z learn]# alias myls='ls -a -l -i -n'  //给'ls -a -l -i -n'起别名为myls
[root@iZ2ze8prly56kg8in4hqa2Z learn]# alias myls //查看别名
alias myls='ls -a -l -i -n'
[root@iZ2ze8prly56kg8in4hqa2Z learn]# myls
total 0
 83886278 drwxr-xr-x   3 0 0  33 Dec 12 22:38 .
      128 dr-xr-xr-x. 18 0 0 257 Dec 12 22:38 ..
117440680 drwxr-xr-x   3 0 0  17 Dec 12 22:38 mydir
 83886280 -rw-r--r--   1 0 0   0 Dec 12 22:06 test.c
```

## 14.grep命令：

> 语法： grep [选项] 搜寻字符串 文件
> 功能： 在文件中搜索字符串，将找到的行打印出来  

14.1常用选项:

> - -i ：忽略大小写的不同，所以大小写视为相同
> - -n ：顺便输出行号
> - -v ：反向选择，亦即显示出没有 '搜寻字符串' 内容的那一行  

14.2代码演示：

```apl
[root@iZ2ze8prly56kg8in4hqa2Z ~]# grep '88' test.txt
hello 88
hello 188
hello 288
hello 388
hello 488
hello 588
hello 688
hello 788
hello 880
hello 881
hello 882
hello 883
hello 884
hello 885
hello 886
hello 887
hello 888
hello 889
hello 988
```

拓展：

`top`命令：

> 相当于windows中的任务管管理器；

![image-20221213200616040](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20221213200616040.png)

------

## 15.zip/unzip指令 :

> 语法： zip 压缩文件.zip 目录或文件
> 功能： 将目录或文件压缩成zip格式  

压缩的意义：是为了减少数据大小以节省保存空间和传输的时间。 为了数据的传输，压缩能够作用于单独的数据内容或者所有的传输单元（包括数据头），这取决于一些特定的因素。

14.1常用选项:

> - -r  递归处理，将指定目录下的所有文件和子目录一并处理  
>
>   将test2目录压缩： zip test2.zip test2/*
>   解压到tmp目录： unzip test2.zip -d /tmp  

14.2代码演示:

```apl
[root@iZ2ze8prly56kg8in4hqa2Z learn]# zip dir.zip dir
  adding: dir (deflated 80%)
[root@iZ2ze8prly56kg8in4hqa2Z learn]# ll
total 16
-rw-r--r-- 1 root root 9901 Dec 13 15:57 dir
-rw-r--r-- 1 root root 2125 Dec 13 20:28 dir.zip
drwxr-xr-x 4 root root   26 Dec 13 20:28 mydir
-rw-r--r-- 1 root root    0 Dec 12 22:06 test.c
[root@iZ2ze8prly56kg8in4hqa2Z learn]# unzip dir.zip
Archive:  dir.zip
replace dir? [y]es, [n]o, [A]ll, [N]one, [r]ename: y
  inflating: dir                     
[root@iZ2ze8prly56kg8in4hqa2Z learn]# ll
total 16
-rw-r--r-- 1 root root 9901 Dec 13 15:57 dir
-rw-r--r-- 1 root root 2125 Dec 13 20:28 dir.zip
drwxr-xr-x 4 root root   26 Dec 13 20:28 mydir
-rw-r--r-- 1 root root    0 Dec 12 22:06 test.c
```

------

