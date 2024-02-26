# MySQL的安装以及配置环境

## 1.删除存在的旧的MySQL文件

```bash
[root@lavm-5wklnbmaja learn]$ ps ajx |grep mariadb # 先检查是否有mariadb存在
13134 14844 14843 13134 pts/0 14843 S+ 1005 0:00 grep --color=auto
mariadb
19010 19187 19010 19010 ? -1 Sl 27 16:55 /usr/libexec/mysqld --
basedir=/usr --datadir=/var/lib/mysql --plugin-dir=/usr/lib64/mysql/plugin --
log-error=/var/log/mariadb/mariadb.log --pid-file=/var/run/mariadb/mariadb.pid
--socket=/var/lib/mysql/mysql.sock
[root@lavm-5wklnbmaja learn]$ systemctl stop mariadb.service # 停⽌mariadb 服务
==== AUTHENTICATING FOR org.freedesktop.systemd1.manage-units ===
Authentication is required to manage system services or units.
Authenticating as: root
Password:
==== AUTHENTICATION COMPLETE ===
[root@lavm-5wklnbmaja learn]$ ps axj |grep mariadb # 停⽌完成
13134 14976 14975 13134 pts/0 14975 S+ 1005 0:00 grep --color=auto
mariadb
```

## 2.检查系统安装包

```bash
[root@lavm-5wklnbmaja learn]$ rpm -qa | grep mariadb
or
[root@lavm-5wklnbmaja learn]$ rpm -qa | grep mysql
...
...
...
# 下⾯是打印出来的mysql的样例，
[root@lavm-5wklnbmaja learn]# rpm -qa | grep mysql
mysql-community-common-5.7.41-1.el7.x86_64
mysql-community-server-5.7.41-1.el7.x86_64
mysql57-community-release-el7-11.noarch
mysql-community-client-5.7.41-1.el7.x86_64
mysql-community-libs-5.7.41-1.el7.x86_64
```

## 3.卸载这些默认的安装包

```bash
#卸载显⽰出来的mariadb/mysql安装包
[root@lavm-5wklnbmaja learn]$ sudo yum remove mariadb # 还是试着将上⾯的包都⼀个⼀个卸载吧
备份/etc/my.cnf,备份/var/lib/mysql数据
```

## 4.获取MySQL官方yum源

①获取MySQL官方yum源 ：http://repo.mysql.com/  

注意：我们最好安装和自己系统一致的MySQL版本，否则可能会存在软件兼容性问题

所以我们可以查看自己的系统版本：

```bash
[root@lavm-5wklnbmaja learn]# cat /etc/redhat-release
CentOS Linux release 7.6.1810 (Core) 
```

我这里的版本是CentOS7.6，然后我们打开链接，寻找对应的版本(因为原本的网页的显示不全，我们直接选择查看网页源代码)，如下是对应我的系统版本的MySQL:

![image-20240226214057990](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240226214057990.png)

接下来我们是下载到自己对应的Windows环境下。然后直接拖动到，我们的`Xshell`窗口即可上传到我们的服务器上。这里我们需要用到`rz`指令如果没有安装可以安装一下：`yum -y install lrzsz`;

## 5.安装MySQL yum源，对比前后yum源

安装前：

```bash
[root@lavm-5wklnbmaja MySQL]# ls /etc/yum.repos.d/ -l 
total 16
drwxr-xr-x 2 root root  187 Jun 11  2019 bak
-rw-r--r-- 1 root root 1522 Jun 11  2019 CentOS-7.repo
-rw-r--r-- 1 root root  221 Jun 11  2019 epel.repo
-rw-r--r-- 1 root root 1358 Sep  5  2021 epel.repo.rpmnew
-rw-r--r-- 1 root root 1457 Sep  5  2021 epel-testing.repo
```

安装的指令：`rpm -ivh mysql57-community-release-el7.rpm`

安装后：最后两个就是MySQL yum源

```bash
[root@lavm-5wklnbmaja MySQL]# ls /etc/yum.repos.d/ -l
total 24
drwxr-xr-x 2 root root  187 Jun 11  2019 bak
-rw-r--r-- 1 root root 1522 Jun 11  2019 CentOS-7.repo
-rw-r--r-- 1 root root  221 Jun 11  2019 epel.repo
-rw-r--r-- 1 root root 1358 Sep  5  2021 epel.repo.rpmnew
-rw-r--r-- 1 root root 1457 Sep  5  2021 epel-testing.repo
-rw-r--r-- 1 root root 1838 Apr 27  2017 mysql-community.repo
-rw-r--r-- 1 root root 1885 Apr 27  2017 mysql-community-source.repo
```

## 6.检查一下是否可以正常工作

```bash
[root@lavm-5wklnbmaja MySQL]# yum list | grep mysql
mysql57-community-release.noarch         el7-11                        installed
akonadi-mysql.x86_64                     1.9.2-4.el7                   base     
anope-mysql.x86_64                       2.1.1-1.el7                   epel     
apr-util-mysql.x86_64                    1.5.2-6.el7_9.1               updates  
calligra-kexi-driver-mysql.x86_64        2.9.10-2.el7                  epel     
collectd-mysql.x86_64                    5.8.1-1.el7                   epel     
dmlite-plugins-mysql.x86_64              1.15.2-15.el7                 epel     
....
zabbix6.0-web-mysql.noarch               6.0.25-1.el7                  epel     
```

## 7.安装MySQL服务

```bash
[root@lavm-5wklnbmaja MySQL]# yum install -y mysql-community-server
Loaded plugins: aliases, auto-update-debuginfo, fastestmirror, protectbase
Repository epel is listed more than once in the configuration
...
Installing : mysql-community-common-5.7.33-1.el7.x86_64 #公共模块
Installing : mysql-community-libs-5.7.33-1.el7.x86_64 #库
Installing : mysql-community-client-5.7.33-1.el7.x86_64 #客⼾端
Installing : mysql-community-server-5.7.33-1.el7.x86_64 #服务器
... #安装了服务和客⼾端
# 如果报错了，看第12步《常⻅问题》
```

## 8.查看配置文件和数据存储位置

```bash
[root@lavm-5wklnbmaja MySQL]# which mysql     //MySQL客户端
/usr/bin/mysql
[root@lavm-5wklnbmaja MySQL]# which mysqld    //MySQL服务端
/usr/sbin/mysqld
[root@lavm-5wklnbmaja MySQL]# ls /etc/my.cnf  //配置文件
/etc/my.cnf
```

## 9.启动服务和查看启动服务

```bash
[root@lavm-5wklnbmaja MySQL]# systemctl start mysqld  //启动服务
[root@lavm-5wklnbmaja MySQL]# ps ajx | grep mysqld    //查看服务
    1   964   963   963 ?           -1 Sl      27   0:00 /usr/sbin/mysqld --daemonize --pid-file=/var/run/mysqld/mysqld.pid
30152  1004  1003  8830 pts/0     1003 S+       0   0:00 grep --color=auto mysqld
[root@lavm-5wklnbmaja MySQL]# ps ajx | grep mysqld
    1   964   963   963 ?           -1 Sl      27   0:00 /usr/sbin/mysqld --daemonize --pid-file=/var/run/mysqld/mysqld.pid
30152  1037  1036  8830 pts/0     1036 S+       0   0:00 grep --color=auto mysqld
[root@lavm-5wklnbmaja MySQL]# systemctl start mysqld
[root@lavm-5wklnbmaja MySQL]# ps ajx | grep mysqld
    1   964   963   963 ?           -1 Sl      27   0:00 /usr/sbin/mysqld --daemonize --pid-file=/var/run/mysqld/mysqld.pid
30152  1048  1047  8830 pts/0     1047 S+       0   0:00 grep --color=auto mysqld

```

## 10.登录的三个方式

①登陆⽅法⼀【不⾏就下⼀个】  

对较老的版本奏效，新版本或许不可以

```bash
#获取临时root密码
[root@lavm-5wklnbmaja MySQL]$ sudo grep 'temporary password' /var/log/mysqld.log
2021-04-12T03:23:46.153263Z 1 [Note] A temporary password is generated for
root@localhost: yLMalT:v+5l*
#使⽤临时密码登录
[root@lavm-5wklnbmaja MySQL]$ mysql -uroot -p
Enter password:
#判断修改密码时候新密码是否符合当前的策略，不满⾜报错，不让修改，关闭它
#安全强度，默认为中，即1，要求必须包含 数字、符号、⼤⼩写字⺟，⻓度⾄少为8位
mysql> set global validate_password_policy=0;
Query OK, 0 rows affected (0.00 sec)
#密码最⼩⻓度
mysql> set global validate_password_length=1;
Query OK, 0 rows affected (0.00 sec)
#修改本地登录密码，暂不授权远程登录
mysql> ALTER USER 'root'@'localhost' IDENTIFIED BY 'qwer@wu.888';
Query OK, 0 rows affected (0.00 sec)
mysql> FLUSH PRIVILEGES;
Query OK, 0 rows affected (0.00 sec)
#如果你安装的最新的mysql，没有所谓的临时密码，root默认没有密码
```

②登陆⽅法⼆【不⾏就下⼀个】  

```bash
# 如果你安装的最新的mysql，没有所谓的临时密码，root默认没有密码
# 试着直接client登陆⼀下
```

③登陆⽅式三【应该⾏了吧】  

让输入密码的时候直接回车！这个方法相当于跳过对应的密码的验证

```bash
[root@lavm-5wklnbmaja MySQL]# vim /etc/my.cnf
在[mysqld]最后⼀栏配置(不知道是什么，就放在配置⽂件最后) 加⼊: skip-grant-tables 选项,
并保存退出
[root@lavm-5wklnbmaja MySQL]# systemctl restart mysqld
[root@lavm-5wklnbmaja MySQL]# mysql -u root -p
Enter password:  
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 2
Server version: 5.7.44 MySQL Community Server (GPL)

Copyright (c) 2000, 2023, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
```

## 11.配置my.cnf

我们在配置文件中加入这几个选项即可。

```bash
port=3306
character-set-server=utf8
default-storage-engine=innodb

# 配置完毕，重启mysql即可
```

## 12.常见问题

```bash
mysql 已经配置了客⼾端服务器utf8编码，但是⽆法输⼊中⽂
确保您在终端命令⾏中可以输⼊中⽂
[root@lavm-5wklnbmaja MySQL]$ env |grep LANG
LANG=en_US.utf8
```

```bash
安装遇到秘钥过期的问题：
Failing package is: mysql-community-client-5.7.39-1.el7.x86_64
GPG Keys are configured as: file:///etc/pki/rpm-gpg/RPM-GPG-KEY-mysql
解决⽅案：
rpm --import https://repo.mysql.com/RPM-GPG-KEY-mysql-2022
```

