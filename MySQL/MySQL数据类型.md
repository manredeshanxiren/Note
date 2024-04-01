# MySQL数据类型

## 1.数据类型的分类

> | 类别           | 类型名称     | 说明                                                         |
> | -------------- | ------------ | ------------------------------------------------------------ |
> | 数值类型       | TINYINT      | 很小的整数                                                   |
> |                | SMALLINT     | 小的整数                                                     |
> |                | MEDIUMINT    | 中等大小的整数                                               |
> |                | INT或INTEGER | 标准的整数                                                   |
> |                | BIGINT       | 较大的整数                                                   |
> |                | FLOAT        | 单精度浮点数                                                 |
> |                | DOUBLE       | 双精度浮点数                                                 |
> |                | DECIMAL      | 定点数，适用于存储精确的小数                                 |
> | 日期和时间类型 | DATE         | 日期值（YYYY-MM-DD）                                         |
> |                | TIME         | 时间值或持续时间（HH:MM:SS）                                 |
> |                | DATETIME     | 日期和时间的组合（YYYY-MM-DD HH:MM:SS）                      |
> |                | TIMESTAMP    | 时间戳，表示自1970年1月1日午夜UTC以来的秒数，用于跟踪更改    |
> |                | YEAR         | 年份值                                                       |
> | 字符串类型     | CHAR         | 定长字符串                                                   |
> |                | VARCHAR      | 变长字符串                                                   |
> |                | BINARY       | 定长二进制字符串                                             |
> |                | VARBINARY    | 变长二进制字符串                                             |
> |                | BLOB         | 二进制大对象，用于存储大量的二进制数据                       |
> |                | TEXT         | 大对象，用于存储大量的非二进制数据                           |
> |                | ENUM         | 枚举，字符串对象的特殊类型，只能有限的几个值中选择一个       |
> |                | SET          | 集合，字符串对象的一个特殊类型，可以从零个或多个值中选择（最多64个） |

数值类型

> | 类型名称     | 字节 | 有符号最小值               | 有符号最大值              | 无符号最小值           | 无符号最大值               |
> | ------------ | ---- | -------------------------- | ------------------------- | ---------------------- | -------------------------- |
> | TINYINT      | 1    | -128                       | 127                       | 0                      | 255                        |
> | SMALLINT     | 2    | -32,768                    | 32,767                    | 0                      | 65,535                     |
> | MEDIUMINT    | 3    | -8,388,608                 | 8,388,607                 | 0                      | 16,777,215                 |
> | INT或INTEGER | 4    | -2,147,483,648             | 2,147,483,647             | 0                      | 4,294,967,295              |
> | BIGINT       | 8    | -9,223,372,036,854,775,808 | 9,223,372,036,854,775,807 | 0                      | 18,446,744,073,709,551,615 |
> | FLOAT        | 4    | -3.402823466E+38           | 3.402823466E+38           | N/A                    | N/A                        |
> | DOUBLE       | 8    | -1.7976931348623157E+308   | 1.7976931348623157E+308   | N/A                    | N/A                        |
> | DECIMAL      | 变动 | 取决于定义的精度和规模     | 取决于定义的精度和规模    | 取决于定义的精度和规模 | 取决于定义的精度和规模     |
>
> 一般默认为有符号。

### 1.1整形类型

#### 1.1.1tinyint的例子

> 如果我们向mysql中插入不合法的数据，MySQL一般都是直接拦截我们，不让我们做对应的操作。
>
> 反过来，我们如果已经有数据成功的插入到了mysql中了，一定插入的是合法的数据。
>
> 所以，mysql，一般而言，数据类型本身也是一种“约束”！

```mysql
mysql> create database test_db;
Query OK, 1 row affected (0.00 sec)

mysql> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| RECOVER_YOUR_DATA  |
| mysql              |
| performance_schema |
| sys                |
| test_db            |
+--------------------+
6 rows in set (0.00 sec)

mysql> use test_db;
Database changed
mysql> create table if not exists t1(
    -> num tinyint
    -> );
Query OK, 0 rows affected (0.01 sec)
mysql> desc t1;
+-------+------------+------+-----+---------+-------+
| Field | Type       | Null | Key | Default | Extra |
+-------+------------+------+-----+---------+-------+
| num   | tinyint(4) | YES  |     | NULL    |       |
+-------+------------+------+-----+---------+-------+
1 row in set (0.01 sec)


//尝试插入数据
mysql> insert into t1 values (1);
Query OK, 1 row affected (0.01 sec)

mysql> insert into t1 values (-1);
Query OK, 1 row affected (0.00 sec)

mysql> insert into t1 values (-128);
Query OK, 1 row affected (0.01 sec)

mysql> insert into t1 values (127);
Query OK, 1 row affected (0.00 sec)

mysql> select * from t1;
+------+
| num  |
+------+
|    1 |
|   -1 |
| -128 |
|  127 |
+------+
4 rows in set (0.00 sec)

mysql> insert into t1 values (128);
ERROR 1264 (22003): Out of range value for column 'num' at row 1
mysql> insert into t1 values (-200);
ERROR 1264 (22003): Out of range value for column 'num' at row 1

```

#### 1.1.2bit类型

> 在 MySQL 中声明 `BIT` 类型的基本语法如下所示：
>
> ```sql
> column_name BIT[(length)]
> ```
>
> 其中：
> - `column_name` 是列的名称。
> - `BIT` 是数据类型关键字。
> - `length` 是可选的参数，指定了位数。如果未指定长度，则默认为1。
>
> 下面是一些例子：
>
> 1. 创建一个存储单个位的列：
> ```sql
> bit_column BIT
> ```
>
> 2. 创建一个存储8位的列：
> ```sql
> bit_column BIT(8)
> ```
>
> 3. 创建一个表，其中包含一个存储布尔值的列：
> ```sql
> CREATE TABLE example_table (
>     id INT PRIMARY KEY,
>     is_active BIT(1)
> );
> ```
>
> 在这个例子中，`is_active` 列将存储布尔值（0或1），因此长度为1位。
>
> 除了以上的基本语法外，还可以使用 `DEFAULT` 关键字来为 `BIT` 类型的列设置默认值，以及使用其他列或表达式来计算该列的值。

```mysql
mysql> create table if not exists t3;
ERROR 1113 (42000): A table must have at least 1 column
mysql> create table if not exists t3(
    -> id int,
    -> online bit
    -> );
Query OK, 0 rows affected (0.01 sec)

mysql> desc t3;
+--------+---------+------+-----+---------+-------+
| Field  | Type    | Null | Key | Default | Extra |
+--------+---------+------+-----+---------+-------+
| id     | int(11) | YES  |     | NULL    |       |
| online | bit(1)  | YES  |     | NULL    |       |
+--------+---------+------+-----+---------+-------+
2 rows in set (0.00 sec)

mysql> insert into t2 (id, online) values (123, 0);
ERROR 1054 (42S22): Unknown column 'id' in 'field list'
mysql> insert into t3 (id, online) values (123, 0);
Query OK, 1 row affected (0.00 sec)

mysql> insert into t3 (id, online) values (124, 1);
Query OK, 1 row affected (0.00 sec)

mysql> insert into t3 (id, online) values (125, 3);
ERROR 1406 (22001): Data too long for column 'online' at row 1
mysql> insert into t3 (id, online) values (125, 5);
ERROR 1406 (22001): Data too long for column 'online' at row 1

mysql> select * from t3;
+------+--------+
| id   | online |
+------+--------+
|  123 |        |
|  124 |       |
+------+--------+
2 rows in set (0.00 sec)

mysql> select id, hex(online) from t3;
+------+-------------+
| id   | hex(online) |
+------+-------------+
|  123 | 0           |
|  124 | 1           |
+------+-------------+
2 rows in set (0.00 sec)

```

**bit类型显示问题：**

> 在 MySQL 中，`BIT` 类型的数据是以二进制形式存储的，因此在查询时可能会以一种不太直观的方式显示出来。在使用 `SELECT` 查询时，`BIT` 类型的值可能会以二进制字符串或十进制整数形式返回，这取决于客户端工具或驱动程序的实现。
>
> 以下是一些可能的情况以及对应的理解方式：
>
> 1. **以二进制字符串形式显示**：有些 MySQL 客户端工具或驱动程序可能会以二进制字符串的形式返回 `BIT` 类型的值。例如，`b'0'` 表示一个位为0的值，而 `b'1'` 表示一个位为1的值。在这种情况下，返回的值可能不太直观，但你可以根据字符串的内容来理解它。
>
> 2. **以十进制整数形式显示**：另一种常见的情况是，`BIT` 类型的值以十进制整数的形式返回。例如，`0` 表示一个位为0的值，而 `1` 表示一个位为1的值。虽然这种表示方式比较直观，但你仍然需要知道返回的整数对应的二进制位是什么。
>
> 无论是哪种表示方式，你都可以根据返回的值来理解 `BIT` 类型的内容。如果你需要更直观的展示方式，可以在查询时使用 MySQL 提供的一些位操作函数来处理 `BIT` 类型的值，或者在应用程序中对查询结果进行进一步处理和格式化。

将bit类型转换成其他的更直观的类型而显示出来：

> 使用 MySQL 中的一些位操作函数，你可以将 `BIT` 类型的值转换为更直观的展示形式，比如转换为字符串表示。以下是一些常用的位操作函数及其用法：
>
> 1. `BIN()`: 将一个整数转换为二进制字符串。
>    
>    示例：
>    ```sql
>    SELECT BIN(bit_column) FROM table_name;
>    ```
>
>    这将返回 `bit_column` 中每个值的二进制表示形式。
>
> 2. `CONV()`: 将一个数字从一种数值系统转换为另一种数值系统。
>
>    示例：
>    ```sql
>    SELECT CONV(bit_column, 2, 10) FROM table_name;
>    ```
>
>    这将返回 `bit_column` 中每个值的十进制表示形式。
>
> 3. 使用 `CASE` 表达式和字符串连接函数，比如 `CONCAT()`，来创建自定义的展示形式。
>
>    示例：
>    ```sql
>    SELECT
>        CASE bit_column
>            WHEN b'0' THEN 'No'
>            WHEN b'1' THEN 'Yes'
>            ELSE 'Unknown'
>        END AS bit_value
>    FROM table_name;
>    ```
>
>    这将返回一个描述 `bit_column` 中每个值的字符串，"No" 表示0，"Yes" 表示1，"Unknown" 表示其他值。
>
> 你可以根据自己的需要选择合适的方法来处理 `BIT` 类型的值，使其更容易理解。

### 1.2浮点类型

#### 1.2.1float类型

> MySQL中的FLOAT数据类型用于存储浮点数值，它具有一定的语法和使用注意事项。
>
> ### FLOAT数据类型的语法
> FLOAT数据类型的语法如下：
> ```sql
> column_name FLOAT(size, d);
> ```
> 其中，`column_name`是列的名称，`size`是可选参数，用于指定显示宽度，表示数字总共占用的位数，包括小数点和符号位。`d`也是可选参数，用于指定小数点后的位数，默认为0。
>
> ### 使用注意事项
> 在使用FLOAT数据类型时，需要注意以下几点：
> 1. **精度问题：** FLOAT类型在存储浮点数时可能存在精度损失，这是由于浮点数的特性决定的。对于对精度要求比较高的场景，建议使用DECIMAL或DOUBLE类型。
> 2. **存储空间：** FLOAT类型占用的存储空间取决于size参数的设置。一般来说，FLOAT(24)占用4个字节，FLOAT(53)占用8个字节。
> 3. **小数位数：** 在定义FLOAT类型时，可以通过d参数指定小数点后的位数，但要注意小数位数不能超过总位数，否则可能导致数据截断。
> 4. **默认值：** 可以为FLOAT类型指定默认值，例如`DEFAULT 0.0`表示默认值为0.0。
> 5. **范围限制：** FLOAT类型的取值范围是-3.402823466E+38到-1.175494351E-38、0，和1.175494351E-38到3.402823466E+38。
>
> 综上所述，FLOAT数据类型适合存储对精度要求不高、取值范围较大的浮点数。如果需要更高精度或更大范围的数值，建议考虑使用DECIMAL或DOUBLE类型。

```mysql
mysql> create table if not exists t1(
    -> id int,
    -> salary float
    -> );
Query OK, 0 rows affected (0.01 sec)
mysql> show tables;
+-------------------+
| Tables_in_test_db |
+-------------------+
| t1                |
+-------------------+
1 row in set (0.00 sec)

mysql> desc t1;
+--------+---------+------+-----+---------+-------+
| Field  | Type    | Null | Key | Default | Extra |
+--------+---------+------+-----+---------+-------+
| id     | int(11) | YES  |     | NULL    |       |
| salary | float   | YES  |     | NULL    |       |
+--------+---------+------+-----+---------+-------+
2 rows in set (0.01 sec)

mysql> insert into t1 (id, salary) values (12,12.35);
Query OK, 1 row affected (0.00 sec)

mysql> select * from t1;
+------+--------+
| id   | salary |
+------+--------+
|   12 |  12.35 |
+------+--------+
1 row in set (0.00 sec)

mysql> insert into t1 (id, salary) values (12,12.3666);
Query OK, 1 row affected (0.00 sec)

mysql> select * from t1;
+------+---------+
| id   | salary  |
+------+---------+
|   12 |   12.35 |
|   12 | 12.3666 |
+------+---------+
2 rows in set (0.00 sec)

mysql> insert into t1 (id, salary) values (12,12.36665555);
Query OK, 1 row affected (0.01 sec)

mysql> select * from t1;
+------+---------+
| id   | salary  |
+------+---------+
|   12 |   12.35 |
|   12 | 12.3666 |
|   12 | 12.3667 |
+------+---------+
3 rows in set (0.00 sec)
```

#### 1.2.2decimal类型

> ### DECIMAL数据类型的语法
> DECIMAL数据类型的语法如下：
> ```sql
> column_name DECIMAL(precision, scale);
> ```
> 其中，`column_name`是列的名称，`precision`是表示数字总共的位数，包括小数点和符号位，而`scale`表示小数点后的位数。
>
> ### 使用注意事项
> 在使用DECIMAL数据类型时，需要注意以下几点：
> 1. **精确性：** DECIMAL类型用于存储精确的十进制数值，不会存在浮点数精度损失的问题，适合对数值精度要求较高的场景。
> 2. **存储空间：** DECIMAL类型占用的存储空间与precision参数有关，一般来说，每9个位数占用4个字节存储空间。
> 3. **小数位数：** 在定义DECIMAL类型时，需要指定precision和scale两个参数，scale表示小数点后的位数，precision要大于或等于scale。
> 4. **默认值：** 可以为DECIMAL类型指定默认值，例如`DEFAULT 0.00`表示默认值为0.00。
> 5. **范围限制：** DECIMAL类型的取值范围与precision参数有关，通常情况下，最大取值范围是10^38-1到-10^38+1。
>
> 综上所述，DECIMAL数据类型适合存储对精度要求较高的十进制数值，例如货币金额等。如果数值范围比较大但对精度要求不高，可以考虑使用FLOAT或DOUBLE类型。

**decimal相对于float的优势**

> DECIMAL数据类型相对于FLOAT数据类型具有一些优势，特别是在需要精确表示十进制数值时：
>
> 1. **精度和精确性：** DECIMAL类型能够精确表示十进制数值，不会出现浮点数精度损失的问题。这对于对数值精度要求较高的场景非常重要，例如财务计算或精确统计数据。
>
> 2. **适用范围：** 虽然DECIMAL类型的存储空间会随着precision参数增加而增加，但它适用于相对较小的数值范围，通常在10^38范围内，这在很多实际应用中已经足够。
>
> 3. **规范的小数位数：** DECIMAL类型要求明确指定小数位数，使得数据更规范和易于理解。相比之下，FLOAT类型的小数位数是可变的，可能会导致数据存储不一致或误差累积的问题。
>
> 4. **计算准确性：** 在进行数值计算时，DECIMAL类型可以避免浮点数计算误差，保证计算结果的准确性。这对于需要高精度计算的场景非常重要。
>
> 5. **财务应用：** DECIMAL类型常用于财务应用中，例如存储货币金额，因为在这些场景中对数值的精度要求非常高，并且不能容忍任何计算误差。
>
> 综上所述，DECIMAL类型相对于FLOAT类型的优势主要体现在精度和精确性方面，适用于对数值精度要求较高的场景，特别是财务和精确统计等领域。

**decimal位数的情况**

> DECIMAL数据类型在MySQL中支持的最大位数和小数位数取决于版本和存储引擎，一般情况下的规定如下：
>
> 1. **最大位数（precision）：**
>    - MySQL 5.0.3及更高版本支持的最大位数为65。
>    - MySQL 8.0之前的版本，最大位数为65，但实际上，MySQL 5.0.3到MySQL 8.0之间的版本对DECIMAL的最大位数是有限制的，通常是30位左右，具体取决于存储引擎和操作系统。
>    - MySQL 8.0及更高版本对DECIMAL的最大位数为65。
> 2. **小数位数（scale）：**
>    - 小数位数不能超过总位数（precision），因此小数位数的范围通常是0到precision之间。
>    - 如果未显式指定小数位数，MySQL会将其默认为0。
>
> 需要注意的是，虽然MySQL支持较大的DECIMAL位数，但实际使用时应根据实际需求和数据库性能来选择合适的位数，避免过度占用存储空间或影响查询性能。

### 1.3字符串类型

#### 1.3.1char类型

> MySQL中的CHAR数据类型用于存储固定长度的字符串。
>
> ### CHAR数据类型的语法
> CHAR数据类型的语法如下：
> ```sql
> column_name CHAR(length);
> ```
> 其中，`column_name`是列的名称，`length`表示字符串的固定长度，范围是1到255个字符。
>
> ### 使用注意事项
> 在使用CHAR数据类型时，需要注意以下几点：
> 1. **固定长度：** CHAR类型存储的字符串长度是固定的，不管实际存储的字符串长度是多少，都会占用指定长度的存储空间。
> 2. **填充空格：** 如果存储的字符串长度小于指定的固定长度，MySQL会使用空格字符填充剩余的空间。
> 3. **效率：** 对于固定长度的字符串，CHAR类型的存储效率比VARCHAR类型要高，因为不需要额外的存储空间来存储长度信息。
> 4. **查询效率：** 在查询固定长度的字符串时，CHAR类型的查询效率可能比VARCHAR类型更高，因为不需要解析长度信息。
> 5. **存储空间：** CHAR类型占用的存储空间是固定的，取决于指定的固定长度，不会随着存储的实际字符串长度变化而变化。
>
> 综上所述，CHAR数据类型适合存储固定长度的字符串，例如固定长度的代码、标识符等。在选择使用CHAR还是VARCHAR时，需要根据实际需求和数据特性来决定，避免浪费存储空间或影响查询效率。

#### 1.3.2varchar

> MySQL中的VARCHAR数据类型用于存储可变长度的字符串，它具有特定的语法和一些相关细节需要注意。
>
> ### VARCHAR数据类型的语法
> VARCHAR数据类型的语法如下：
> ```sql
> column_name VARCHAR(length);
> ```
> 其中，`column_name`是列的名称，`length`表示字符串的最大长度，范围是1到65535个字符。在MySQL 5.0.3版本之后，VARCHAR数据类型支持的最大长度为65535个字符。
>
> ### 相关细节
> 在使用VARCHAR数据类型时，需要注意以下几点相关细节：
> 1. **可变长度：** VARCHAR类型存储的字符串长度是可变的，即根据实际存储的字符串长度分配存储空间，不会浪费额外的存储空间。
> 2. **存储空间：** VARCHAR类型的存储空间取决于存储的实际字符串长度，实际占用的存储空间为实际字符串长度加上1或2个字节的长度信息。
> 3. **效率：** VARCHAR类型在存储可变长度字符串时相比CHAR类型在存储空间上更加高效，因为不会固定占用固定长度的存储空间。
> 4. **最大长度：** VARCHAR数据类型支持的最大长度是65535个字符。如果需要存储更长的字符串，可以考虑使用TEXT或BLOB类型。
> 5. **字符集：** VARCHAR类型可以指定字符集和校对规则，以支持不同语言的字符存储和比较。
>
> 综上所述，VARCHAR数据类型适合存储可变长度的字符串，例如用户姓名、地址等信息。在选择VARCHAR还是CHAR类型时，需要根据存储的字符串长度是否固定以及存储空间的使用情况来做出合适的选择。
>
> 另外数据库中所指的字符为三个字节。

### 1.4日期和时间类型

> 在MySQL中，日期和时间类型主要有四种：DATE、TIME、DATETIME和TIMESTAMP。下面我将分别介绍它们的语法和一些细节：
>
> #### 1.4.1DATE 类型
>
> - 语法：`DATE`
> - 存储范围：1000-01-01 到 9999-12-31
> - 存储格式：YYYY-MM-DD
> - 使用场景：存储年、月、日，不包含具体的时间信息。
> - 示例：
>   ```
>   CREATE TABLE events (
>       event_date DATE
>   );
>   ```
>
> #### 1.4.2 TIME 类型
> - 语法：`TIME`
> - 存储范围：'-838:59:59' 到 '838:59:59'
> - 存储格式：HH:MM:SS
> - 使用场景：存储时间，不包含日期信息。
> - 示例：
>   ```
>   CREATE TABLE tasks (
>       start_time TIME
>   );
>   ```
>
> #### 1.4.3 DATETIME 类型
> - 语法：`DATETIME`
> - 存储范围：1000-01-01 00:00:00 到 9999-12-31 23:59:59
> - 存储格式：YYYY-MM-DD HH:MM:SS
> - 使用场景：存储日期和时间。
> - 示例：
>   ```
>   CREATE TABLE appointments (
>       appointment_datetime DATETIME
>   );
>   ```
>
> ### 4. TIMESTAMP 类型
> - 语法：`TIMESTAMP`
> - 存储范围：1970-01-01 00:00:01 到 2038-01-19 03:14:07 (在一些情况下，如UTC时区，范围可扩展到 1901 年到 2155 年之间)
> - 存储格式：YYYY-MM-DD HH:MM:SS
> - 使用场景：存储日期和时间，但会根据时区自动转换为 UTC 时间，并且会在插入和更新时自动更新为当前时间。
> - 示例：
>   ```
>   CREATE TABLE log (
>       log_time TIMESTAMP
>   );
>   ```
>
> 注意事项和细节：
>
> 1. 在插入数据时，日期和时间值应该使用正确的格式。否则，MySQL 可能会拒绝插入或者进行默认值填充。
> 2. 对于 TIMESTAMP 类型，MySQL 会根据当前时区进行存储和检索，但实际存储的是 UTC 时间。这意味着在不同的时区，同一时间值的显示可能会有所不同。
> 3. TIMESTAMP 类型在插入和更新时会自动更新为当前时间，但 DATETIME 类型不会。
> 4. 可以使用一些日期和时间函数对日期和时间进行操作，如 `DATE_FORMAT()`、`DATE_ADD()`、`DATE_SUB()` 等。
> 5. 在设计数据库时，应根据需求选择合适的日期和时间类型，并考虑时区、存储空间和查询效率等因素。

### 1.5enum和set类型

> 在MySQL中，ENUM和SET是用于定义列的数据类型，它们允许你在一组预定义的值中选择一个或多个。下面我将解释它们的语法和使用注意事项，并举例说明。
>
> #### 1.5.1ENUM类型
>
> ENUM类型允许你在一个列中定义一个预定义的值列表，**你只能选择其中的一个作为列的值**。
>
> **语法：**
>
> ```sql
> column_name ENUM('value1', 'value2', ...)
> ```
>
> **注意事项：**
>
> - **ENUM列中的值必须是唯一的，重复的值会被忽略。**
> - 如果插入的值不在预定义的值列表中，MySQL会将其转换为空字符串''，如果严格模式开启，插入不在列表中的值会导致错误。
> - 枚举类型的内部存储是整数，但在查询结果中会返回字符串形式的枚举值。
>
> **示例：**
>
> 假设我们有一个表`users`，其中有一个列`gender`使用ENUM类型：
>
> ```sql
> CREATE TABLE users (
>     id INT PRIMARY KEY AUTO_INCREMENT,
>     name VARCHAR(50),
>     gender ENUM('male', 'female')
> );
> ```
>
> 插入一些数据：
>
> ```sql
> INSERT INTO users (name, gender) VALUES ('Alice', 'female');
> INSERT INTO users (name, gender) VALUES ('Bob', 'male');
> ```
>
> #### 1.5.2SET类型
>
> SET类型允许你在一个列中**选择多个预定义的值**，你可以选择其中的一个或多个作为列的值。
>
> **语法：**
>
> ```sql
> column_name SET('value1', 'value2', ...)
> ```
>
> **注意事项：**
> - SET列中的值必须是唯一的，重复的值会被忽略。
> - **如果插入的值不在预定义的值列表中，MySQL会将其转换为空字符串''**，如果严格模式开启，插入不在列表中的值会导致错误。
> - SET类型的内部存储是一个整数(位图的思想)，但在查询结果中会返回一个以逗号分隔的字符串，表示选中的值。
>
> **示例：**
>
> 假设我们有一个表`preferences`，其中有一个列`languages`使用SET类型：
>
> ```sql
> CREATE TABLE preferences (
>     id INT PRIMARY KEY AUTO_INCREMENT,
>     user_id INT,
>     languages SET('English', 'French', 'Spanish', 'German')
> );
> ```
>
> 插入一些数据：
>
> ```sql
> INSERT INTO preferences (user_id, languages) VALUES (1, 'English,Spanish');
> INSERT INTO preferences (user_id, languages) VALUES (2, 'French');
> ```
>
> 这样，用户1选择了英语和西班牙语，用户2选择了法语。