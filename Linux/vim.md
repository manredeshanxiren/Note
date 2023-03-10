# vim

## 1. vim最小集:

### 1.1 vim的概念:

> 简单来说`vim`就是一款多模式的`编辑器`;
>
> 我们一般常用其中的3-5种

## 1.2 注意事项:

> 1.如果你不知道自己在什么模式下，无脑`esc`;
>
> 2.退出的时候，一般都是先保存，在退出;
>
> 3.在vim中操作，不要使用鼠标和鼠标滚轮;

![image-20230104210720367](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230104210720367.png)

## 2. vim指令集:

> (1)底行模式下:
>
> 调出或去掉行号: `set nu / set nonu`;
>
> 替换:`%s/A/B/g`  --->将文档中**A**的字符串替换成**B**;
>
> 查找:`/key` --->在文档中查找和`key`匹配的字符串;
>
> 执行命令后按任意键返回vim当前页面:`! command`;
>
> 在vim中同时分窗口编辑文件:`vs file` /`ctrl + ww(快速按两次w)`--->两个文件的切换; 
>
> --------
>
> 大部分指令，全部都是在<font color = 'red'>**命令模式**</font>下进行的(不是底行，也不是插入)的;
>
> (2)`yy/nyy`: 复制当前行/复制当前行在内的指定n行;
>
> (3)`p/np`: 粘贴一行或者n行相同的内容到当前光标的所在行之下;
>
> (4)`u`: <font color = 'red'>撤销刚刚的操作</font>;
>
> (5)`ctrl + r`: <font color = 'red'>对撤销再做撤销</font>;
>
> (6)`dd/ndd`: 对当前行,或者当前行在内的n行内容进行剪切;
>
> (7)光标定位:
>
> `shift + g`:将光标定位到文档的**最结尾**;
>
> `gg`:将光标定位到文档的**最开始**;
>
> `n shift g`: 将光标定位到文档的指定行;
>
> `shift + 4`: 将光标定位到文档行的结尾;
>
> `shift + 6`:将光标定位到文档行的开始;
>
> `n w , n b`:以单词为单位(n表示n个单词)，进行前后移动，`w`向后，`b`向前;
>
> (8)`shift + ~`: 快速大小写切换;
>
> (9)`n r`: 替换光标所在的字符;
>
> (10)`n x`:行内删除(左侧--->右侧);
>
> (11)`n shift + x`:行内删除(右侧--->左侧);
>
> (12)移动光标:
>
> `h j k l`:分别对应←，↓，↑，→;

-------

## 3. vim配置:

### 3.1自己配置:

> vim在启动的时候，会自动在当前用户的目录下，寻找配置文件(`.vimrc`)如果没有就是默认
>
> 一个用户，一个vim配置，我们所做的配置不会影响其他人;
>
> 可以在往上搜索，相关的配置命令，来实现自己想要的效果;
>
> 例如:
>
> 我们创建vim的配置文件`.vimrc`，并且修改其中配置为`set nu`，并查看效果;
>
> ```
> [mi@iZ2ze8prly56kg8in4hqa2Z ~]$ vim .vimrc
> ```
>
> ![image-20230104204142578](C:\Users\jason\AppData\Roaming\Typora\typora-user-images\image-20230104204142578.png)

### 3.2 自动化配置:

在Linux目录下输入以下命令即可:

> 目前只支持 `centos7`系列的系统；

```
curl -sLf https://gitee.com/HGtz2222/VimForCpp/raw/master/install.sh -o ./install.sh && bash ./install.sh
```

拓展:

普通用户无法`sudo`的解决:

> ①首先我们添加写的权限:`chmod u+w /etc/sudoers`
>
> ②然后使用vim打开此文件`vim /etc/sudoers`
>
> ③在`root  ALL=(ALL) ALL`后加上 `用户名   ALL=(ALL)  ALL`然后保存后退出
>
> ④最后将写的权限去掉:`chmod u-w /etc/sudoers`