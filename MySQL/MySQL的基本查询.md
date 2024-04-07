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

### 1.2Retrieve

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

##### 1.2.1.7 结果排序

语法：

```mysql
-- ASC 为升序（从小到大）
-- DESC 为降序（从大到小）
-- 默认为 ASC
SELECT ... FROM table_name [WHERE ...]
ORDER BY column [ASC|DESC], [...];
```

**注意：没有 ORDER BY 子句的查询，返回的顺序是未定义的，永远不要依赖这个顺序**  

案例：

**同学及数学成绩，按数学成绩升序显示**  

```mysql
mysql> SELECT name, math FROM exam_result ORDER BY math;
+-----------+------+
| name      | math |
+-----------+------+
| 宋公明    |   65 |
| 孙权      |   73 |
| 孙悟空    |   78 |
| 曹孟德    |   84 |
| 刘玄德    |   85 |
| 唐三藏    |   98 |
| 猪悟能    |   98 |
+-----------+------+
7 rows in set (0.00 sec)
```

**同学及 qq 号，按 qq 号排序显示**  

```mysql
-- NULL 视为比任何值都小，升序出现在最上面
SELECT name, qq FROM students ORDER BY qq;
+-----------+-------+
| name | qq |
+-----------+-------+
| 唐大师 | NULL |
| 孙仲谋 | NULL |
| 曹阿瞒 | NULL |
| 孙悟空 | 11111 |
+-----------+-------+
4 rows in set (0.00 sec)
```

```mysql
-- NULL 视为比任何值都小，降序出现在最下面
SELECT name, qq FROM students ORDER BY qq DESC;
+-----------+-------+
| name | qq |
+-----------+-------+
| 孙悟空 | 11111 |
| 唐大师 | NULL |
| 孙仲谋 | NULL |
| 曹阿瞒 | NULL |
+-----------+-------+
4 rows in set (0.00 sec)
```

**查询同学各门成绩，依次按 数学降序，英语升序，语文升序的方式显示**  

```mysql
-- 多字段排序，排序优先级随书写顺序
SELECT name, math, english, chinese FROM exam_result
ORDER BY math DESC, english, chinese;
+-----------+--------+--------+-------+
| name | math | english | chinese |
+-----------+--------+--------+-------+
| 唐三藏 | 98 | 56 | 67 |
| 猪悟能 | 98 | 90 | 88 |
| 刘玄德 | 85 | 45 | 55 |
| 曹孟德 | 84 | 67 | 82 |
| 孙悟空 | 78 | 77 | 87 |
| 孙权 | 73 | 78 | 70 |
| 宋公明 | 65 | 30 | 75 |
+-----------+--------+--------+-------+
7 rows in set (0.00 sec)
```

**查询同学及总分，由高到低**  

```mysql
-- ORDER BY 中可以使用表达式
SELECT name, chinese + english + math FROM exam_result
ORDER BY chinese + english + math DESC;
+-----------+-------------------------+
| name | chinese + english + math |
+-----------+-------------------------+
| 猪悟能 | 276 |
| 孙悟空 | 242 |
| 曹孟德 | 233 |
| 唐三藏 | 221 |
| 孙权 | 221 |
| 刘玄德 | 185 |
| 宋公明 | 170 |
+-----------+-------------------------+
7 rows in set (0.00 sec)
```

```mysql
-- ORDER BY 子句中可以使用列别名
-- 原因是order by是相较于select之后执行，所以它可以识别别名
SELECT name, chinese + english + math 总分 FROM exam_result
ORDER BY 总分 DESC;
+-----------+--------+
| name | 总分 |
+-----------+--------+
| 猪悟能 | 276 |
| 孙悟空 | 242 |
| 曹孟德 | 233 |
| 唐三藏 | 221 |
| 孙权 | 221 |
| 刘玄德 | 185 |
| 宋公明 | 170 |
+-----------+--------+
7 rows in set (0.00 sec)
```

**查询姓孙的同学或者姓曹的同学数学成绩，结果按数学成绩由高到低显示**  

```mysql
-- 结合 WHERE 子句 和 ORDER BY 子句
SELECT name, math FROM exam_result
WHERE name LIKE '孙%' OR name LIKE '曹%'
ORDER BY math DESC;
+-----------+--------+
| name | math |
+-----------+--------+
| 曹孟德 | 84 |
| 孙悟空 | 78 |
| 孙权 | 73 |
+-----------+--------+
3 rows in set (0.00 sec)
```

**筛选分页结果**  

语法：

```mysql
-- 起始下标为 0
-- 从 s 开始，筛选 n 条结果
SELECT ... FROM table_name [WHERE ...] [ORDER BY ...] LIMIT s, n
-- 从 0 开始，筛选 n 条结果
SELECT ... FROM table_name [WHERE ...] [ORDER BY ...] LIMIT n;
;
-- 从 s 开始，筛选 n 条结果，比第二种用法更明确，建议使用
SELECT ... FROM table_name [WHERE ...] [ORDER BY ...] LIMIT n OFFSET s;
```

建议：对未知表进行查询时，最好加一条 LIMIT 1，避免因为表中数据过大，查询全表数据导致数据库卡死
**按 id 进行分页，每页 3 条记录，分别显示 第 1、2、3 页**   

```mysql
-- 第 1 页
SELECT id, name, math, english, chinese FROM exam_result
ORDER BY id LIMIT 3 OFFSET 0;
+----+-----------+--------+--------+-------+
| id | name | math | english | chinese |
+----+-----------+--------+--------+-------+
| 1 | 唐三藏 | 98 | 56 | 67 |
| 2 | 孙悟空 | 78 | 77 | 87 |
| 3 | 猪悟能 | 98 | 90 | 88 |
+----+-----------+--------+--------+-------+
3 rows in set (0.02 sec)
```

```mysql
-- 第 2 页
SELECT id, name, math, english, chinese FROM exam_result
ORDER BY id LIMIT 3 OFFSET 3;
+----+-----------+--------+--------+-------+
| id | name | math | english | chinese |
+----+-----------+--------+--------+-------+
| 4 | 曹孟德 | 84 | 67 | 82 |
| 5 | 刘玄德 | 85 | 45 | 55 |
| 6 | 孙权 | 73 | 78 | 70 |
+----+-----------+--------+--------+-------+
3 rows in set (0.00 sec)
```

```mysql
-- 第 3 页，如果结果不足 3 个，不会有影响
SELECT id, name, math, english, chinese FROM exam_result
ORDER BY id LIMIT 3 OFFSET 6;
+----+-----------+--------+--------+-------+
| id | name | math | english | chinese |
+----+-----------+--------+--------+-------+
| 7 | 宋公明 | 65 | 30 | 75 |
+----+-----------+--------+--------+-------+
1 row in set (0.00 sec)
```

### 1.3 Update  

语法：

```mysql
UPDATE table_name SET column = expr [, column = expr ...]
[WHERE ...] [ORDER BY ...] [LIMIT ...]
```

**对查询到的结果进行列值更新**  

案例：  

**将孙悟空同学的数学成绩变更为 80 分**  

```cpp
-- 更新值为具体值
-- 查看原数据
SELECT name, math FROM exam_result WHERE name = '孙悟空';
+-----------+--------+
| name | math |
+-----------+--------+
| 孙悟空 | 78 |
+-----------+--------+
1 row in set (0.00 sec)
-- 数据更新
UPDATE exam_result SET math = 80 WHERE name = '孙悟空';
Query OK, 1 row affected (0.04 sec)
    Rows matched: 1 Changed: 1 Warnings: 0
-- 查看更新后数据
SELECT name, math FROM exam_result WHERE name = '孙悟空';
+-----------+--------+
| name | math |
+-----------+--------+
| 孙悟空 | 80 |
+-----------+--------+
1 row in set (0.00 sec)
```

**将曹孟德同学的数学成绩变更为 60 分，语文成绩变更为 70 分**  

```mysql
-- 一次更新多个列
| nam
-- 查看原数据
SELECT name, math, chinese FROM exam_result WHERE name = '曹孟德';
+-----------+--------+-------+
e | math | chinese |
+-----------+--------+-------+
| 曹孟德 | 84 | 82 |
+-----------+--------+-------+
1 row in set (0.00 sec)
-- 数据更新
UPDATE exam_result SET math = 60, chinese = 70 WHERE name = '曹孟德';
Query OK, 1 row affected (0.14 sec)
Rows matched: 1 Changed: 1 Warnings: 0
-- 查看更新后数据
SELECT name, math, chinese FROM exam_result WHERE name = '曹孟德';
+-----------+--------+-------+
| name | math | chinese |
+-----------+--------+-------+
| 曹孟德 | 60 | 70 |
+-----------+--------+-------+
1 row in set (0.00 sec)
```

**将总成绩倒数前三的 3 位同学的数学成绩加上 30 分**  

```mysql
-- 更新值为原值基础上变更
-- 查看原数据
-- 别名可以在ORDER BY中使用
SELECT name, math, chinese + math + english 总分 FROM exam_result
ORDER BY 总分 LIMIT 3;
+-----------+--------+--------+
| name | math | 总分 |
+-----------+--------+--------+
| 宋公明 | 65 | 170 |
| 刘玄德 | 85 | 185 |
| 曹孟德 | 60 | 197 |
+-----------+--------+--------+
3 rows in set (0.00 sec)
-- 数据更新，不支持 math += 30 这种语法
UPDATE exam_result SET math = math + 30
ORDER BY chinese + math + english LIMIT 3;
-- 查看更新后数据
-- 思考：这里还可以按总分升序排序取前 3 个么？
SELECT name, math, chinese + math + english 总分 FROM exam_result
WHERE name IN ('宋公明', '刘玄德', '曹孟德');
+-----------+--------+--------+
| name | math | 总分 |
+-----------+--------+--------+
| 曹孟德 | 90 | 227 |
| 刘玄德 | 115 | 215 |
| 宋公明 | 95 | 200 |
+-----------+--------+--------+
3 rows in set (0.00 sec)
-- 按总成绩排序后查询结果
SELECT name, math, chinese + math + english 总分 FROM exam_result
ORDER BY 总分 LIMIT 3;
+-----------+--------+--------+
| name | math | 总分 |
+-----------+--------+--------+
| 宋公明 | 95 | 200 |
| 刘玄德 | 115 | 215 |
| 唐三藏 | 98 | 221 |
+-----------+--------+--------+
3 rows in set (0.00 sec)
```

**将所有同学的语文成绩更新为原来的 2 倍**  

**注意：更新全表的语句慎用！**  

```mysql
-- 没有 WHERE 子句，则更新全表
-- 查看原数据
SELECT * FROM exam_result;
+----+-----------+-------+--------+--------+
| id | name | chinese | math | english |
+----+-----------+-------+--------+--------+
| 1 | 唐三藏 | 67 | 98 | 56 |
| 2 | 孙悟空 | 87 | 80 | 77 |
| 3 | 猪悟能 | 88 | 98 | 90 |
| 4 | 曹孟德 | 70 | 90 | 67 |
| 5 | 刘玄德 | 55 | 115 | 45 |
| 6 | 孙权 | 70 | 73 | 78 |
| 7 | 宋公明 | 75 | 95 | 30 |
+----+-----------+-------+--------+--------+
7 rows in set (0.00 sec)
-- 数据更新
UPDATE exam_result SET chinese = chinese * 2;
Query OK, 7 rows affected (0.00 sec)
Rows matched: 7 Changed: 7 Warnings: 0
-- 查看更新后数据
SELECT * FROM exam_result;
+----+-----------+-------+--------+--------+
| id | name | chinese | math | english |
+----+-----------+-------+--------+--------+
| 1 | 唐三藏 | 134 | 98 | 56 |
| 2 | 孙悟空 | 174 | 80 | 77 |
| 3 | 猪悟能 | 176 | 98 | 90 |
| 4 | 曹孟德 | 140 | 90 | 67 |
| 5 | 刘玄德 | 110 | 115 | 45 |
| 6 | 孙权 | 140 | 73 | 78 |
| 7 | 宋公明 | 150 | 95 | 30 |
+----+-----------+-------+--------+--------+
7 rows in set (0.00 sec)
```

### 1.4 Delete  

#### 1.4.1删除数据

语法：

```mysql
DELETE FROM table_name [WHERE ...] [ORDER BY ...] [LIMIT ...]
```

例子：

**删除孙悟空同学的考试成绩**  

```mysql
-- 查看原数据
SELECT * FROM exam_result WHERE name = '孙悟空';
+----+-----------+-------+--------+--------+
| id | name | chinese | math | english |
+----+-----------+-------+--------+--------+
| 2 | 孙悟空 | 174 | 80 | 77 |
+----+-----------+-------+--------+--------+
1 row in set (0.00 sec)
-- 删除数据
DELETE FROM exam_result WHERE name = '孙悟空';
Query OK, 1 row affected (0.17 sec)
-- 查看删除结果
SELECT * FROM exam_result WHERE name = '孙悟空';
Empty set (0.00 sec)
```

**删除整张表数据**  

**注意：删除整表操作要慎用！**  

```mysql
-- 准备测试表
CREATE TABLE for_delete (
id INT PRIMARY KEY AUTO_INCREMENT,
name VARCHAR(20)
);
Query OK, 0 rows affected (0.16 sec)
-- 插入测试数据
INSERT INTO for_delete (name) VALUES ('A'), ('B'), ('C');
Query OK, 3 rows affected (1.05 sec)
Records: 3 Duplicates: 0 Warnings: 0
-- 查看测试数据
SELECT * FROM for_delete;
+----+------+
| id | name |
+----+------+
| 1 | A |
| 2 | B |
| 3 | C |
+----+------+
3 rows in set (0.00 sec)
```

```mysql
-- 删除整表数据
DELETE FROM for_delete;
Query OK, 3 rows affected (0.00 sec)
-- 查看删除结果
SELECT * FROM for_delete;
Empty set (0.00 sec)
```

```mysql
-- 再插入一条数据，自增 id 在原值上增长
INSERT INTO for_delete (name) VALUES ('D');
Query OK, 1 row affected (0.00 sec)
-- 查看数据
SELECT * FROM for_delete;
+----+------+
| id | name |
+----+------+
| 4 | D |
+----+------+
1 row in set (0.00 sec)
-- 查看表结构，会有 AUTO_INCREMENT=n 项
SHOW CREATE TABLE for_delete\G
*************************** 1. row ***************************
Table: for_delete
Create Table: CREATE TABLE `for_delete` (
`id` int(11) NOT NULL AUTO_INCREMENT,
`name` varchar(20) DEFAULT NULL
PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8
1 row in set (0.00 sec)
```

#### 1.4.2截断表

语法：

```mysql
TRUNCATE [TABLE] table_name
```

**注意：这个操作慎用**  

- 只能对整表操作，不能像 DELETE 一样针对部分数据操作；
- 实际上 TRUNCATE 不对数据操作，所以比 DELETE 更快，但是TRUNCATE在删除数据的时候，并不经过真正的事
  物，所以无法回滚
- 会重置 AUTO_INCREMENT 项  

```mysql
-- 准备测试表
CREATE TABLE for_truncate (
id INT PRIMARY KEY AUTO_INCREMENT,
name VARCHAR(20)
);
Query OK, 0 rows affected (0.16 sec)
-- 插入测试数据
INSERT INTO for_truncate (name) VALUES ('A'), ('B'), ('C');
Query OK, 3 rows affected (1.05 sec)
Records: 3 Duplicates: 0 Warnings: 0
-- 查看测试数据
SELECT * FROM for_truncate;
+----+------+
| id | name |
+----+------+
| 1 | A |
| 2 | B |
| 3 | C |
+----+------+
3 rows in set (0.00 sec)
```

```mysql
-- 截断整表数据，注意影响行数是 0，所以实际上没有对数据真正操作
TRUNCATE for_truncate;
Query OK, 0 rows affected (0.10 sec)
-- 查看删除结果
SELECT * FROM for_truncate;
Empty set (0.00 sec)
```

```mysql
-- 再插入一条数据，自增 id 在重新增长
INSERT INTO for_truncate (name) VALUES ('D');
Query OK, 1 row affected (0.00 sec)
-- 查看数据
SELECT * FROM for_truncate;
+----+------+
| id | name |
+----+------+
| 1 | D |
+----+------+
1 row in set (0.00 sec)
-- 查看表结构，会有 AUTO_INCREMENT=2 项
SHOW CREATE TABLE for_truncate\G
*************************** 1. row ***************************
Table: for_truncate
Create Table: CREATE TABLE `for_truncate` (
`id` int(11) NOT NULL AUTO_INCREMENT,
`name` varchar(20) DEFAULT NULL,
PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8
1 row in set (0.00 sec)
```

MySQL 中的 DELETE 和 TRUNCATE 是用于删除数据的两种不同的命令，它们之间有一些重要的区别：

1. **DELETE**：
   - DELETE 语句用于从表中删除指定条件的行。
   - DELETE 语句是 DML（数据操作语言）的一部分，它影响表中的行，同时可以与 WHERE 子句一起使用来指定要删除的行。
   - DELETE 语句会触发表的触发器（如果有的话）。
   - DELETE 语句在执行时，会将每一行都放入事务日志（或称为回滚日志）中，以便在需要时进行回滚操作。
2. **TRUNCATE**：
   - TRUNCATE 语句用于从表中删除所有行，并且不激活触发器。
   - TRUNCATE 语句是 DDL（数据定义语言）的一部分，它是一个快速且有效的方法来清空表中的数据。
   - TRUNCATE 语句比 DELETE 语句更快，因为它不会逐行删除数据，而是直接删除整个表，然后重新创建一个空表。
   - TRUNCATE 语句在执行时会重置表的自增计数器（如果有的话）。

总的来说，DELETE 和 TRUNCATE 都可以删除表中的数据，但它们的用途和效果略有不同。DELETE 更适用于根据条件删除部分数据，而 TRUNCATE 则更适用于清空整个表的数据。同时，TRUNCATE 比 DELETE 执行速度更快，但不会触发触发器，并且无法恢复删除的数据。

### 1.5插入查询结果  -- 对表去重

语法：  

```mysql
INSERT INTO table_name [(column [, column ...])] SELECT ...
```

案例：删除表中的的重复复记录，重复的数据只能有一份  

```mysql
-- 创建原数据表
CREATE TABLE duplicate_table (id int, name varchar(20));
Query OK, 0 rows affected (0.01 sec)
-- 插入测试数据
INSERT INTO duplicate_table VALUES
(100, 'aaa'),
(100, 'aaa'),
(200, 'bbb'),
(200, 'bbb'),
(200, 'bbb'),
(300, 'ccc');
Query OK, 6 rows affected (0.00 sec)
Records: 6 Duplicates: 0 Warnings: 0
```

思路：  

```mysql
-- 创建一张空表 no_duplicate_table，结构和 duplicate_table 一样
CREATE TABLE no_duplicate_table LIKE duplicate_table;
Query OK, 0 rows affected (0.00 sec)
-- 将 duplicate_table 的去重数据插入到 no_duplicate_table
INSERT INTO no_duplicate_table SELECT DISTINCT * FROM duplicate_table;
Query OK, 3 rows affected (0.00 sec)
Records: 3 Duplicates: 0 Warnings: 0
-- 通过重命名表，实现原子的去重操作
RENAME TABLE duplicate_table TO old_duplicate_table,
no_duplicate_table TO duplicate_table;
Query OK, 0 rows affected (0.00 sec)
-- 查看最终结果
SELECT * FROM duplicate_table;
+------+------+
| id | name |
+------+------+
| 100 | aaa |
| 200 | bbb |
| 300 | ccc |
+------+------+
3 rows in set (0.00 sec)
```

### 1.6聚合函数

| 聚合函数             | 说明                                                     |
| -------------------- | -------------------------------------------------------- |
| COUNT(column)        | 计算指定列非 NULL 值的行数，或计算所有行数（COUNT(*)）。 |
| SUM(column)          | 计算指定列中所有值的总和，忽略 NULL 值。                 |
| AVG(column)          | 计算指定列中所有值的平均值，忽略 NULL 值。               |
| MAX(column)          | 获取指定列中的最大值，忽略 NULL 值。                     |
| MIN(column)          | 获取指定列中的最小值，忽略 NULL 值。                     |
| GROUP_CONCAT(column) | 将指定列的值连接成一个字符串，并返回结果。               |

案例：  

**统计班级共有多少同学**  

```mysql
-- 使用 * 做统计，不受 NULL 影响
SELECT COUNT(*) FROM students;
+----------+
| COUNT(*) |
+----------+
| 4 |
+----------+
1 row in set (0.00 sec)
-- 使用表达式做统计
SELECT COUNT(1) FROM students;
+----------+
| COUNT(1) |
+----------+
| 4 |
+----------+
1 row in set (0.00 sec)
```

**统计班级收集的 qq 号有多少**  

```mysql
-- NULL 不会计入结果
SELECT COUNT(qq) FROM students;
+-----------+
| COUNT(qq) |
+-----------+
| 1 |
+-----------+
1 row in set (0.00 sec)
```

**统计本次考试的数学成绩分数个数**  

```mysql
-- COUNT(math) 统计的是全部成绩
SELECT COUNT(math) FROM exam_result;
+---------------+
| COUNT(math) |
+---------------+
| 6 |
+---------------+
1 row in set (0.00 sec)
-- COUNT(DISTINCT math) 统计的是去重成绩数量
SELECT COUNT(DISTINCT math) FROM exam_result;
+------------------------+
| COUNT(DISTINCT math) |
+------------------------+
| 5 |
+------------------------+
1 row in set (0.00 sec)
```

**统计数学成绩总分**  

```mysql
SELECT SUM(math) FROM exam_result;
+-------------+
| SUM(math) |
+-------------+
| 569 |
+-------------+
1 row in set (0.00 sec)
-- 不及格 < 60 的总分，没有结果，返回 NULL
SELECT SUM(math) FROM exam_result WHERE math < 60;
+-------------+
| SUM(math) |
+-------------+
| NULL |
+-------------+
1 row in set (0.00 sec)
```

**统计平均总分**  

```mysql
SELECT AVG(chinese + math + english) 平均总分 FROM exam_result;
+--------------+
| 平均总分 |
+--------------+
| 297.5 |
+--------------+
```

**返回英语最高分**  

```mysql
SELECT MAX(english) FROM exam_result;
+-------------+
| MAX(english) |
+-------------+
| 90 |
+-------------+
1 row in set (0.00 sec)
```

**返回 > 70 分以上的数学最低分**  

```mysql
SELECT MIN(math) FROM exam_result WHERE math > 70;
+-------------+
| MIN(math) |
+-------------+
| 73 |
+-------------+
1 row in set (0.00 sec)
```

### 1.7分组聚合统计

**在select中使用group by 子句可以对指定列进行分组查询**  

```mysql
select column1, column2, .. from table group by column;
```

案例：  

- 准备工作，创建一个雇员信息表（来自oracle 9i的经典测试表）

  > EMP员工表
  > DEPT部门表
  > SALGRADE工资等级表

- 如何显示每个部门的平均工资和最高工资  

```mysql
select deptno,avg(sal),max(sal) from EMP group by deptno;
```

- 显示每个部门的每种岗位的平均工资和最低工资  

```mysql
select avg(sal),min(sal),job, deptno from EMP group by deptno, job;
```

- 显示平均工资低于2000的部门和它的平均工资  

  ①统计各个部门的平均工资  

```mysql
select avg(sal) from EMP group by deptno
```

​       ②having和group by配合使用，对group by结果进行过滤  

```mysql
select avg(sal) as myavg from EMP group by deptno having myavg<2000;
--having经常和group by搭配使用，作用是对分组进行筛选，作用有些像where
```

#### 1.7.1where和having的区别

在MySQL中，`WHERE` 和 `HAVING` 关键字都用于过滤数据，但它们之间有一些关键的区别：

1. **用途**：
   - `WHERE`：用于在对表中的记录进行选择时指定条件。它在**查询执行之前**过滤数据，只能用于对行进行过滤。
   - `HAVING`：用于在对分组后的结果集进行过滤时指定条件。它在对结果集**进行分组之后**进行过滤，用于对分组后的结果集进行条件过滤。

2. **作用对象**：
   - `WHERE`：作用于原始数据行，在数据被分组前过滤行。
   - `HAVING`：作用于分组后的结果集，在数据被分组后过滤分组。

3. **使用场景**：
   - 当你想要在数据被分组前进行过滤时，使用 `WHERE`。
   - 当你想要在数据被分组后进行过滤时，使用 `HAVING`。

4. **性能影响**：
   - 由于 `WHERE` 是在数据被分组前过滤行，因此它可以利用索引来提高性能。
   - `HAVING` 在数据分组后进行过滤，因此它可能需要更多的系统资源，并且可能会导致性能下降，特别是在处理大量数据时。

总的来说，`WHERE` 用于过滤行，而 `HAVING` 用于过滤分组后的结果集。在大多数情况下，你应该在可能的情况下首选 `WHERE` 条件来过滤数据，以提高查询性能。只有在需要对分组后的数据进行过滤时才使用 `HAVING`。

### 1.8执行顺序

> 在 SQL 的 SELECT 语句中，各个部分的执行顺序通常如下：
>
> 1. **FROM**：指定要查询的数据表或视图，它是 SQL 查询的起点。在 FROM 子句中指定的表将被用来执行后续的操作。
>
> 2. **JOIN**：如果查询涉及到多个表，那么 JOIN 子句将被用来将这些表连接在一起。JOIN 子句可以包括 INNER JOIN、LEFT JOIN、RIGHT JOIN、FULL JOIN 等不同类型的连接，用来根据某些条件将多个表中的数据组合在一起。
>
> 3. **WHERE**：在 WHERE 子句中指定查询条件，用来过滤从 FROM 子句中获取的数据。只有满足 WHERE 子句中指定的条件的数据行才会被包含在查询结果中。
>
> 4. **GROUP BY**：如果查询中包含了聚合函数（如 COUNT、SUM、AVG 等），那么 GROUP BY 子句将会按照指定的列对结果集进行分组。这样，每个分组将会有一个汇总值。注意，如果使用了 GROUP BY 子句，那么 SELECT 中除了聚合函数外的列，都需要在 GROUP BY 子句中出现。
>
> 5. **HAVING**：HAVING 子句类似于 WHERE 子句，但它是用来过滤 GROUP BY 子句生成的分组的，而不是用来过滤行。只有满足 HAVING 子句中指定的条件的分组才会被包含在查询结果中。
>
> 6. **SELECT**：SELECT 子句用来指定查询要返回的列。在 SELECT 子句中，可以包含列名、表达式、常量和函数等。查询的结果将由 SELECT 子句指定的内容组成。
>
> 7. **ORDER BY**：ORDER BY 子句用来对查询结果进行排序。可以指定一个或多个列，并且可以指定升序（ASC）或降序（DESC）排序。
>
> 8. **LIMIT/OFFSET**：LIMIT 和 OFFSET 子句用来限制返回的行数，并可以指定从结果集中的哪一行开始返回。这两个子句通常在最后执行。
>
> 总的来说，查询语句的执行顺序是从上到下，依次执行各个子句。每个子句的执行结果都会作为下一个子句的输入。

<u>**SQL查询中各个关键字的执行先后顺序 from > on> join > where > group by > with > having > select  \> distinct > order by > limit ；**</u>
