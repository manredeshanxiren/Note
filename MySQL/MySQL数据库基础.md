# MySQL数据库基础

## 1.基本使用

### 1.1MySQL安装

> - [CentOS 6.5下编译安装MySQL 5.6.14](https://blog.51cto.com/aiilive/2119890)
> - [CentOS 7 通过 yum 安装 MariaDB](https://zhuanlan.zhihu.com/p/49046496)
> - [Windows下通过MySQL Installer安装MySQL服务](https://blog.51cto.com/aiilive/2116476)

### 1.2连接服务器

#### 1.2.1CentOS下的服务器MySQL的服务启动

输入：

```cpp
mysql -h 127.0.0.1 -P 3306 -u root -p
```

输出：

```bash
[mi@lavm-5wklnbmaja learn]$ mysql -h 127.0.0.1 -P 3306 -u root -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 4
Server version: 5.7.44 MySQL Community Server (GPL)

Copyright (c) 2000, 2023, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql> 
```

> 注意：
>
> 如果没有写 `-h 127.0.0.1 `默认是连接本地  
>
> 如果没有写 -P 3306 默认是连接3306端口号  

#### 1.2.2Win中的服务器管理

- 执行 win+r 输入 services.msc 打开服务管理器  
- 通过下图左侧停止，暂停，重启动按钮进行服务管理  

![image-20240308095010333](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240308095010333.png)

### 1.3服务器，数据库，表的关系

- 所谓安装数据库服务器，只是在机器上安装了一个数据库管理系统程序，这个管理程序可以管理多个数据库，一般开发人员会针对每一个应用创建一个数据库。
-  为保存应用中实体的数据，一般会在数据库中创建多个表，以保存程序中实体的数据。
-   数据库服务器、数据库和表的关系如下：  

![image-20240308095139368](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240308095139368.png)

### 1.4使用案例

- 创建数据库  

  ```bash
  create database helloworld;
  ```

- 使用数据库

  ```bash
  use helloworld;
  ```

- 创建数据库表  

  ```mysql
  create table student(
  id int,
  name varchar(32),
  gender varchar(2)
  );
  ```

- 表中插入数据

  ```mysql
  insert into student (id, name, gender) values (1, '张三', '男');
  insert into student (id, name, gender) values (2, '李四', '女');
  insert into student (id, name, gender) values (3, '王五', '男');
  ```

  

- 查询表中的数据

  ```mysql
  select * from student;
  ```

  ![image-20240308100922625](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240308100922625.png)

### 1.5MySQL架构

MySQL 是一个可移植的数据库，几乎能在当前所有的操作系统上运行，如 Unix/Linux、Windows、Mac 和 Solaris。各种系统在底层实现方面各有不同，但是 MySQL 基本上能保证在各个平台上的物理体系结构的一致性 

![image-20240308103807759](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240308103807759.png)

### 1.6SQL语句分类

> - `DDL【data definition language】` 数据定义语言，用来维护存储数据的**结构**  
>
>   代表指令: `create, drop, alter`  
>
> - `DML【data manipulation language】` 数据操纵语言，用来对数据进行操作
>   代表指令： `insert，delete，update `
>
>   ​	-DML中又单独分了一个DQL，数据查询语言，代表指令：` select  `
>
> - `DCL【Data Control Language】` 数据控制语言，主要负责权限管理和事务
>   代表指令：` grant，revoke，commit  `

### 1.7存储引擎

#### 1.7.1存储引擎

存储引擎是：数据库管理系统如何存储数据、如何为存储的**数据建立索引**和**如何更新**、查询数据等技术的实现方法。

**MySQL的核心就是插件式存储引擎，支持多种存储引擎。**  

#### 1.7.2查看存储引擎

```mysql
show engines;
```

![image-20240308103153924](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240308103153924.png)

