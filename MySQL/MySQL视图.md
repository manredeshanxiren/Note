# MySQL视图

视图是一个虚拟表，其内容由查询定义。同真实的表一样，视图包含一系列带有名称的列和行数据。视
图的数据变化会影响到基表，基表的数据变化也会影响到视图  

## 1.基本使用

- 创建视图  

  ```mysql
  create view 视图名 as select语句；
  ```

- 案例  

  ```
  create view v_ename_dname as select ename, dname
  from EMP, DEPT where EMP.deptno=DEPT.deptno;
  ```

  ```
  select * from v_ename_dname order by dname;
  +--------+------------+
  | ename | dname |
  +--------+------------+
  | CLARK | ACCOUNTING |
  | KING | ACCOUNTING |
  | MILLER | ACCOUNTING |
  | SMITH | RESEARCH |
  | JONES | RESEARCH |
  | SCOTT | RESEARCH |
  | ADAMS | RESEARCH |
  | FORD | RESEARCH |
  | ALLEN | SALES |
  | WARD | SALES |
  | MARTIN | SALES |
  | BLAKE | SALES |
  | TURNER | SALES |
  | JAMES | SALES |
  +--------+------------+
  ```

- 修改了视图，对基表数据有影响  

  ```mysql
  select emp.ename,dept.dname,dept.deptno from emp,dept where
  emp.deptno=dept.deptno order by dname;
  update v_ename_dname set ename='TEST' where ename='CLARK';
  select * from EMP where ename='CLARK';
  select * from EMP where ename='TEST';
  ```

- 修改了基表，对视图有影响  

  ```mysql
  mysql> update EMP set deptno=10 where ename='JAMES'; -- 修改基表
  Query OK, 1 row affected (0.00 sec)
  Rows matched: 1 Changed: 1 Warnings: 0
  mysql> select * from v_ename_dname where ename='JAMES';
  +-------+----------+
  | ename | dname |
  +-------+----------+
  | JAMES | RESEARCH | <== 视图中的数据也发生了变化
  +-------+----------+
  ```

- 删除视图  

  ```mysql
  drop view 视图名；
  ```

## 2.视图规则和限制

> - 与表一样，必须唯一命名（不能出现同名视图或表名）  
> - 创建视图数目无限制，但要考虑复杂查询创建为视图之后的性能影响  
> - 视图不能添加索引，也不能有关联的触发器或者默认值  
> - 视图可以提高安全性，必须具有足够的访问权限  
> - `order by` 可以用在视图中，但是如果从该视图检索数据 select 中也含有 order by ,那么该视图中的 `order by` 将被覆盖  
> - 视图可以和表一起使用  

