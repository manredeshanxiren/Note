# MySQL表的操作

## 1.创建表

语法：

```mysql
CREATE TABLE table_name (
field1 datatype,
field2 datatype,
field3 datatype
) character set 字符集 collate 校验规则 engine 存储引擎;
```

说明：

- field 表示列名
- datatype 表示列的类型
- character set 字符集，如果没有指定字符集，则以所在数据库的字符集为准
- collate 校验规则，如果没有指定校验规则，则以所在数据库的校验规则为准

### 1.1创建表的案例

```mysql
create table users (
id int,
name varchar(20) comment '用户名',
password char(32) comment '密码是32位的md5值',
birthday date comment '生日'
) character set utf8 engine MyISAM;
```

说明：

- 不同的存储引擎，创建表的文件不一样。

  `user`表存储引擎是`MYISAM`，在数据目中有三个不同的文件，分别是：

  -users.frm:表结构

  -users.MYD:表数据

  -users.MYI:表索引

![image-20240309112710874](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309112710874.png)

创建一个innodb的数据库，观察一下存储目录：

发现后缀是idb，并且好像表数据和表索引在同一个文件。

![image-20240309113142140](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309113142140.png)

## 2.查看表结构

### 2.1查看表结构

```mysql
desc 表名;
```

示例：

![image-20240309113813433](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309113813433.png)

### 2.2查看表创建时的语句

```mysql
show create table 表名;
```

示例：

其中末尾的`\G`目的是让MySQL去掉一些不必要的显示，并且格式化我们的语句。

![image-20240309114008862](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309114008862.png)

## 3.修改表

在项目实际开发中，经常修改某个表的结构，比如字段名字，字段大小，字段类型，表的字符集类型，表的存储引擎等等。我们还有需求，添加字段，删除字段等等。这时我们就需要修改表。  

```mysql
ALTER TABLE tablename ADD (column datatype [DEFAULT expr][,column
datatype]...);
ALTER TABLE tablename MODIfy (column datatype [DEFAULT expr][,column
datatype]...);
ALTER TABLE tablename DROP (column);
```

案例：

- 在users表添加二条记录  

  ```mysql
  mysql> insert into users values(1, '张三', '1234' , '2002-9-21', ''),(2, '李四','3213','2003-5-22','');
  ```

  结果：

  ![image-20240309121829700](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309121829700.png)

- 在users表添加一个字段，用于保存图片路径  

  ```mysql
  ysql> alter table users add imagepath varchar(128) comment '图片路径' after birthday;
  ```

  结果：

  ![image-20240309121920235](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309121920235.png)

- 修改name，将其长度改成60  

  ```mysql
  mysql> alter table users modify name varcahr(60);
  ```

  结果：

  ![image-20240309122158564](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309122158564.png)

- 删除password列

  > 注意：删除字段一定要小心，删除字段及其对应的列数据都没了  

![image-20240309122336386](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309122336386.png)

- 修改表名位employee

  to：可以省掉  

  ```mysql
  mysql> alter table users rename to employee;
  ```

  结果：

  ![image-20240309122511598](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309122511598.png)

- 将name列修改为xingming  

  ```mysql
  mysql> alter table employee change name xingming varchar(20);
  ```

  结果：

  ![image-20240309122711686](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309122711686.png)

- 删除表

  语法格式：

  ```mysql
  DROP [TEMPORARY] TABLE [IF EXISTS] tbl_name [, tbl_name] ...
  ```

  示例：

  ```mysql
  mysql> drop table users1;
  ```

  结果：

  ![image-20240309123321776](https://gitee.com/slow-heating-shaanxi-people/pictrue/raw/master/pmm/image-20240309123321776.png)