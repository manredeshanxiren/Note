# Linux工具

## 1.Linux 软件包管理器`yum`的使用:

### 1.1什么是软件包:

> - 在Linux下安装软件, 一个通常的办法是下载到程序的源代码, 并进行编译, 得到可执行程序.
> - 但是这样太麻烦了, 于是有些人把一些常用的软件提前编译好, 做成软件包(可以理解成windows上的安装程序)放在一个服务器上, 通过包管理器可以很方便的获取到这个编译好的软件包, 直接进行安装.
> - 软件包和软件包管理器, 就好比 "App" 和 "应用商店" 这样的关系.
> - yum(Yellow dog Updater, Modified)是Linux下非常常用的一种包管理器. 主要应用在Fedora, RedHat , Centos等发行版上.  

### 1.2常用命令：

> ①`yum list | grep  xxxx` :   在`yum`源中搜索`xxxx`的软件包
>
> ②`yum -y install xxxx`:  安装`xxxx`名称的软件
>
> ③`yum -y remove xxxx`:	卸载`xxxx`名称的软件

### 1.3 yum源:

> 正如我们的软件管家，这个软件商店/管家，它怎么知道要到哪里去下载我们所需要的软件？
>
> 这些软件管家一定是要知道各个软件的下载地址的，这些软件管家一般都是内置下载链接的地址(配置文件);
>
> `yum`怎么知道我们要下载的软件在哪里呢？
>
> 其实`yum`也有自己的配置文件-----<u>**yum源**</u>;
>
> 一般位于`/etc/yum.repos.d/`

#### 1.3.1更新yum源:

> ①先备份旧的`yum源`  `Centos-Base.repo`
>
> ②`wget` 获取新的`yum源`配置文件  --- 可以通过网络搜索 
>
> ③`mv` 重命名成`Centos-Base.repo`
>
> ④`yum clean all ` && `yum makecache`

​	注意:

> 如果你想安装的软件找不到，有可能就是在拓展的`yum源`中:`epel.repo`
>
> 可以进行如下操作:
>
> `sudo yum install -y epel-release`    //根据我们的base yum源,帮我们找到和他匹配的拓展`yum源`
