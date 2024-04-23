# MySQL的内外连接

## 1.内连接

内连接实际上就是利用where子句对两种表形成的笛卡儿积进行筛选，我们前面学习的查询都是内连接，也是在开发过程中使用的最多的连接查询。  

语法：

```mysql
select 字段 from 表1 inner join 表2 on 连接条件 and 其他条件；
```

案例：显示SMITH的名字和部门名称  

```mysql
-- 用前面的写法
select ename, dname from EMP, DEPT where EMP.deptno=DEPT.deptno and
ename='SMITH';
-- 用标准的内连接写法
select ename, dname from EMP inner join DEPT on EMP.deptno=DEPT.deptno and
ename='SMITH';
```

## 2.外连接

外连接分为左外连接和右外连接  

### 2.1左外连接

如果联合查询，左侧的表完全显示我们就说是左外连接。
语法：  

```mysql
select 字段名 from 表名1 left join 表名2 on 连接条件
```

案例：  

```mysql
-- 建两张表
create table stu (id int, name varchar(30)); -- 学生表
insert into stu values(1,'jack'),(2,'tom'),(3,'kity'),(4,'nono');
create table exam (id int, grade int); -- 成绩表
insert into exam values(1, 56),(2,76),(11, 8);
```

- 查询所有学生的成绩，如果这个学生没有成绩，也要将学生的个人信息显示出来  

  ```mysql
  -- 当左边表和右边表没有匹配时，也会显示左边表的数据
  select * from stu left join exam on stu.id=exam.id;
  ```

### 2.2右外连接

如果联合查询，右侧的表完全显示我们就说是右外连接。  	

语法：

```mysql
select 字段 from 表名1 right join 表名2 on 连接条件；
```

案例：

- 对stu表和exam表联合查询，把所有的成绩都显示出来，即使这个成绩没有学生与它对应，也要显示出来

  ```mysql
  select * from stu right join exam on stu.id=exam.id;
  ```

    

练习：

- 列出部门名称和这些部门的员工信息，同时列出没有员工的部门  

  ```mysql
  方法一：
  select d.dname, e.* from dept d left join emp e on d.deptno=e.deptno;
  方法二：
  select d.dname, e.* from emp e right join dept d on d.deptno=e.deptno;
  ```

  