# MySQL事务

## 1.什么是事务

> 事务（Transaction）通常是指在数据库中执行的一组操作单元，它要么完全执行，要么完全不执行，不会出现部分执行的情况。事务具有四个特性，通常用**ACID**来描述；
>
> 1. **原子性（Atomicity）：** 事务是一个原子操作单元，不可再分割，要么全部执行成功，要么全部执行失败。如果事务中的任何一项操作失败，整个事务将被**<u>回滚</u>**到初始状态，以确保数据库的一致性。
> 2. **一致性（Consistency）：** 事务执行前后，数据库的完整性约束没有被破坏。即使在事务执行过程中发生错误，数据库也应该<u>保持一致性</u>。
> 3. **隔离性（Isolation）：** 多个事务并发执行时，每个事务的操作应该<u>**独立于其他事务**</u>，互不干扰。这意味着每个事务在执行时看到的数据应该是一致的，而不受其他事务影响。
> 4. **持久性（Durability）：** <u>**一旦事务提交，其对数据库的修改应该永久保存**</u>，即使系统发生故障或断电也不会丢失。已提交的事务的修改应该被持久化到数据库中。

## 2.为什么会出现事务  

> 1. **<u>保证数据完整性和一致性：</u>** 在数据库中，多个操作可能需要以原子性的方式执行，以确保数据库的一致性和完整性。例如，如果一个银行客户同时进行存款和取款操作，需要保证这两个操作要么全部成功，要么全部失败，以防止出现账户余额不一致等问题。
>
> 2. **<u>支持并发访问：</u>** 在多用户环境下，多个用户可能同时对数据库进行操作，如果没有事务的支持，可能会导致数据不一致或丢失更新等问题。通过事务的隔离性特性，可以确保多个并发事务之间相互独立，避免数据冲突和并发问题。
>
> 3. **<u>系统故障恢复：</u>** 数据库系统可能会面临硬件故障、软件崩溃等问题，为了保证数据的持久性，需要使用事务来确保已提交的数据不会因系统故障而丢失。事务的持久性特性确保了已提交的事务对数据库的修改是永久性的。
>
> 4. **<u>简化复杂操作：</u>** 在复杂的业务逻辑中，可能需要进行多个数据库操作，例如订单处理、库存管理等。通过将这些操作组织成一个事务，可以简化操作流程，并确保所有操作要么全部成功，要么全部失败。
>
> 综上所述，事务的出现是为了解决数据库操作中的多个问题，包括保证数据完整性、支持并发访问、系统故障恢复和简化复杂操作等。通过事务的特性，可以确保数据库操作的正确性和可靠性。

## 3.事务版本的支持

在 MySQL 中只有使用了 Innodb 数据库引擎的数据库或表才支持事务， MyISAM 不支持 

查看数据库引擎  

```mysql
mysql> show engines\G;
*************************** 1. row ***************************
      Engine: InnoDB
     Support: DEFAULT
     Comment: Supports transactions, row-level locking, and foreign keys
Transactions: YES
          XA: YES
  Savepoints: YES
*************************** 2. row ***************************
      Engine: MRG_MYISAM
     Support: YES
     Comment: Collection of identical MyISAM tables
Transactions: NO
          XA: NO
  Savepoints: NO
*************************** 3. row ***************************
      Engine: MEMORY
     Support: YES
     Comment: Hash based, stored in memory, useful for temporary tables
Transactions: NO
          XA: NO
  Savepoints: NO
*************************** 4. row ***************************
      Engine: BLACKHOLE
     Support: YES
     Comment: /dev/null storage engine (anything you write to it disappears)
Transactions: NO
          XA: NO
  Savepoints: NO
*************************** 5. row ***************************
      Engine: MyISAM
     Support: YES
     Comment: MyISAM storage engine
Transactions: NO
          XA: NO
  Savepoints: NO
*************************** 6. row ***************************
      Engine: CSV
     Support: YES
     Comment: CSV storage engine
Transactions: NO
          XA: NO
  Savepoints: NO
*************************** 7. row ***************************
      Engine: ARCHIVE
     Support: YES
     Comment: Archive storage engine
Transactions: NO
          XA: NO
  Savepoints: NO
*************************** 8. row ***************************
      Engine: PERFORMANCE_SCHEMA
     Support: YES
     Comment: Performance Schema
Transactions: NO
          XA: NO
  Savepoints: NO
*************************** 9. row ***************************
      Engine: FEDERATED
     Support: NO
     Comment: Federated MySQL storage engine
Transactions: NULL
          XA: NULL
  Savepoints: NULL
9 rows in set (0.00 sec)
```

##  4.事务的提交方式

事务的提交方式常见的有两种

- 自动提交
- 手动提交

查看事务提交方式

```mysql
mysql> show variables like 'autocommit';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit    | ON    |
+---------------+-------+
1 row in set (0.01 sec)

mysql> set autocommit=0;   -- 将自动提交关闭
Query OK, 0 rows affected (0.00 sec)

mysql> show variables like 'autocommit';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit    | OFF   |
+---------------+-------+
1 row in set (0.00 sec)

mysql> set autocommit=1;   -- 将自动提交打开
Query OK, 0 rows affected (0.00 sec)

mysql> show variables like 'autocommit';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit    | ON    |
+---------------+-------+
1 row in set (0.00 sec)
```

## 5.事务常见操作方式

### 5.1提前准备

简单银行用户表  

- 提前准备  

```mysql
## Centos 7 云服务器，默认开启3306 mysqld服务
[whb@VM-0-3-centos ~]$ sudo netstat -nltp
[sudo] password for whb:
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address Foreign Address State
PID/Program name
tcp6 0 0 :::3306 :::* LISTEN
30415/mysqld
## 使用win cmd远程访问Centos 7云服务器，mysqld服务(需要win上也安装了MySQL，这里看到结
果即可)
## 注意，使用本地mysql客户端，可能看不到链接效果，本地可能使用域间套接字，查不到链接
C:\Users\whb>mysql -uroot -p -h42.192.83.143
Enter password: ***********
Welcome to the MySQL monitor. Commands end with ; or \g.
Your MySQL connection id is 3484
Server version: 5.7.33 MySQL Community Server (GPL)
Copyright (c) 2000, 2019, Oracle and/or its affiliates. All rights reserved.
Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.
Type 'help;' or '\h' for help. Type '\c' to clear the current input
statement.
## 使用netstat查看链接情况，可知：mysql本质是一个客户端进程
[whb@VM-0-3-centos ~]$ sudo netstat -ntp
Active Internet connections (w/o servers)
Proto Recv-Q Send-Q Local Address Foreign Address State
PID/Program name
tcp6 0 0 172.17.0.3:3306 113.132.141.236:19354
ESTABLISHED 30415/mysqld
## 为了便于演示，我们将mysql的默认隔离级别设置成读未提交。
## 具体操作我们后面专门会讲，现在已使用为主。
mysql> set global transaction isolation level READ UNCOMMITTED;
Query OK, 0 rows affected (0.00 sec)
mysql> quit
Bye
##需要重启终端，进行查看
mysql> select @@tx_isolation;
+------------------+
| @@tx_isolation |
+------------------+
| READ-UNCOMMITTED |
+------------------+
1 row in set, 1 warning (0.00 sec)
```

- 创建测试表

```mysql
create table if not exists account(
id int primary key,
name varchar(50) not null default '',
blance decimal(10,2) not null default 0.0
)ENGINE=InnoDB DEFAULT CHARSET=UTF8;
```

- 正常演示 - 证明事务的开始与回滚  

```mysql
mysql> start transaction; -- 开始一个事务begin也可以，推荐begin
Query OK, 0 rows affected (0.00 sec)
mysql> savepoint save1; -- 创建一个保存点save1
Query OK, 0 rows affected (0.00 sec)
mysql> insert into account values (1, '张三', 100); -- 插入一条记录
Query OK, 1 row affected (0.05 sec)
mysql> savepoint save2; -- 创建一个保存点save2
Query OK, 0 rows affected (0.01 sec)
mysql> insert into account values (2, '李四', 10000); -- 在插入一条记录
Query OK, 1 row affected (0.00 sec)
mysql> select * from account; -- 两条记录都在了
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 100.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> rollback to save2; -- 回滚到保存点save2
Query OK, 0 rows affected (0.03 sec)
mysql> select * from account; -- 一条记录没有了
+----+--------+--------+
| id | name | blance |
+----+--------+--------+
| 1 | 张三 | 100.00 |
+----+--------+--------+
1 row in set (0.00 sec)
mysql> rollback; -- 直接rollback，回滚在最开始
Query OK, 0 rows affected (0.00 sec)
mysql> select * from account; -- 所有刚刚的记录没有了
Empty set (0.00 sec)
```

- 非正常演示1 - 证明未commit，客户端崩溃，MySQL自动会回滚（隔离级别设置为读未提交）  

```mysql
-- 终端A
mysql> select * from account; -- 当前表内无数据
Empty set (0.00 sec)
mysql> show variables like 'autocommit'; -- 依旧自动提交
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit | ON |
+---------------+-------+
1 row in set (0.00 sec)
mysql> begin; --开启事务
Query OK, 0 rows affected (0.00 sec)
mysql> insert into account values (1, '张三', 100); -- 插入记录
Query OK, 1 row affected (0.00 sec)
mysql> select * from account; --数据已经存在，但没有commit，此时同时查看
终端B
+----+--------+--------+
| id | name | blance |
+----+--------+--------+
| 1 | 张三 | 100.00 |
+----+--------+--------+
1 row in set (0.00 sec)
mysql> Aborted -- ctrl + \ 异常终止MySQL
--终端B
mysql> select * from account; --终端A崩溃前
+----+--------+--------+
| id | name | blance |
+----+--------+--------+
| 1 | 张三 | 100.00 |
+----+--------+--------+
1 row in set (0.00 sec)
mysql> select * from account; --数据自动回滚
Empty set (0.00 sec)
```

- 非正常演示2 - 证明commit了，客户端崩溃，MySQL数据不会在受影响，已经持久化  

```mysql
--终端 A
mysql> show variables like 'autocommit'; -- 依旧自动提交
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit | ON |
+---------------+-------+
1 row in set (0.00 sec)
mysql> select * from account; -- 当前表内无数据
Empty set (0.00 sec)
mysql> begin; -- 开启事务
Query OK, 0 rows affected (0.00 sec)
mysql> insert into account values (1, '张三', 100); -- 插入记录
Query OK, 1 row affected (0.00 sec)
mysql> commit; --提交事务
Query OK, 0 rows affected (0.04 sec)
mysql> Aborted -- ctrl + \ 异常终止MySQL
--终端 B
mysql> select * from account; --数据存在了，所以commit的作用是将数据持久
化到MySQL中
+----+--------+--------+
| id | name | blance |
+----+--------+--------+
| 1 | 张三 | 100.00 |
+----+--------+--------+
1 row in set (0.00 sec)
```

- 非正常演示3 - 对比试验。证明begin操作会自动更改提交方式，不会受MySQL是否自动提交影响  

```mysql
-- 终端 A
mysql> select *from account; --查看历史数据
+----+--------+--------+
| id | name | blance |
+----+--------+--------+
| 1 | 张三 | 100.00 |
+----+--------+--------+
1 row in set (0.00 sec)
mysql> show variables like 'autocommit'; --查看事务提交方式
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit | ON |
+---------------+-------+
1 row in set (0.00 sec)
mysql> set autocommit=0; --关闭自动提交
Query OK, 0 rows affected (0.00 sec)
mysql> show variables like 'autocommit'; --查看关闭之后结果
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit | OFF |
+---------------+-------+
1 row in set (0.00 sec)
mysql> begin; --开启事务
Query OK, 0 rows affected (0.00 sec)
mysql> insert into account values (2, '李四', 10000); --插入记录
Query OK, 1 row affected (0.00 sec)
mysql> select *from account; --查看插入记录，同时查看终端B
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 100.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> Aborted --再次异常终止
-- 终端B
mysql> select * from account; --终端A崩溃前
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 100.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> select * from account; --终端A崩溃后，自动回滚
+----+--------+--------+
| id | name | blance |
+----+--------+--------+
| 1 | 张三 | 100.00 |
+----+--------+--------+
1 row in set (0.00 sec)
```

- 非正常演示4 - 证明单条 SQL 与事务的关系  

```mysql
--实验一
-- 终端A
mysql> select * from account;
+----+--------+--------+
| id | name | blance |
+----+--------+--------+
| 1 | 张三 | 100.00 |
+----+--------+--------+
1 row in set (0.00 sec)
mysql> show variables like 'autocommit';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit | ON |
+---------------+-------+
1 row in set (0.00 sec)
mysql> set autocommit=0; --关闭自动提交
Query OK, 0 rows affected (0.00 sec)
mysql> insert into account values (2, '李四', 10000); --插入记录
Query OK, 1 row affected (0.00 sec)
mysql> select *from account; --查看结果，已经插入。此时可以在查
看终端B
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 100.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> ^DBye --ctrl + \ or ctrl + d,终止终
端 --
终端B
mysql> select * from account; --终端A崩溃前
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 100.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> select * from account; --终端A崩溃后
+----+--------+--------+
| id | name | blance |
+----+--------+--------+
| 1 | 张三 | 100.00 |
+----+--------+--------+
1 row in set (0.00 sec)
-- 实验二
--终端A
mysql> show variables like 'autocommit'; --开启默认提交
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
| autocommit | ON |
+---------------+-------+
1 row in set (0.00 sec)
mysql> select * from account;
+----+--------+--------+
| id | name | blance |
+----+--------+--------+
| 1 | 张三 | 100.00 |
+----+--------+--------+
1 row in set (0.00 sec)
mysql> insert into account values (2, '李四', 10000);
Query OK, 1 row affected (0.01 sec)
mysql> select *from account; --数据已经插入
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 100.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> Aborted --异常终止
--终端B
mysql> select * from account; --终端A崩溃前
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 100.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> select * from account; --终端A崩溃后，并不影响，已经持久化。autocommit
起作用
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 100.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
```

结论：  

> - 只要输入begin或者start transaction，事务便必须要通过commit提交，才会持久化，与是否设置set autocommit无关。
> - 事务可以手动回滚，同时，当操作异常，MySQL会自动回滚
> - 对于 InnoDB 每一条 SQL 语言都默认封装成事务，自动提交。（select有特殊情况，因为MySQL 有 MVCC ）
> - 从上面的例子，我们能看到事务本身的原子性(回滚)，持久性(commit)
> - 那么隔离性？一致性？  

事务操作注意事项：

> - 如果没有设置保存点，也可以回滚，只能回滚到事务的开始。直接使用 rollback(前提是事务还没有提交)
> - 如果一个事务被提交了（commit），则不可以回退（rollback）
> - 可以选择回退到哪个保存点
> - InnoDB 支持事务， MyISAM 不支持事务
> - 开始事务可以使 start transaction 或者 begin  

### 5.2事务隔离级别

**如何理解隔离性**

> - MySQL服务可能会同时被多个客户端进程(线程)访问，访问的方式以事务方式进行
> - 一个事务可能由多条SQL构成，也就意味着，任何一个事务，都有执行前，执行中，执行后的阶
>   段。而所谓的原子性，其实就是让用户层，要么看到执行前，要么看到执行后。执行中出现问题，
>   可以随时回滚。所以单个事务，对用户表现出来的特性，就是原子性。
> - 但，毕竟所有事务都要有个执行过程，那么在多个事务各自执行多个SQL的时候，就还是有可能会
>   出现互相影响的情况。比如：多个事务同时访问同一张表，甚至同一行数据。
> - 就如同你妈妈给你说：你要么别学，要学就学到最好。至于你怎么学，中间有什么困难，你妈妈不
>   关心。那么你的学习，对你妈妈来讲，就是原子的。那么你学习过程中，很容易受别人干扰，此
>   时，就需要将你的学习隔离开，保证你的学习环境是健康的。
> - 数据库中，为了保证事务执行过程中尽量不受干扰，就有了一个重要特征：隔离性
> - 数据库中，允许事务受不同程度的干扰，就有了一种重要特征：隔离级别  

**隔离级别**

> - **读未提交【Read Uncommitted】：** 在该隔离级别，所有的事务都可以看到其他事务没有提交的
>   执行结果。（实际生产中不可能使用这种隔离级别的），但是相当于没有任何隔离性，也会有很多
>   并发问题，如脏读，幻读，不可重复读等，我们上面为了做实验方便，用的就是这个隔离性。
> - **读提交【Read Committed】 ：**该隔离级别是大多数数据库的默认的隔离级别（不是 MySQL 默
>   认的）。它满足了隔离的简单定义:一个事务只能看到其他的已经提交的事务所做的改变。这种隔离
>   级别会引起不可重复读，即一个事务执行时，如果多次 select， 可能得到不同的结果。
> - **可重复读【Repeatable Read】：** 这是 MySQL 默认的隔离级别，它确保同一个事务，在执行
>   中，多次读取操作数据时，会看到同样的数据行。但是会有幻读问题。
> - **串行化【Serializable】:** 这是事务的最高隔离级别，它通过强制事务排序，使之不可能相互冲突，
>   从而解决了幻读的问题。它在每个读的数据行上面加上共享锁，。但是可能会导致超时和锁竞争
>   （这种隔离级别太极端，实际生产基本不使用）

隔离级别如何实现：隔离，基本都是通过锁实现的，不同的隔离级别，锁的使用是不同的。常见有，表
锁，行锁，读锁，写锁，间隙锁(GAP),Next-Key锁(GAP+行锁)等。不过，我们目前现有这个认识行，
先关注上层使用。

**查看与设置隔离性**      

```mysql
-- 查看
mysql> SELECT @@global.tx_isolation; --查看全局隔级别
+-----------------------+
| @@global.tx_isolation |
+-----------------------+
| REPEATABLE-READ |
+-----------------------+
1 row in set, 1 warning (0.00 sec)
mysql> SELECT @@session.tx_isolation; --查看会话(当前)全局隔级别
+------------------------+
| @@session.tx_isolation |
+------------------------+
| REPEATABLE-READ |
+------------------------+
1 row in set, 1 warning (0.00 sec)
mysql> SELECT @@tx_isolation; --默认同上
+-----------------+
| @@tx_isolation |
+-----------------+
| REPEATABLE-READ |
+-----------------+
1 row in set, 1 warning (0.00 sec)
--设置
-- 设置当前会话 or 全局隔离级别语法
SET [SESSION | GLOBAL] TRANSACTION ISOLATION LEVEL {READ UNCOMMITTED | READ
COMMITTED | REPEATABLE READ | SERIALIZABLE}
--设置当前会话隔离性，另起一个会话，看不多，只影响当前会话
mysql> set session transaction isolation level serializable; -- 串行化
Query OK, 0 rows affected (0.00 sec)
mysql> SELECT @@global.tx_isolation; --全局隔离性还是RR
+-----------------------+
| @@global.tx_isolation |
+-----------------------+
| REPEATABLE-READ |
+-----------------------+
1 row in set, 1 warning (0.00 sec)
mysql> SELECT @@session.tx_isolation; --会话隔离性成为串行化
+------------------------+
| @@session.tx_isolation |
+------------------------+
| SERIALIZABLE |
+------------------------+
1 row in set, 1 warning (0.00 sec)
mysql> SELECT @@tx_isolation; --同上
+----------------+
| @@tx_isolation |
+----------------+
| SERIALIZABLE |
+----------------+
1 row in set, 1 warning (0.00 sec)
--设置全局隔离性，另起一个会话，会被影响
mysql> set global transaction isolation level READ UNCOMMITTED;
Query OK, 0 rows affected (0.00 sec)
mysql> SELECT @@global.tx_isolation;
+-----------------------+
| @@global.tx_isolation |
+-----------------------+
| READ-UNCOMMITTED |
+-----------------------+
1 row in set, 1 warning (0.00 sec)
mysql> SELECT @@session.tx_isolation;
+------------------------+
| @@session.tx_isolation |
+------------------------+
| READ-UNCOMMITTED |
+------------------------+
1 row in set, 1 warning (0.00 sec)
mysql> SELECT @@tx_isolation;
+------------------+
| @@tx_isolation |
+------------------+
| READ-UNCOMMITTED |
+------------------+
1 row in set, 1 warning (0.00 sec)
-- 注意，如果没有现象，关闭mysql客户端，重新连接
```

#### 5.2.1读未提交[Read Uncommitted  ]

缺陷：脏读

```mysql
--几乎没有加锁，虽然效率高，但是问题太多，严重不建议采用
--终端A
-- 设置隔离级别为 读未提交
mysql> set global transaction isolation level read uncommitted;
Query OK, 0 rows affected (0.00 sec)
--重启客户端
mysql> select @@tx_isolation;
+------------------+
| @@tx_isolation |
+------------------+
| READ-UNCOMMITTED |
+------------------+
1 row in set, 1 warning (0.00 sec)
mysql> select * from account;
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 100.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> begin; --开启事务
Query OK, 0 rows affected (0.00 sec)
mysql> update account set blance=123.0 where id=1; --更新指定行
Query OK, 1 row affected (0.05 sec)
Rows matched: 1 Changed: 1 Warnings: 0
--没有commit哦！！！
--终端B
mysql> begin;
mysql> select * from account;
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 123.00 | --读到终端A更新但是未commit的数据[insert，
delete同样]
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
--一个事务在执行中，读到另一个执行中事务的更新(或其他操作)但是未commit的数据，这种现象叫做脏读
(dirty read)
```

#### 5.2.2读提交[Read Committed]

缺陷：不可重复读

```mysql
-- 终端A
mysql> set global transaction isolation level read committed;
Query OK, 0 rows affected (0.00 sec)
--重启客户端
mysql> select * from account; --查看当前数据
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 123.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> begin; --手动开启事务，同步的开始终端B事务
Query OK, 0 rows affected (0.00 sec)
mysql> update account set blance=321.0 where id=1; --更新张三数据
Query OK, 1 row affected (0.00 sec)
Rows matched: 1 Changed: 1 Warnings: 0
--切换终端到终端B，查看数据。
mysql> commit; --commit提交！
Query OK, 0 rows affected (0.01 sec)
--切换终端到终端B，再次查看数据。
--终端B
mysql> begin; --手动开启事务，和终端A一前一后
Query OK, 0 rows affected (0.00 sec)
mysql> select * from account; --终端A commit之前，查看不到
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 123.00 | --老的值
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
--终端A commit之后，看到了！
--but，此时还在当前事务中，并未commit，那么就造成了，同一个事务内，同样的读取，在不同的时间段
(依旧还在事务操作中！)，读取到了不同的值，这种现象叫做不可重复读(non reapeatable read)！！
（这个是问题吗？？）
mysql> select *from account;
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 321.00 | --新的值
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
```

#### 5.2.3可重复读[Repeatable Read]

```mysql
--终端A
mysql> set global transaction isolation level repeatable read; --设置全局隔离级别
RR
Query OK, 0 rows affected (0.01 sec)
--关闭终端重启
mysql> select @@tx_isolation;
+-----------------+
| @@tx_isolation |
+-----------------+
| REPEATABLE-READ | --隔离级别RR
+-----------------+
1 row in set, 1 warning (0.00 sec)
mysql> select *from account; --查看当前数据
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> begin; --开启事务，同步的，终端B也开始事务
Query OK, 0 rows affected (0.00 sec)
mysql> update account set blance=4321.0 where id=1; --更新数据
Query OK, 1 row affected (0.00 sec)

Rows matched: 1 Changed: 1 Warnings: 0
--切换到终端B，查看另一个事务是否能看到
mysql> commit; --提交事务
--切换终端到终端B，查看数据。
--终端B
mysql> begin;
Query OK, 0 rows affected (0.00 sec)
mysql> select * from account; --终端A中事务 commit之前，查看当前表中数据，数据未更新
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> select * from account; --终端A中事务 commit 之后，查看当前表中数据，数据未更新
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
--可以看到，在终端B中，事务无论什么时候进行查找，看到的结果都是一致的，这叫做可重复读！
mysql> commit; --结束事务
Query OK, 0 rows affected (0.00 sec)
mysql> select * from account; --再次查看，看到最新的更新数据
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
----------------------------------------------------------------
--如果将上面的终端A中的update操作，改成insert操作，会有什么问题？？
--终端A
mysql> select *from account;
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> begin; --开启事务，终端B同步开启
Query OK, 0 rows affected (0.00 sec)
mysql> insert into account (id,name,blance) values(3, '王五', 5432.0);
Query OK, 1 row affected (0.00 sec)
--切换到终端B，查看另一个事务是否能看到
mysql> commit; --提交事务
Query OK, 0 rows affected (0.00 sec)
--切换终端到终端B，查看数据。
mysql> select * from account;
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
| 3 | 王五 | 5432.00 |
+----+--------+----------+
3 rows in set (0.00 sec)
--终端B
mysql> begin; --开启事务
Query OK, 0 rows affected (0.00 sec)
mysql> select * from account; --终端A commit前 查看
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> select * from account; --终端A commit后 查看
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> select * from account; --多次查看，发现终端A在对应事务中insert的数据，在终端B的事
务周期中，也没有什么影响，也符合可重复的特点。但是，一般的数据库在可重复读情况的时候，无法屏蔽其
他事务insert的数据(为什么？因为隔离性实现是对数据加锁完成的，而insert待插入的数据因为并不存
在，那么一般加锁无法屏蔽这类问题),会造成虽然大部分内容是可重复读的，但是insert的数据在可重复读
情况被读取出来，导致多次查找时，会多查找出来新的记录，就如同产生了幻觉。这种现象，叫做幻读
(phantom read)。很明显，MySQL在RR级别的时候，是解决了幻读问题的(解决的方式是用Next-Key锁
(GAP+行锁)解决的。这块比较难，有兴趣同学了解一下)。
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
Query OK, 0 rows affected (0.00 sec)
mysql> insert into account (id,name,blance) values(3, '王五', 5432.0);
Query OK, 1 row affected (0.00 sec)
--切换到终端B，查看另一个事务是否能看到
mysql> commit; --提交事务
Query OK, 0 rows affected (0.00 sec)
--切换终端到终端B，查看数据。
mysql> select * from account;
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
| 3 | 王五 | 5432.00 |
+----+--------+----------+
3 rows in set (0.00 sec)
--终端B
mysql> begin; --开启事务
Query OK, 0 rows affected (0.00 sec)
mysql> select * from account; --终端A commit前 查看
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> select * from account; --终端A commit后 查看
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
mysql> select * from account; --多次查看，发现终端A在对应事务中insert的数据，在终端B的事
务周期中，也没有什么影响，也符合可重复的特点。但是，一般的数据库在可重复读情况的时候，无法屏蔽其
他事务insert的数据(为什么？因为隔离性实现是对数据加锁完成的，而insert待插入的数据因为并不存
在，那么一般加锁无法屏蔽这类问题),会造成虽然大部分内容是可重复读的，但是insert的数据在可重复读
情况被读取出来，导致多次查找时，会多查找出来新的记录，就如同产生了幻觉。这种现象，叫做幻读
(phantom read)。很明显，MySQL在RR级别的时候，是解决了幻读问题的(解决的方式是用Next-Key锁
(GAP+行锁)解决的。这块比较难，有兴趣同学了解一下)。
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
+----+--------+----------+
2 rows in set (0.00 sec)
```

#### 5.2.4串行化[serializable]

```mysql
--对所有操作全部加锁，进行串行化，不会有问题，但是只要串行化，效率很低，几乎完全不会被采用
--终端A
mysql> set global transaction isolation level serializable;
Query OK, 0 rows affected (0.00 sec)
mysql> select @@tx_isolation;
+----------------+
| @@tx_isolation |
+----------------+
| SERIALIZABLE |
+----------------+
1 row in set, 1 warning (0.00 sec)
mysql> begin; --开启事务，终端B同步开启
Query OK, 0 rows affected (0.00 sec)
mysql> select * from account; --两个读取不会串行化，共享锁
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
| 3 | 王五 | 5432.00 |
+----+--------+----------+
3 rows in set (0.00 sec)
mysql> update account set blance=1.00 where id=1; --终端A中有更新或者其他操作，会阻
塞。直到终端B事务提交。
Query OK, 1 row affected (18.19 sec)
Rows matched: 1 Changed: 1 Warnings: 0
--终端B
mysql> begin;
Query OK, 0 rows affected (0.00 sec)
mysql> select * from account; --两个读取不会串行化
+----+--------+----------+
| id | name | blance |
+----+--------+----------+
| 1 | 张三 | 4321.00 |
| 2 | 李四 | 10000.00 |
| 3 | 王五 | 5432.00 |
+----+--------+----------+
3 rows in set (0.00 sec)
mysql> commit; --提交之后，终端A中的update才会提交。
Query OK, 0 rows affected (0.00 sec)
```

![image-20240417110042977](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240417110042977.png)

总结：  

- 其中隔离级别越严格，安全性越高，但数据库的并发性能也就越低，往往需要在两者之间找一个平衡点 .
- 不可重复读的重点是修改和删除：同样的条件, 你读取过的数据,再次读取出来发现值不一样了幻读的重点在于新增：同样的条件, 第1次和第2次读出来的记录数不一样.
- 说明： mysql 默认的隔离级别是可重复读,一般情况下不要修改  
- 上面的例子可以看出，事务也有长短事务这样的概念。事务间互相影响，指的是事务在并行执行的时候，即都没有commit的时候，影响会比较大  

| 隔离级别 | 脏读 (Dirty Read) | 可重复读 (Repeatable Read) | 幻读 (Phantom Read) | 加锁读 (Lock Read) |
| -------- | ----------------- | -------------------------- | ------------------- | ------------------ |
| 读未提交 | √                 | ×                          | √                   | ×                  |
| 读已提交 | ×                 | ×                          | √                   | ×                  |
| 可重复读 | ×                 | √                          | ×                   | ×                  |
| 可串行化 | ×                 | √                          | ×                   | √                  |

这个表格使用了√代表“是”，使用×代表“否”，来表示每种隔离级别的特性。

**一致性(Consistency)**

- 事务执行的结果，必须使数据库从一个一致性状态，变到另一个一致性状态。当数据库只包含事务成功提交的结果时，数据库处于一致性状态。如果系统运行发生中断，某个事务尚未完成而被迫中断，而该未完成的事务对数据库所做的修改已被写入数据库，此时数据库就处于一种不正确（不一致）的状态。因此一致性是通过原子性来保证的。
- 其实一致性和用户的业务逻辑强相关，一般MySQL提供技术支持，但是一致性还是要用户业务逻辑做支撑，也就是，一致性，是由用户决定的.
- 而技术上，通过AID保证Consistency.

推荐阅读：

```
https://www.jianshu.com/p/398d788e1083
https://tech.meituan.com/2014/08/20/innodb-lock.html
https://www.cnblogs.com/aspirant/p/9177978.html
```

```
基本上，了解了上面的知识，在MySQL事务使用上，肯定没有问题。不过，这块设计很优秀，也是面试中可能
被问到的，一般学生，如果能说出上面的内容，就已经不错了。但是如果我们能更详细，更深入的谈论这个问
题，那么对我们的面试与学习肯定是大大的有帮助。
RR级别的时候，多个事务的update，多个事务的insert,多个事务的delete，是否会有加锁现象?
现象结果是，update，insert，delete之间是会有加锁现象的，但是select和这些操作是不冲突的。这就
是通过读写锁(锁有行锁或者表锁)+MVCC完成隔离性。
```

### 5.3如何理解隔离性

**数据库中并发的场景有三种**

- `读读`：不存在任何问题，也不需要并发控制
- `读-写` ：有线程安全问题，可能会造成事务隔离性问题，可能遇到脏读，幻读，不可重复读
- `写-写` ：有线程安全问题，可能会存在更新丢失问题，比如第一类更新丢失，第二类更新丢失(后面
  补充)  

**读写**

多版本并发控制（ `MVCC` ）是一种用来解决 **读-写冲突 的无锁并发控制**
为事务分配单向增长的事务ID，为每个修改保存一个版本，版本与事务ID关联，读操作只读该事务开始
前的数据库的快照。 所以 MVCC 可以为数据库解决以下问题  

- 在并发读写数据库时，可以做到在读操作时不用阻塞写操作，写操作也不用阻塞读操作，提高了数
  据库并发读写的性能  
- 同时还可以解决脏读，幻读，不可重复读等事务隔离问题，但不能解决更新丢失问题  

理解 MVCC 需要知道三个前提知识：  

- 三个隐藏字段
- undo日志
- read veiw

**3个记录隐藏列字段**

- `DB_TRX_ID` ：6 byte，最近修改( 修改/插入 )事务ID，记录创建这条记录/最后一次修改该记录的事务ID
- `DB_ROLL_PTR` : 7 byte，回滚指针，指向这条记录的上一个版本（简单理解成，指向历史版本就行，这些数据一般在 undo log 中）  
- `DB_ROW_ID` : 6 byte，隐含的自增ID（隐藏主键），如果数据表没有主键， InnoDB 会自动以
  DB_ROW_ID 产生一个聚簇索引  
- 补充：实际还有一个删除flag隐藏字段, 既记录被更新或删除并不代表真的删除，而是删除flag变了    

​	**假设测试表结构是：**  

```mysql
mysql> create table if not exists student(
name varchar(11) not null,
age int not null
);
mysql> insert into student (name, age) values ('张三', 28);
Query OK, 1 row affected (0.05 sec)
mysql> select * from student;
+--------+-----+
| name | age |
+--------+-----+
| 张三 | 28 |
+--------+-----+
1 row in set (0.00 sec)
```

上面描述的意思是：  

| name | age  | `DB_TRX_ID`(创建该记录的事 务ID) | `DB_ROW_ID`(隐式 主键) | `DB_ROLL_PTR`(回滚 指针) |
| :--: | :--: | :------------------------------: | :--------------------: | :----------------------: |
| 张三 |  28  |               null               |           1            |           null           |

我们目前并不知道创建该记录的事务ID，隐式主键，我们就默认设置成null，1。第一条记录也没有其他版本，我们设置回滚指针为null。

**`undo`日志**

> MySQL 将来是以服务进程的方式，在内存中运行。我们之前所讲的所有机制：索引，事务，隔离性，日志等，都是在内存中完成的，即在 MySQL 内部的相关缓冲区中，保存相关数据，完成各种判断操作。然后在合适的时候，将相关数据刷新到磁盘当中的。
> 所以，我们这里理解undo log，简单理解成，就是 MySQL 中的一段内存缓冲区，用来保存日志数据的就行

模拟`MVCC` 

> 现在有一个事务10(仅仅为了好区分)，对student表中记录进行修改(update)：将name(张三)改成name(李四)
>
> - 事务10,因为要修改，所以要先给该记录加行锁  
> - 修改前，现将改行记录拷贝到undo log中，所以，undo log中就有了一行副本数据。(原理就是写时拷贝)
> - 所以现在 MySQL 中有两行同样的记录。现在修改原始记录中的name，改成 '李四'。并且修改原始记录的隐藏字段 DB_TRX_ID 为当前 事务10 的ID, 我们默认从 10 开始，之后递增。而原始记录的回滚指针 DB_ROLL_PTR 列，里面写入undo log中副本数据的地址，从而指向副本记录，既表示我的上一个版本就是它  
> - 事务10提交，释放锁  

![image-20240417155849284](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240417155849284.png)

备注：此时，最新的记录是’李四‘那条记录    

现在又有一个事务11，对student表中记录进行修改(update)：将age(28)改成age(38)  

- 事务11,因为也要修改，所以要先给该记录加行锁  
- 修改前，现将改行记录拷贝到undo log中，所以，undo log中就又有了一行副本数据。此时，新的副本，我们采用头插方式，插入undo log  
- 现在修改原始记录中的age，改成 38。并且修改原始记录的隐藏字段 DB_TRX_ID 为当前 事务11 的ID。而原始记录的回滚指针 DB_ROLL_PTR 列，里面写入undo log中副本数据的地址，从而指向副本记录，既表示我的上一个版本就是它  
- 事务11提交，释放锁

![image-20240417160128865](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240417160128865.png)

这样，我们就有了一个基于链表记录的历史版本链。所谓的回滚，无非就是用历史数据，覆盖当前数据 

上面的一个一个版本，我们可以称之为一个一个的快照   

```
##一些思考
上面是以更新（`upadte`）主讲的,如果是`delete`呢？
一样的，别忘了，删数据不是清空，而是设置flag为删除即可。也可以形成版本

如果是`insert`呢？因为`insert`是插入，也就是之前没有数据，那么`insert`也就没有历史版本。但是
一般为了回滚操作，insert的数据也是要被放入undo log中，如果当前事务commit了，那么这个undo log 的历史insert记录就可以被清空了

总结一下，也就是我们可以理解成，`update`和`delete`可以形成版本链，`insert`暂时不考虑。
那么`select`呢？

首先，`select`不会对数据做任何修改，所以，为`select`维护多版本，没有意义。不过，此时有个问题，
就是： select读取，是读取最新的版本呢？还是读取历史版本？

当前读：读取最新的记录，就是当前读。增删改，都叫做当前读，select也有可能当前读，比如：select
lock in share mode(共享锁), select for update （这个好理解，我们后面不讨论）

快照读：读取历史版本(一般而言)，就叫做快照读。(这个我们后面重点讨论)

我们可以看到，在多个事务同时删改查的时候，都是当前读，是要加锁的。那同时有select过来，如果也要读
取最新版(当前读)，那么也就需要加锁，这就是串行化。

但如果是快照读，读取历史版本的话，是不受加锁限制的。也就是可以并行执行！换言之，提高了效率，即MVCC的意义所在。

那么，是什么决定了，select是当前读，还是快照读呢？隔离级别!

那为什么要有隔离级别呢？

事务都是原子的。所以，无论如何，事务总有先有后。

但是经过上面的操作我们发现，事务从begin->CURD->commit，是有一个阶段的。也就是事务有执行前，执
行中，执行后的阶段。但，不管怎么启动多个事务，总是有先有后的。

那么多个事务在执行中，CURD操作是会交织在一起的。那么，为了保证事务的“有先有后”，是不是应该让不同
的事务看到它该看到的内容，这就是所谓的隔离性与隔离级别要解决的问题。

先来的事务，应不应该看到后来的事务所做的修改呢?
```

那么，如何保证，不同的事务，看到不同的内容呢？也就是如何如何实现隔离级别？  

`Read View`  

Read View就是事务进行 `快照读` 操作的时候生产的 读视图 (Read View)，在该事务执行的快照读的那一刻，会生成数据库系统当前的一个快照，记录并维护系统当前活跃事务的ID(当每个事务开启时，都会被分配一个ID, 这个ID是递增的，所以最新的事务，ID值越大)
Read View 在 MySQL 源码中,就是一个`类`，本质是`用来进行可见性判断的`。 即当我们某个事务执行快照读的时候，对该记录创建一个 Read View 读视图，把它比作条件,用来判断当前事务能够看到哪个版本的数据，既可能是当前最新的数据，也有可能是该行记录的 undo log 里面的某个版本的数据  



下面是 ReadView 结构,但为了减少同学们负担，我们简化一下  

```cpp
class ReadView {
// 省略...
private:
/** 高水位，大于等于这个ID的事务均不可见*/
trx_id_t m_low_limit_id
/** 低水位：小于这个ID的事务均可见 */
trx_id_t m_up_limit_id;
/** 创建该 Read View 的事务ID*/
trx_id_t m_creator_trx_id;
/** 创建视图时的活跃事务id列表*/
ids_t m_ids;
/** 配合purge，标识该视图不需要小于m_low_limit_no的UNDO LOG，
* 如果其他视图也不需要，则可以删除小于m_low_limit_no的UNDO LOG*/
trx_id_t m_low_limit_no;
/** 标记视图是否被关闭*/
bool m_closed;
// 省略...
};
```

```cpp
m_ids; //一张列表，用来维护Read View生成时刻，系统正活跃的事务ID
up_limit_id; //记录m_ids列表中事务ID最小的ID(没有写错)
low_limit_id; //ReadView生成时刻系统尚未分配的下一个事务ID，也就是目前已出现过的事务ID的最大值+1(也没有写错)
creator_trx_id //创建该ReadView的事务ID
```

我们在实际读取数据版本链的时候，是能读取到每一个版本对应的事务ID的，即：当前记录的
`DB_TRX_ID`   那么，我们现在手里面有的东西就有，当前快照读的 ReadView 和 版本链中的某一个记录的`DB_TRX_ID`.

所以现在的问题就是，当前快照读，应不应该读到当前版本记录。一张图，解决所有问题！  

![image-20240417181320083](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240417181320083.png) 

如果查到不应该看到当前版本，接下来就是遍历下一个版本，直到符合条件，即可以看到。上面的
readview 是当你进行select的时候，会自动形成  

**整体流程**

假设当前有条记录：  

| name | age  | `DB_TRX_ID`(创建该记录的事 务ID) | `DB_ROW_ID`(隐式 主键) | `DB_ROLL_PTR`(回滚 指针) |
| :--: | :--: | :------------------------------: | :--------------------: | :----------------------: |
| 张三 |  28  |               null               |           1            |           null           |

事务操作：

| 事务1[id=1] | 事务2[id=1] | 事务3[id=1] | 事务4[id=1] |
| :---------: | :---------: | :---------: | :---------: |
|  事务开始   |  事务开始   |  事务开始   |  事务开始   |
|     ...     |     ...     |     ...     | 修改且提交  |
|   进行中    |   快照读    |   进行中    |             |
|     ...     |     ...     |     ...     |             |

- 事务4：修改name(张三) 变成name(李四)  
- 当 `事务2` 对某行数据执行了 `快照读` ，数据库为该行数据生成一个 `Read View` 读视图 

```cpp
//事务2的 Read View
m_ids; // 1,3
up_limit_id; // 1
low_limit_id; // 4 + 1 = 5，原因：ReadView生成时刻，系统尚未分配的下一个事务ID
creator_trx_id // 2
```

 此时版本链是：  

![image-20240417182052184](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240417182052184.png)

- 只有事务4修改过该行记录，并在事务2执行快照读前，就提交了事务  

![image-20240417182503755](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240417182503755.png)

- 我们的事务2在快照读该行记录的时候，就会拿该行记录的 DB_TRX_ID 去跟`up_limit_id`,`low_limit_id`和活跃事务ID列表(trx_list) 进行比较，判断当前事务2能看到该记录的版本。

  ```cpp
  //事务2的 Read View
  m_ids; // 1,3
  up_limit_id; // 1
  low_limit_id; // 4 + 1 = 5，原因：ReadView生成时刻，系统尚未分配的下一个事务ID
  creator_trx_id // 2
  //事务4提交的记录对应的事务ID
  DB_TRX_ID=4
  //比较步骤
  DB_TRX_ID（4）< up_limit_id（1） ? 不小于，下一步
  DB_TRX_ID（4）>= low_limit_id(5) ? 不大于，下一步
  m_ids.contains(DB_TRX_ID) ? 不包含，说明，事务4不在当前的活跃事务中。
  //结论
  故，事务4的更改，应该看到。
  所以事务2能读到的最新数据记录是事务4所提交的版本，而事务4提交的版本也是全局角度上最新的版本
  ```

### 5.4RR 与 RC的本质区别 

**当前读和快照读在RR级别下的区别**

下面的代码经过测试，是完全没有问题的。要不要现场测试，主要看上课的时间允不允许  

> `select * from user lock in share mode` ,以加共享锁方式进行读取，对应的就是**当前读**。此
> 处只作为测试使用，不重讲 

测试表：     

```mysql
--设置RR模式下测试
mysql> set global transaction isolation level REPEATABLE READ;
Query OK, 0 rows affected (0.00 sec)
--重启终端
mysql> select @@tx_isolation;
+-----------------+
| @@tx_isolation |
+-----------------+
| REPEATABLE-READ |
+-----------------+
1 row in set, 1 warning (0.00 sec)
--依旧用之前的表
Create Table: CREATE TABLE `users` (
  `id` int(11) DEFAULT NULL,
  `age` int(11) DEFAULT NULL,
  `name` varchar(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8
--插入一条记录，用来测试
mysql> insert into users (id, age, name) values (1, 15,'黄蓉');
Query OK, 1 row affected (0.00 sec)
```

测试用例1-表1：  

|              事务A操作               |      事务A描 述      |                   事务B描述                    |                事务B操作                |
| :----------------------------------: | :------------------: | :--------------------------------------------: | :-------------------------------------: |
|               `begin`                |       开启事务       |                    开启事务                    |                 `begin`                 |
|         `select * from user`         | 快照读 (无影响) 查询 |                   快照读查询                   |          `select * from user`           |
| `update user set age=18 where id=1;` |     更新 age=18      |                       -                        |                    -                    |
|               `commit`               |       提交事务       |                       -                        |                    -                    |
|                                      |                      |        `select 快照读` ,没有读到 age=18        |          `select * from user`           |
|                                      |                      | `select lock in share mode当前读` , 读到age=18 | `select * from user lock in share mode` |

测试用例2-表2：  

|              事务A操作               |      事务A描 述      |                   事务B描述                    |                事务B操作                |
| :----------------------------------: | :------------------: | :--------------------------------------------: | :-------------------------------------: |
|               `begin`                |       开启事务       |                    开启事务                    |                 `begin`                 |
|         `select * from user`         | 快照读 (无影响) 查询 |                   快照读查询                   |                    -                    |
| `update user set age=18 where id=1;` |     更新 age=18      |                       -                        |                    -                    |
|               `commit`               |       提交事务       |                       -                        |                    -                    |
|                                      |                      |        `select 快照读` ,没有读到 age=18        |          `select * from user`           |
|                                      |                      | `select lock in share mode当前读` , 读到age=18 | `select * from user lock in share mode` |

- 用例1与用例2：唯一区别仅仅是 表1 的事务B在事务A修改age前 快照读 过一次age数据  
- 而 `表2` 的事务B在事务A修改age前没有进行过快照读  

结论：

- 事务中快照读的结果是非常依赖该事务首次出现快照读的地方，即某个事务中首次出现快照读，决
  定该事务后续快照读结果的能力  
- delete同样如此  

**RR 与 RC的本质区别**  

- 总之在RC隔离级别下，是每个快照读都会生成并获取最新的Read View；而在RR隔离级别下，则是
  同一个事务中的第一个快照读才会创建Read View, 之后的快照读获取的都是同一个Read View。  
- 正是RC每次快照读，都会形成Read View，所以，RC才会有不可重复读问题  



## 6.推荐阅读  

```
关于这块，有很好的文章，推荐大家阅读
https://blog.csdn.net/SnailMann/article/details/94724197
https://www.cnblogs.com/f-ck-need-u/archive/2018/05/08/9010872.html
https://blog.csdn.net/chenghan_yang/article/details/97630626
```

