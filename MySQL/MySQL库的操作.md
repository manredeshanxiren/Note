# MySQL库的操作

## 1.创建数据库

本质就是在`/var/lib/mysql`目录下创建了对应的目录

语法：

```mysql
CREATE DATABASE [IF NOT EXISTS] db_name [create_specification [,
create_specification] ...]
create_specification:
[DEFAULT] CHARACTER SET charset_name
[DEFAULT] COLLATE collation_name
```

说明：

- 大写的表示关键字  
- [] 是可选项  
- CHARACTER SET: 指定数据库采用的字符集  
- COLLATE: 指定数据库字符集的校验规则  

### 1.1创建数据库的案例

- 创建名为`db1`的数据库文件

  ```mysql
  create database db1;
  ```

  ![image-20240308170624253](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240308170624253.png)

  说明：当我们创建数据库没有指定字符集和校验规则时，系统使用默认字符集：utf8，校验规则是：utf8_ general_ ci  

- 创建一个使用utf8字符集的 db2 数据库  

  ```mysql
  create database db2 charset=utf8;
  ```

- 创建一个使用`utf`字符集，并带校对规则的 db3 数据库  

  ```mysql
  create database db3 charset=utf8 collate utf8_general_ci;
  ```

## 2.字符集和校验规则

- 数据库编码集：数据未来存储数据
- 数据库校验集：支持数据库，进行字段比较使用的编码，本质也是一种读取数据库中数据的采用的编码格式

数据库无论做什么操作，都必须保证操作和编码必须是编码一致的。

### 2.1查看系统默认字符集以及校验规则

```mysql
show variables like 'character_set_database';
show variables like 'collation_database';
```

![image-20240308125036299](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240308125036299.png)	

### 2.2查看数据库支持的字符集

```mysql
show charset;
```

![image-20240308125104768](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240308125104768.png)

### 2.3查看数据库支持的字符集校验规则

```mysql
show collation;
```

![image-20240308125226877](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240308125226877.png)

### 2.4校验规则对数据库的影响

- 不区分大小写

创建一个数据库，检验规则使用utf8_general_ci[不区分大小写]

```mysql
create database test1 collate utf8_general_ci;
```

```mysql
use test1;
```

```mysql
create table person(name varchar(20));
```

```mysql
insert into person values('a');
insert into person values('A');
insert into person values('b');
insert into person values('B');
```

- 区分大小写

创建一个数据库，校验规则使用utf8_bin[区分大小写]

```mysql
create database test2 collate utf8_bin;
```

```mysql
use test2
```

```mysql
create table person(name varchar(20));
```

```mysql
insert into person values('a');
insert into person values('A');
insert into person values('b');
insert into person values('B');
```

- 进行查询

**不区分大小写的查询以及结果**

```mysql
mysql> select * from person where name='a';
+------+
| name |
+------+
| a    |
| A    |
+------+
```

**区分大小写的查询以及结果**

```mysql
mysql> select * from person where name='a';
+------+
| name |
+------+
| a    |
+------+
```

- 结果排序

**不区分大小写排序以及结果：**

```mysql
mysql> select * from person order by name;
+------+
| name |
+------+
| a    |
| A    |
| b    |
| B    |
+------+
4 rows in set (0.00 sec)
```

**区分大小写排序以及结果：**

```mysql
mysql> select * from person order by name;
+------+
| name |
+------+
| A    |
| B    |
| a    |
| b    |
+------+
4 rows in set (0.00 sec)
```

## 3.操纵数据库

### 3.1查看数据库

```mysql
show databases;
```

### 3.2显示创建语句

```mysql
show create database 数据库名;
```

### 3.3查看我们当前use了哪一个数据库

```mysql
mysql> use test1;
mysql> select database();
+------------+
| database() |
+------------+
| test1      |
+------------+
1 row in set (0.00 sec)
```

示例：

```mysql
mysql> show create database test1;
+----------+----------------------------------------------------------------+
| Database | Create Database                                                |
+----------+----------------------------------------------------------------+
| test1    | CREATE DATABASE `test1` /*!40100 DEFAULT CHARACTER SET utf8 */ |
+----------+----------------------------------------------------------------+
1 row in set (0.00 sec)
```

说明：

- MySQL建议我们的关键字使用大写，但是不是必须的。
- 数据库名字的反引号``,是为了防止使用的数据库名刚好是关键字
- ` /*!40100 DEFAULT CHARACTER SET utf8 */`这个不是注释，表示当前`mysql`版本大于4.01版本，就执行这句话

### 3.4修改数据库

语法：

```mysql
ALTER DATABASE db_name
[alter_spacification [,alter_spacification]...]
alter_spacification:
[DEFAULT] CHARACTER SET charset_name
[DEFAULT] COLLATE collation_name
```

说明：

- 对数据的修改主要指的是修改数据库的字符集，校验规则

示例：将test1数据库字符集修改为`gbk`，以及校验规则修改为`gbk_chinese_ci`

```mysql
mysql> alter database test1 charset=gbk collate gbk_chinese_ci;
Query OK, 1 row affected (0.00 sec)
[root@lavm-5wklnbmaja mysql]# cat test1/db.opt 
default-character-set=gbk
default-collation=gbk_chinese_ci
```

### 3.5数据库的删除

```mysql
DROP DATABASE [IF EXISTS] db_ name;
```

执行删除之后的结果：

- 数据库内部看不到对应的数据库
- 对应的数据库文件夹被删除，级联删除，里面的数据表全部被删

> 注意：不要随便删除数据库

### 3.6备份和恢复

#### 3.6.1备份

语法：

```mysql
# mysqldump -P3306 -u root -p 密码 -B 数据库名 > 数据库备份存储的文件路径
```

示例：将test2库备份到文件（退出连接）

```mysql
# mysqldump -P -uroot -p -B test1 > /learn/MySQL/test1.sql
```

这时，可以打开看看 `mytest.sql` 文件里的内容，其实把我们整个创建数据库，建表，导入数据的语句都装载这个文件中。  

#### 3.6.2还原

```mysql
mysql> source /learn/MySQL/test1.sql;
```

注意：

- 如果备份的不是整个数据库，而是其中的一张表，怎么做？  

  ```mysql
  # mysqldump -u root -p 数据库名 表名1 表名2 > D:/mytest.sql
  ```

- 同时备份多个数据库  

  ```mysql
  # mysqldump -u root -p -B 数据库名1 数据库名2 ... > 数据库存放路径
  ```

- 如果备份一个数据库时，没有带上-B参数， 在恢复数据库时，需要先创建空数据库，然后使用数据库，再使用source来还原。 

### 3.7查看连接情况

语法：

```mysql
show processlist;
```

示例：

```mysql
mysql> show processlist;
+----+------+-----------+------------+---------+-------+----------+------------------+
| Id | User | Host      | db         | Command | Time  | State    | Info             |
+----+------+-----------+------------+---------+-------+----------+------------------+
|  5 | root | localhost | helloworld | Sleep   | 21363 |          | NULL             |
|  6 | root | localhost | test1      | Query   |     0 | starting | show processlist |
+----+------+-----------+------------+---------+-------+----------+------------------+
2 rows in set (0.00 sec)
```

可以告诉我们当前有哪些用户连接到我们的MySQL，如果查出某个用户不是你正常登陆的，很有可能你的数据库被人入侵了。以后大家发现自己数据库比较慢时，可以用这个指令来查看数据库连接情况。 