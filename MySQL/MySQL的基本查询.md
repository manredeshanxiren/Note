# MySQL的基本查询

## 1.表的增删查改

CRUD : Create(创建), Retrieve(读取)，Update(更新)，Delete（删除）  

### 1.1Create

在MySQL中，INSERT操作用于向数据库表中插入新的行。以下是INSERT操作的基本语法：

```sql
INSERT INTO table_name (column1, column2, column3, ...)
VALUES (value1, value2, value3, ...);
sql复制代码
```

- `table_name` 是要插入数据的表名。
- `column1, column2, column3, ...` 是要插入数据的列名，如果省略列名，则表示要插入所有列。
- `value1, value2, value3, ...` 是要插入到相应列的数据值。

例子：

```mysql
mysql> create table student (
    -> id int unsigned primary key auto_increment,
    -> sn int not null unique comment '学号',
    -> name varchar(20) not null,
    -> qq varchar(20) unique
    -> );
Query OK, 0 rows affected (0.02 sec)
```

#### 1.1.1单行数据+全列插入

```mysql
mysql> insert into student values(1,101,'张三', '213123123');
Query OK, 1 row affected (0.01 sec)

mysql> insert into student values(2,121,'张飞', '3531623123');
Query OK, 1 row affected (0.00 sec)

mysql> select * from student;
+----+-----+--------+------------+
| id | sn  | name   | qq         |
+----+-----+--------+------------+
|  1 | 101 | 张三   | 213123123  |
|  2 | 121 | 张飞   | 3531623123 |
+----+-----+--------+------------+
2 rows in set (0.00 sec)
```

#### 1.1.2多行数据+指定列插入

```mysql
mysql> insert into student (sn, name, qq) values (123, '曹操', '33242343');
Query OK, 1 row affected (0.00 sec)

mysql> insert into student (sn, name, qq) values (12223, '孙权', '34343242343');
Query OK, 1 row affected (0.00 sec)

mysql> select * from student;
+----+-------+--------+-------------+
| id | sn    | name   | qq          |
+----+-------+--------+-------------+
|  1 |   101 | 张三   | 213123123   |
|  2 |   121 | 张飞   | 3531623123  |
|  3 |   123 | 曹操   | 33242343    |
|  4 | 12223 | 孙权   | 34343242343 |
+----+-------+--------+-------------+
4 rows in set (0.00 sec)
```

#### 1.1.3插入或者更新

由于 **主键** 或者 **唯一键** 对应的值已经存在而导致插入失败  

```mysql
-- 主键冲突
INSERT INTO students (id, sn, name) VALUES (100, 10010, '唐大师');
ERROR 1062 (23000): Duplicate entry '100' for key 'PRIMARY'
-- 唯一键冲突
INSERT INTO students (sn, name) VALUES (20001, '曹阿瞒');
ERROR 1062 (23000): Duplicate entry '20001' for key 'sn'
```

可以选择性的进行同步更新操作 语法：  

```mysql
INSERT ... ON DUPLICATE KEY UPDATE
column = value [, column = value] ...
```

```mysql
INSERT INTO students (id, sn, name) VALUES (100, 10010, '唐大师')
ON DUPLICATE KEY UPDATE sn = 10010, name = '唐大师';
Query OK, 2 rows affected (0.47 sec)
-- 0 row affected: 表中有冲突数据，但冲突数据的值和 update 的值相等
-- 1 row affected: 表中没有冲突数据，数据被插入
-- 2 row affected: 表中有冲突数据，并且数据已经被更新
-- ON DUPLICATE KEY 当发生重复key的时候
```

#### 1.1.4Replace

```mysql
-- 主键 或者 唯一键 没有冲突，则直接插入；
-- 主键 或者 唯一键 如果冲突，则删除后再插入
REPLACE INTO students (sn, name) VALUES (20001, '曹阿瞒');
Query OK, 2 rows affected (0.00 sec)
-- 1 row affected: 表中没有冲突数据，数据被插入
-- 2 row affected: 表中有冲突数据，删除后重新插入
```

### 1.2Rerieve

语法：

```mysql
-- 选择要检索的列，可以使用 * 通配符表示所有列，或者列名列表指定特定列。
SELECT
    [DISTINCT] {* | {column [, column] ...}
-- 指定数据来源的表名。
[FROM table_name]
-- 可选项，用于筛选数据。只有满足条件的行才会被检索出来。
[WHERE ...]
-- 可选项，用于对检索结果进行排序。可以指定一个或多个列，并且可以指定升序（ASC）或降序（DESC）。
[ORDER BY column [ASC | DESC], ...]
-- 可选项，用于限制检索结果返回的行数。
LIMIT ...
```

案例：

```mysql
mysql> CREATE TABLE exam_result (
    -> id INT UNSIGNED PRIMARY KEY AUTO_INCREMENT,
    -> name VARCHAR(20) NOT NULL COMMENT '同学姓名',
    -> chinese float DEFAULT 0.0 COMMENT '语文成绩',
    -> math float DEFAULT 0.0 COMMENT '数学成绩',
    -> english float DEFAULT 0.0 COMMENT '英语成绩'
    -> );
Query OK, 0 rows affected (0.02 sec)
mysql> show tables;
+---------------+
| Tables_in_tt1 |
+---------------+
| exam_result   |
| student       |
+---------------+
2 rows in set (0.00 sec)

mysql> INSERT INTO exam_result (name, chinese, math, english) VALUES
    -> ('唐三藏', 67, 98, 56),
    -> ('孙悟空', 87, 78, 77),
    -> ('猪悟能', 88, 98, 90),
    -> ('曹孟德', 82, 84, 67),
    -> ('刘玄德', 55, 85, 45),
    -> ('孙权', 70, 73, 78),
    -> ('宋公明', 75, 65, 30);
Query OK, 7 rows affected (0.01 sec)
Records: 7  Duplicates: 0  Warnings: 0

```

#### 1.2.1select列

##### 1.2.1.1全列查询

```mysql
-- 通常情况下不建议使用 * 进行全列查询
-- 1. 查询的列越多，意味着需要传输的数据量越大；
-- 2. 可能会影响到索引的使用。（索引待后面课程讲解）
SELECT * FROM exam_result;
+----+-----------+-------+--------+--------+
| id | name | chinese | math | english |
+----+-----------+-------+--------+--------+
| 1 | 唐三藏 | 67 | 98 | 56 |
| 2 | 孙悟空 | 87 | 78 | 77 |
| 3 | 猪悟能 | 88 | 98 | 90 |
| 4 | 曹孟德 | 82 | 84 | 67 |
| 5 | 刘玄德 | 55 | 85 | 45 |
| 6 | 孙权 | 70 | 73 | 78 |
| 7 | 宋公明 | 75 | 65 | 30 |
+----+-----------+-------+--------+--------+
7 rows in set (0.00 sec)
```

##### 1.2.1.2指定列查询

```mysql
-- 指定列的顺序不需要按定义表的顺序来
SELECT id, name, english FROM exam_result;
+----+-----------+--------+
| id | name | english |
+----+-----------+--------+
| 1 | 唐三藏 | 56 |
| 2 | 孙悟空 | 77 |
| 3 | 猪悟能 | 90 |
| 4 | 曹孟德 | 67 |
| 5 | 刘玄德 | 45 |
| 6 | 孙权 | 78 |
| 7 | 宋公明 | 30 |
+----+-----------+--------+
7 rows in set (0.00 sec)
```

##### 1.2.1.3查询字段为表达式

```mysql
-- 表达式不包含字段
SELECT id, name, 10 FROM exam_result;
+----+-----------+----+
| id | name | 10 |
+----+-----------+----+
| 1 | 唐三藏 | 10 |
| 2 | 孙悟空 | 10 |
| 3 | 猪悟能 | 10 |
| 4 | 曹孟德 | 10 |
| 5 | 刘玄德 | 10 |
| 6 | 孙权 | 10 |
| 7 | 宋公明 | 10 |
+----+-----------+----+
7 rows in set (0.00 sec)

-- 表达式包含一个字段
SELECT id, name, english + 10 FROM exam_result;
+----+-----------+-------------+
| id | name | english + 10 |
+----+-----------+-------------+
| 1 | 唐三藏 | 66 |
| 2 | 孙悟空 | 87 |
| 3 | 猪悟能 | 100 |
| 4 | 曹孟德 | 77 |
| 5 | 刘玄德 | 55 |
| 6 | 孙权 | 88 |
| 7 | 宋公明 | 40 |
+----+-----------+-------------+
7 rows in set (0.00 sec)

-- 表达式包含多个字段
SELECT id, name, chinese + math + english FROM exam_result;
+----+-----------+-------------------------+
| id | name | chinese + math + english |
+----+-----------+-------------------------+
| 1 | 唐三藏 | 221 |
| 2 | 孙悟空 | 242 |
| 3 | 猪悟能 | 276 |
| 4 | 曹孟德 | 233 |
| 5 | 刘玄德 | 185 |
| 6 | 孙权 | 221 |
| 7 | 宋公明 | 170 |
+----+-----------+-------------------------+
7 rows in set (0.00 sec)
```

##### 1.2.1.4为查询结果添加别名

语法：  其中的as可省略

```mysql
SELECT column [AS] alias_name [...] FROM table_name;
```

```mysql
SELECT id, name, chinese + math + english 总分 FROM exam_result;
+----+-----------+--------+
| id | name | 总分 |
+----+-----------+--------+
| 1 | 唐三藏 | 221 |
| 2 | 孙悟空 | 242 |
| 3 | 猪悟能 | 276 |
| 4 | 曹孟德 | 233 |
| 5 | 刘玄德 | 185 |
| 6 | 孙权 | 221 |
| 7 | 宋公明 | 170 |
+----+-----------+--------+
7 rows in set (0.00 sec)
```

##### 1.2.1.5结果去重

```mysql
-- 98 分重复了
SELECT math FROM exam_result;
+--------+
| math |
+--------+
| 98 |
| 78 |
| 98 |
| 84 |
| 85 |
| 73 |
| 65 |
+--------+
7 rows in set (0.00 sec)
-- 去重结果
SELECT DISTINCT math FROM exam_result;
+--------+
| math |
+--------+
| 98 |
| 78 |
| 84 |
| 85 |
| 73 |
| 65 |
+--------+
6 rows in set (0.00 sec)
```

##### 1.2.1.6where条件

执行顺序：①from... ②where...③select...

**比较条件：**

| 比较运算符  | 含义与用法                                                   |
| ----------- | ------------------------------------------------------------ |
| =           | 等于。用于检查两个值是否相等。例如：`age = 30`   --->NULL不安全 |
| <> 或 !=    | 不等于。用于检查两个值是否不相等。例如：`age <> 30` 或 `age != 30` |
| >           | 大于。用于检查左侧的值是否大于右侧的值。例如：`age > 30`     |
| <           | 小于。用于检查左侧的值是否小于右侧的值。例如：`age < 30`     |
| >=          | 大于等于。用于检查左侧的值是否大于或等于右侧的值。例如：`age >= 30` |
| <=          | 小于等于。用于检查左侧的值是否小于或等于右侧的值。例如：`age <= 30` |
| BETWEEN     | 在指定的范围内（包括边界值）。用于检查某个值是否在指定范围内。例如：`age BETWEEN 18 AND 30` |
| LIKE        | 模糊匹配。用于在 WHERE 子句中进行模糊匹配。通常与通配符配合使用。例如：`name LIKE 'John%'`（匹配以 "John" 开头的名字），name LIKE 'John_'。 其中%可以匹配任意多个包括0，`_`严格匹配一个。 |
| IN          | 在指定的值列表中。用于检查某个值是否在给定的列表中。例如：`age IN (20, 30, 40)` |
| IS NULL     | 判断为 NULL。用于检查某个值是否为 NULL。例如：`age IS NULL`  |
| IS NOT NULL | 判断不为 NULL。用于检查某个值是否不为 NULL。例如：`age IS NOT NULL` |

**逻辑运算符：**

| 逻辑运算符 | 含义与用法                                                   |
| ---------- | ------------------------------------------------------------ |
| AND        | 逻辑 AND 运算符。用于将两个条件连接起来，要求两个条件都满足。例如：`condition1 AND condition2` |
| OR         | 逻辑 OR 运算符。用于将两个条件连接起来，要求其中一个条件满足即可。例如：`condition1 OR condition2` |
| NOT        | 逻辑 NOT 运算符。用于对条件进行取反。例如：`NOT condition` 或 `condition1 AND NOT condition2` |

例子：

**查询英语成绩不及格的人**

```mysql
mysql> select name, english from exam_result;
+-----------+---------+
| name      | english |
+-----------+---------+
| 唐三藏    |      56 |
| 孙悟空    |      77 |
| 猪悟能    |      90 |
| 曹孟德    |      67 |
| 刘玄德    |      45 |
| 孙权      |      78 |
| 宋公明    |      30 |
+-----------+---------+
7 rows in set (0.00 sec)

mysql> select name, english from exam_result where english < 60;
+-----------+---------+
| name      | english |
+-----------+---------+
| 唐三藏    |      56 |
| 刘玄德    |      45 |
| 宋公明    |      30 |
+-----------+---------+
3 rows in set (0.01 sec)
```

**语文成绩在 [80, 90] 分的同学及语文成绩**  

```mysql
-- 使用 AND 进行条件连接
SELECT name, chinese FROM exam_result WHERE chinese >= 80 AND chinese <= 90;
+-----------+-------+
| name | chinese |
+-----------+-------+
| 孙悟空 | 87 |
| 猪悟能 | 88 |
| 曹孟德 | 82 |
+-----------+-------+
3 rows in set (0.00 sec)
```

```mysql
-- 使用 BETWEEN ... AND ... 条件
SELECT name, chinese FROM exam_result WHERE chinese BETWEEN 80 AND 90;
+-----------+-------+
| name | chinese |
+-----------+-------+
| 孙悟空 | 87 |
| 猪悟能 | 88 |
| 曹孟德 | 82 |
+-----------+-------+
3 rows in set (0.00 sec)
```

**数学成绩是 58 或者 59 或者 98 或者 99 分的同学及数学成绩**  

```mysql
-- 使用 OR 进行条件连接
SELECT name, math FROM exam_result
WHERE math = 58
OR math = 59
OR math = 98
OR math = 99;
+-----------+--------+
| name | math |
+-----------+--------+
| 唐三藏 | 98 |
| 猪悟能 | 98 |
+-----------+--------+
2 rows in set (0.01 sec)
```

```mysql
-- 使用 IN 条件
SELECT name, math FROM exam_result WHERE math IN (58, 59, 98, 99);
+-----------+--------+
| name | math |
+-----------+--------+
| 唐三藏 | 98 |
| 猪悟能 | 98 |
+-----------+--------+
2 rows in set (0.00 sec)
```

**姓孙的同学 及 孙某同学**

```mysql
-- % 匹配任意多个（包括 0 个）任意字符
SELECT name FROM exam_result WHERE name LIKE '孙%';
+-----------+
| name |
+-----------+
| 孙悟空 |
| 孙权 |
+-----------+
2 rows in set (0.00 sec)
```

```mysql
-- _ 匹配严格的一个任意字符
SELECT name FROM exam_result WHERE name LIKE '孙_';
+--------+
| name |
+--------+
| 孙权 |
+--------+
1 row in set (0.00 sec)
```

**语文成绩好于英语成绩的同学**  

```mysql
-- WHERE 条件中比较运算符两侧都是字段
SELECT name, chinese, english FROM exam_result WHERE chinese > english;
+-----------+-------+--------+
| name | chinese | english |
+-----------+-------+--------+
| 唐三藏 | 67 | 56 |
| 孙悟空 | 87 | 77 |
| 曹孟德 | 82 | 67 |
| 刘玄德 | 55 | 45 |
| 宋公明 | 75 | 30 |
+-----------+-------+--------+
5 rows in set (0.00 sec)
```

**总分在 200 分以下的同学**  

```mysql
-- WHERE 条件中使用表达式
-- 别名不能用在 WHERE 条件中
SELECT name, chinese + math + english 总分 FROM exam_result
WHERE chinese + math + english < 200;
+-----------+--------+
| name | 总分 |
+-----------+--------+
| 刘玄德 | 185 |
| 宋公明 | 170 |
+-----------+--------+
2 rows in set (0.00 sec)
```

**语文成绩 > 80 并且不姓孙的同学**

```mysql
-- AND 与 NOT 的使用
SELECT name, chinese FROM exam_result
WHERE chinese > 80 AND name NOT LIKE '孙%';
+----+-----------+-------+--------+--------+
| id | name | chinese | math | english |
+----+-----------+-------+--------+--------+
| 3 | 猪悟能 | 88 | 98 | 90 |
| 4 | 曹孟德 | 82 | 84 | 67 |
+----+-----------+-------+--------+--------+
2 rows in set (0.00 sec)
```

  **孙某同学，否则要求总成绩 > 200 并且 语文成绩 < 数学成绩 并且 英语成绩 > 80**   

```mysql
-- 综合性查询
SELECT name, chinese, math, english, chinese + math + english 总分
FROM exam_result
WHERE name LIKE '孙_' OR (
chinese + math + english > 200 AND chinese < math AND english > 80
);
+-----------+-------+--------+--------+--------+
| name | chinese | math | english | 总分 |
+-----------+-------+--------+--------+--------+
| 猪悟能 | 88 | 98 | 90 | 276 |
| 孙权 | 70 | 73 | 78 | 221 |
+-----------+-------+--------+--------+--------+
2 rows in set (0.00 sec)
```

**NULL 的查询**  

```mysql
-- 查询 students 表
+-----+-------+-----------+-------+
| id | sn | name | qq |
+-----+-------+-----------+-------+
| 100 | 10010 | 唐大师 | NULL |
| 101 | 10001 | 孙悟空 | 11111 |
| 103 | 20002 | 孙仲谋 | NULL |
| 104 | 20001 | 曹阿瞒 | NULL |
+-----+-------+-----------+-------+
4 rows in set (0.00 sec)
-- 查询 qq 号已知的同学姓名
SELECT name, qq FROM students WHERE qq IS NOT NULL;
+-----------+-------+
| name | qq |
+-----------+-------+
| 孙悟空 | 11111 |
+-----------+-------+
1 row in set (0.00 sec)
-- NULL 和 NULL 的比较，= 和 <=> 的区别
SELECT NULL = NULL, NULL = 1, NULL = 0;
+-------------+----------+----------+
| NULL = NULL | NULL = 1 | NULL = 0 |
+-------------+----------+----------+
| NULL | NULL | NULL |
+-------------+----------+----------+
1 row in set (0.00 sec)
SELECT NULL <=> NULL, NULL <=> 1, NULL <=> 0;
+---------------+------------+------------+
| NULL <=> NULL | NULL <=> 1 | NULL <=> 0 |
+---------------+------------+------------+
| 1 | 0 | 0 |
+---------------+------------+------------+
1 row in set (0.00 sec)
```

