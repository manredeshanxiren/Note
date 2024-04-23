# MySQL内置函数

## 1.日期和时间相关的函数

| 函数                         | 描述                                                     |
| ---------------------------- | -------------------------------------------------------- |
| now()                        | 返回当前日期和时间                                       |
| curdate()                    | 返回当前日期                                             |
| curtime()                    | 返回当前时间                                             |
| date(datetime)               | 提取日期时间表达式的日期部分                             |
| time(datetime)               | 提取日期时间表达式的时间部分                             |
| year(date)                   | 提取日期的年份                                           |
| month(date)                  | 提取日期的月份                                           |
| day(date)                    | 提取日期的天数                                           |
| hour(time)                   | 提取时间的小时                                           |
| minute(time)                 | 提取时间的分钟                                           |
| second(time)                 | 提取时间的秒数                                           |
| dayname(date)                | 返回日期的星期几名称（全名）                             |
| monthname(date)              | 返回日期的月份名称（全名）                               |
| dayofweek(date)              | 返回日期的星期几，其中1表示星期日，2表示星期一，以此类推 |
| dayofmonth(date)             | 返回日期的月份中的天数                                   |
| dayofyear(date)              | 返回日期的年份中的天数                                   |
| week(date)                   | 返回年份和周数的数字                                     |
| weekday(date)                | 返回日期的星期几，其中0表示星期一，1表示星期二，以此类推 |
| yearweek(date)               | 返回日期的年份和周数，格式为YYYYWW（年份+周数）          |
| last_day(date)               | 返回给定日期所在月份的最后一天                           |
| adddate(date,整数)           | 将一个日期或日期时间表达式加上一段时间间隔               |
| date_add(date,整数)          | 将一个日期或日期时间表达式加上一段时间间隔               |
| date_sub(date,整数)          | 将一个日期或日期时间表达式减去一段时间间隔               |
| date_format(date,格式字符串) | 将日期或日期时间表达式格式化为指定的格式                 |

- 获得年月日：

  ```mysql
  select current_date();
  +----------------+
  | current_date() |
  +----------------+
  | 2017-11-19 |
  +----------------+
  ```

- 获得时分秒：

  ```mysql
  select current_time();
  +----------------+
  | current_time() |
  +----------------+
  | 13:51:21 |
  +----------------+
  ```

- 获得时间戳：

  ```mysql
  select current_timestamp();
  +---------------------+
  | current_timestamp() |
  +---------------------+
  | 2017-11-19 13:51:48 |
  +---------------------+
  ```

- 在日期的基础上加日期：  

  ```mysql
  select date_add('2017-10-28', interval 10 day);
  +-----------------------------------------+
  | date_add('2017-10-28', interval 10 day) |
  +-----------------------------------------+
  | 2017-11-07 |
  +-----------------------------------------+
  ```

- 在日期的基础上减去时间：  

  ```mysql
  select date_sub('2017-10-1', interval 2 day);
  +---------------------------------------+
  | date_sub('2017-10-1', interval 2 day) |
  +---------------------------------------+
  | 2017-09-29 |
  +---------------------------------------+
  ```

- 计算两个日期之间相差多少天：

  ```mysql
  select datediff('2017-10-10', '2016-9-1');
  +------------------------------------+
  | datediff('2017-10-10', '2016-9-1') |
  +------------------------------------+
  | 404 |
  +------------------------------------+
  ```

  案例-1：

  - 创建一张表，记录生日  

    ```mysql
    create table tmp(
    id int primary key auto_increment,
    birthday date
    );
    ```

  - 添加当前日期：  

    ```mysql
    mysql> insert tmp (birthday) values(curdate());
    Query OK, 1 row affected (0.00 sec)
    
    mysql> select * from tmp;
    +----+------------+
    | id | birthday   |
    +----+------------+
    |  1 | 2024-04-08 |
    +----+------------+
    ```

    案例-2：

    - 创建一个留言表

      ```mysql
      mysql> create table msg (
      id int primary key auto_increment,
      content varchar(30) not null,
      sendtime datetime
      );
      ```

    - 插入数据

      ```mysql
      mysql> insert into msg(content,sendtime) values('hello1', now());
      Query OK, 1 row affected (0.00 sec)
      
      mysql> insert into msg(content,sendtime) values('hello2', now());
      Query OK, 1 row affected (0.00 sec)
      
      mysql> select * from msg;
      +----+---------+---------------------+
      | id | content | sendtime            |
      +----+---------+---------------------+
      |  1 | hello1  | 2024-04-08 19:21:17 |
      |  2 | hello2  | 2024-04-08 19:21:22 |
      +----+---------+---------------------+
      2 rows in set (0.00 sec)
      ```

    - 显示所有留言信息，发布日期只显示日期，不用显示时间  

      ```mysql
      mysql> select content, date(sendtime) from msg;
      +---------+----------------+
      | content | date(sendtime) |
      +---------+----------------+
      | hello1  | 2024-04-08     |
      | hello2  | 2024-04-08     |
      +---------+----------------+
      2 rows in set (0.00 sec)
      ```

    - 请查询在2分钟内发布的帖子  

      ```mysql
      mysql> insert into msg(content,sendtime) values('hello3', now());
      Query OK, 1 row affected (0.00 sec)
      
      mysql> select content, date(sendtime) from msg where sendtime + 2 > now();
      +---------+----------------+
      | content | date(sendtime) |
      +---------+----------------+
      | hello3  | 2024-04-08     |
      +---------+----------------+
      1 row in set (0.00 sec)
      ```


## 2.字符串函数

| 函数                                 | 描述                                               |
| ------------------------------------ | -------------------------------------------------- |
| concat(str1, str2,...)               | 连接两个或多个字符串                               |
| concat_ws(separator, str1, str2,...) | 使用指定的分隔符连接两个或多个字符串               |
| length(str)                          | 返回字符串的长度                                   |
| char_length(str)                     | 返回字符串的字符数（多字节字符算一个字符）         |
| upper(str)                           | 将字符串转换为大写                                 |
| lower(str)                           | 将字符串转换为小写                                 |
| trim(str)                            | 去除字符串两端的空格                               |
| ltrim(str)                           | 去除字符串左端的空格                               |
| rtrim(str)                           | 去除字符串右端的空格                               |
| substring(str, start, length)        | 返回字符串的子串，从指定位置开始，指定长度的字符   |
| replace(str, from_str, to_str)       | 替换字符串中的子串为另一个字符串                   |
| reverse(str)                         | 反转字符串                                         |
| locate(substr, str, start)           | 返回子串在字符串中第一次出现的位置                 |
| instr(str, substr)                   | 返回子串在字符串中第一次出现的位置，没有则返回0    |
| left(str, length)                    | 返回字符串左边指定长度的子串                       |
| right(str, length)                   | 返回字符串右边指定长度的子串                       |
| insert(str, pos, length, new_str)    | 在字符串中插入新的字符串                           |
| mid(str, start, length)              | 返回字符串中从指定位置开始的一段子串               |
| substring_index(str, delim, count)   | 返回字符串中指定分隔符出现次数之前或之后的子串     |
| format(number, decimal_places)       | 将数字格式化为带有千位分隔符和指定小数位数的字符串 |
| charset(str)                         | 返回字符串的字符集名称                             |

- 获取emp表的ename列的字符集  

  ```mysql
  select charset(ename) from EMP;
  ```

- 要求显示exam_result表中的信息，显示格式：“XXX的语文是XXX分，数学XXX分，英语XXX分”  

  ```mysql
  select concat(name, '的语文是',chinese,'分，数学是',math,'分') as '分数' from student;
  ```

- 求学生表中学生姓名占用的字节数  

  ```mysql
  select length(name), name from student;
  ```

  > 注意：length函数返回字符串长度，以字节为单位。如果是多字节字符则计算多个字节数；
  > 如果是单字节字符则算作一个字节。比如：字母，数字算作一个字节，中文表示多个字节数
  > （与字符集编码有关）  

- 将EMP表中所有名字中有S的替换成'上海'  

  ```mysql
  select replace(ename, 'S', '上海') ,ename from EMP;
  ```

- 截取EMP表中ename字段的第二个到第三个字符  

  ```mysql
  select substring(ename, 2, 2), ename from EMP;
  ```

- 以首字母小写的方式显示所有员工的姓名  

  ```mysql
  select concat(lcase(substring(ename, 1, 1)),substring(ename,2)) from EMP;
  ```


## 3.数学函数

| 函数           | 描述                                                 |
| -------------- | ---------------------------------------------------- |
| abs(x)         | 返回一个数的绝对值                                   |
| sign(x)        | 返回一个数的符号函数：-1（负数）、0（零）、1（正数） |
| sqrt(x)        | 返回一个数的平方根                                   |
| power(x, y)    | 返回 x 的 y 次幂                                     |
| exp(x)         | 返回 e 的 x 次方（e 为自然对数的底）                 |
| log(x)         | 返回 x 的自然对数（基数为 e）                        |
| log2(x)        | 返回 x 的以 2 为底的对数                             |
| log10(x)       | 返回 x 的以 10 为底的对数                            |
| ceil(x)        | 返回不小于 x 的最小整数                              |
| floor(x)       | 返回不大于 x 的最大整数                              |
| round(x)       | 返回最接近 x 的整数                                  |
| truncate(x, d) | 返回 x 截断到 d 位小数                               |
| mod(x, y)      | 返回 x 除以 y 的余数                                 |
| rand()         | 返回 0 到 1 之间的随机数                             |
| pi()           | 返回圆周率 π 的值                                    |
| radians(x)     | 将角度 x 转换为弧度                                  |
| degrees(x)     | 将弧度 x 转换为角度                                  |

- 绝对值  

  ```mysql
  select abs(-100.2);
  ```

- 向上取整  

  ```mysql
  select ceiling(23.04);
  ```

- 向下取整  

  ```mysql
  select floor(23.7);
  ```

- 保留2位小数位数（小数四舍五入)  

  ```mysql
  select format(12.3456, 2);
  ```

- 产生随机数  

  ```mysql
  select rand();
  ```

## 4.其他函数

- user() 查询当前用户  

  ```mysql
  select user();
  ```

- md5(str)对一个字符串进行md5摘要，摘要后得到一个32位字符串  

  ```mysql
  select md5('admin')
  ```

  ```mysql
  +----------------------------------+
  | md5('admin') |
  +----------------------------------+
  | 21232f297a57a5a743894a0e4a801fc3 |
  +----------------------------------+
  ```

- database()显示当前正在使用的数据库  

  ```mysql
  select database();
  ```

- password()函数，MySQL数据库使用该函数对用户加密  

  ```mysql
  select password('root');
  +-------------------------------------------+
  | password('root') |
  +-------------------------------------------+
  | *81F5E21E35407D884A6CD4A731AEBFB6AF209E1B |
  +-------------------------------------------+
  ```

- `ifnull（val1， val2）` 如果val1为null，返回val2，否则返回val1的值  

```mysql
select ifnull('abc', '123');
+----------------------+
| ifnull('abc', '123') |
+----------------------+
| abc |
+----------------------+
1 row in set (0.01 sec)
select ifnull(null, '123');
+---------------------+
| ifnull(null, '123') |
+---------------------+
| 123 |
+---------------------+
1 row in set (0.00 sec)
```

