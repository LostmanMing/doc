> # ***MySQL***
# **基础部分**
## ***数据库相关概念***
|名称|全称|简称|
|:---:|---|---|
|数据库|存储数据的仓库，数据有组织的进行存储|DB|
|数据库管理系统|操纵和管理数据库的大型软件|DBMS|
|SQL|操作关系型数据库的编程语言，定义了一套操作型数据库统一标准 |SQL |


SQL---->DBMS---->DB  
- [MySQL下载地址](https://www.mysql.com/downloads/)

## ***MySQL数据库***
### ***概念***
**关系型数据库**：建立在关系模型的基础上，由**多张相互连接的二维表**组成的数据库  
**数据模型特点**：   
1. 使用表存储数据，格式统一，便于维护。
2. 使用SQL语言操作，标准统一，使用方便。
   
### ***使用***
- 配置环境变量  
    $path : `C:\Program Files\MySQL\MySQL Server 8.0\bin\`
- 启动与停止  
    1. `win+r services.msc`找到mysql服务手动停止
    2. `net start mysql80`   
       `net stop mysql80`
- 客户端连接  
    windows :  
    `mysql [-h 127.0.0.1] [-P 3306] -u root -p`
    `mysql -u root -p`

## ***SQL***
### *SQL通用语法*
1. SQL 语句可以单行或多行书写，以分号结尾、
2. SQL 语句可以使用**空格/缩进**来增强语句的可读性。
3. MUSQL 数据库的 SQL 语句不区分大小写，**关键字建议使用大写**。
4. 注释：
- 单行注释：`--` 或 `#`
- 多行注释：`/* */`

### *SQL分类*
SQL语句，根据其功能，主要分为四类：DDL、DML、DQL、DCL。  
|分类|全称|说明|
|:----:|----|----|
|DDL|Data Definition Language|数据定义语言，用l来定义数据库对象(数据库，表，字段，索引)|
|DML|Data Manipulation Language|数据操作语言，用来对数据库表中的数据进行增删改|
|DQL|Data Query Language|数据查询语言，用来查询数据库中表的记录|
|DCL|Data Control Language|数据控制语言，用来创建数据库用户、控制数据库的访问权限|

### *DDL*
数据库操作 
- 查询数据库  
    查询所有数据库   
    `show databases;`   
    查询当前数据库  
    `select database();` 
- 创建  
    `create database [ if not exists ] 数据库 [ default charset 字符集 ] [ collate 排序规则 ] ;`
- 删除   
    `drop database [ if exists ] 数据库名; `
- 使用/切换数据库  
    `use 数据库名;`  


DDL 表操作
- 查询当前数据库所有表    
    `show tables;`
- 查看指定表结构  
    `desc 表名;`
- 查询指定表的建表语句  
    `show create table 表名;`    
- 创建表结构
    ```bash
    # 结构
    CREATE TABLE 表名(
    字段1 字段1类型 [COMMENT 字段1注释 ],
    字段2 字段2类型 [COMMENT 字段2注释 ],
    字段3 字段3类型 [COMMENT 字段3注释 ],
    ......
    字段n 字段n类型 [COMMENT 字段n注释 ]
    ) [COMMENT 表注释 ] ;

    # 例子
    mysql> create table emp(
    ->     id int ,
    ->     workno varchar(10),
    ->     name varchar(10),
    ->     gender char(1),
    ->     age tinyint unsigned ,
    ->     idcard char(18),
    ->     entrydata date
    -> )comment '员工表';
    Query OK, 0 rows affected (0.01 sec)
    ```
- 数据类型  
    1. 数值类型  
        |类型| 大小 |有符号(SIGNED)范围 |无符号(UNSIGNED)范围| 描述|
        |---|---|---|---|---|
        |TINYINT| 1byte| (-128，127)| (0，255)|小整数值|
        |SMALLINT| 2bytes| (-32768，32767)| (0，65535)|大整数值|
        |MEDIUMINT| 3bytes| (-8388608，8388607)| (0，16777215)|大整数值|
        |INT/INTEGER |4bytes|(-2147483648，2147483647)|(0，4294967295)|大整数值|
        |BIGINT| 8bytes |(-2^63，2^63-1) |(0，2^64-1)|极大整数值|
        |FLOAT |4bytes|(-3.402823466 E+38，3.402823466351 E+38)|0 和 (1.175494351 E-38，3.402823466 E+38)|单精度浮点数值|
        |DOUBLE| 8bytes|(-1.7976931348623157E+308，1.7976931348623157E+308)|0 和(2.2250738585072014E-308，1.7976931348623157E+308)|双精度浮点数值|
        |DECIMAL| |依赖于M(精度)和D(标度)的值|依赖于M(精度)和D(标度)的值|小数值(精确定点数)|
    2. 字符类型  
        |类型 |大小| 描述|
        |---|---|---|
        |CHAR| 0-255 bytes |定长字符串(需要指定长度)|
        |VARCHAR| 0-65535 bytes |变长字符串(需要指定长度)|
        |TINYBLOB |0-255 bytes |不超过255个字符的二进制数据|
        |TINYTEXT| 0-255 bytes| 短文本字符串|
        |BLOB |0-65 535 bytes |二进制形式的长文本数据|
        |TEXT |0-65 535 bytes |长文本数据|
        |MEDIUMBLOB| 0-16 777 215 bytes| 二进制形式的中等长度文本数据|
        |MEDIUMTEXT |0-16 777 215 bytes |中等长度文本数据|
        |LONGBLOB |0-4 294 967 295 bytes|二进制形式的极大文本数据|
        |LONGTEXT |0-4 294 967 295 bytes| 极大文本数据 |
        不确定长度使用`varchar`  
        确定长度使用`char`
    3. 日期和时间类型  
        |类型大小| 范围 |格式 |描述|
        |---|---|---|---|
        |DATE |3 |1000-01-01 至 9999-12-31| YYYY-MM-DD |日期值|
        TIME |3 |-838:59:59 至 838:59:59| HH:MM:SS|时间值或持续时间|
        |YEAR |1 |1901 至 2155 |YYYY| 年份值|
        |DATETIME| 8|1000-01-01 00:00:00 至9999-12-31 23:59:59|YYYY-MM-DD HH:MM:SS|混合日期和时间值|
        |TIMESTAMP| 4|1970-01-01 00:00:01 至2038-01-19 03:14:07|YYYY-MM-DD HH:MM:SS|混合日期和时间值，时间戳|
- 修改表结构  
1.  添加字段
    `ALTER TABLE 表名 ADD 字段名 类型 (长度) [ COMMENT 注释 ] [ 约束 ];`
2. 修改字段  
    修改数据类型  
    `ALTER TABLE 表名 MODIFY 字段名 新数据类型 (长度);`  
    修改字段名和字段类型  
    `ALTER TABLE 表名 CHANGE 旧字段名 新字段名 类型 (长度) [ COMMENT 注释 ] [ 约束 ];`
3. 删除字段  
    `ALTER TABLE 表名 DROP 字段名;`  
4. 修改表名  
    `ALTER TABLE 表名 RENAME TO 新表名;`
5. 删除表  
    1. 删除表  
    `DROP TABLE [ IF EXISTS ] 表名;`
    2.  删除指定表, 并重新创建表  
    `TRUNCATE TABLE 表名;`  

### *DML*
DML 表操作  
添加数据（**INSERT**）  
修改数据（**UPDATE**）  
删除数据（**DELETE**）  

- 添加数据  
  ```
  注意：
  1. 插入数据时，指定的字段顺序需要与值的顺序是一一对应的。
  2. 字符串和日期型数据应该包含在引号中。
  3. 插入的数据大小，应该在字段的规定范围内。
  ```
1. 给指定字段添加数据   
`INSERT INTO 表名 (字段名1, 字段名2, ...) VALUES (值1, 值2, ...);`   
2. 给全部字段添加数据  
`INSERT INTO 表名 VALUES (值1, 值2, ...);`   
3. 批量添加数据  
`INSERT INTO 表名 (字段名1, 字段名2, ...) VALUES (值1, 值2, ...), (值1, 值2, ...), (值1, 值2, ...) ;`   
`INSERT INTO 表名 VALUES (值1, 值2, ...), (值1, 值2, ...), (值1, 值2, ...) ;`

- 修改数据  
   `UPDATE 表名 SET 字段名1 = 值1 , 字段名2 = 值2 , .... [ WHERE 条件 ] ;`
- 删除数据  
    `DELETE FROM 表名 [ WHERE 条件 ] ;`
    ```
    注意：
    修改语句的条件可以有，也可以没有，如果没有条件，则会修改整张表的所有数据。
    ```

### *DQL*

DQL 表操作  


查询关键字: ***SELECT***  
在一个正常的业务系统中，查询操作的频次是要远高于增删改的，当我们去访问企业官网、电商网站，
在这些网站中我们所看到的数据，实际都是需要从数据库中查询并展示的。而且在查询的过程中，可能
还会涉及到条件、排序、分页等操作。
- DQL语法及编写时命令顺序
    ```sql
    SELECT
        字段列表
    FROM
        表名列表
    WHERE
        条件列表
    GROUP BY
        分组字段列表
    HAVING
        分组后条件列表
    ORDER BY
        排序字段列表
    LIMIT
        分页参数
    ```
- 基本查询
    1. 查询多个字段  
    `SELECT 字段1, 字段2, 字段3 ... FROM 表名 ;`  
    `SELECT * FROM 表名 ;`
    2. 设置别名  
    `SELECT 字段1 [ AS 别名1 ] , 字段2 [ AS 别名2 ] ... FROM 表名; 1`    
    `SELECT 字段1 [ 别名1 ] , 字段2 [ 别名2 ] ... FROM 表名;`
    3. 去除重复记录
    `SELECT DISTINCT 字段列表 FROM 表名; `
- 条件查询  
    1. 语法  
        `SELECT 字段列表 FROM 表名 WHERE 条件列表 ;`
    2. 条件  
        |比较运算符| 功能|
        |  :----:|:----:|
        |> |大于|
        |>= |大于等于|
        |< |小于|
        |<= |小于等于|
        | =| 等于|
        |<> 或 !=| 不等于|
        |BETWEEN ... AND ... |在某个范围之内(含最小、最大值)|
        |IN(...) |在in之后的列表中的值，多选一
        |LIKE 占位符 |模糊匹配(_匹配单个字符, %匹配任意个字符)|
        |IS NULL| 是NULL|


        |逻辑运算符|功能|
        |  :----:|:----:|
        |AND 或 &&| 并且 (多个条件同时成立)|    
        |OR 或 \| \| |或者 (多个条件任意一个成立)|
        |NOT 或 !| 非 , 不是|
        ```sql
        # 例子
        # 查询年龄等于18 或 20 或 40 的员工信息
        select * from emp where age in(18,20,40);
        # 查询姓名为两个字的员工信息 _ %
        select * from emp where name like '__';
        # 查询身份证号最后一位是X的员工信息
        select * from emp where idcard like '%X';
        ```
- 聚合函数
    ```
    注意：
    NULL值是不参与所有聚合函数运算的。
    ```


    1. 介绍
    将一列数据作为一个整体，进行纵向计算。
    2. 常见的聚合函数  
        |函数 |功能|
        |:---:|:---:|
        |count| 统计数量|
        |max |最大值|
        |min| 最小值|
        |avg |平均值|
        |sum |求和|
    3. 语法  
    ` SELECT 聚合函数(字段列表) FROM 表名 ;`

- 分组查询
    1. 语法  
    `SELECT 字段列表 FROM 表名 [ WHERE 条件 ] GROUP BY 分组字段名 [ HAVING 分组 后过滤条件 ];`   
    2. where与having区别  
        - 执行时机不同：where是分组之前进行过滤，不满足where条件，不参与分组；而having是分组之后对结果进行过滤。  
        - 判断条件不同：where不能对聚合函数进行判断，而having可以。
        ```bash
        # 查询年龄小于45的员工 , 并根据工作地址分组 , 获取员工数量大于等于3的工作地址
        select workaddress, count(*) address_count from emp where age < 45 group by workaddress having address_count >= 3;
        # 统计各个工作地址上班的男性及女性员工的数量
        select workaddress, gender, count(*) '数量' from emp group by gender , workaddress;
        
        
        注意：
        • 分组之后，查询的字段一般为聚合函数和分组字段，查询其他字段无任何意义。
        • 执行顺序: where > 聚合函数 > having
        • 支持多字段分组, 具体语法为 : group by columnA,columnB
        ```
- 排序查询
    1. 语法  
    `SELECT 字段列表 FROM 表名 ORDER BY 字段1 排序方式1 , 字段2 排序方式2 ;`  
    2. 排序方式  
        - ASC : 升序(默认值)
        - DESC: 降序
        ```bash
        # 根据年龄对公司的员工进行升序排序 , 年龄相同 , 再按照入职时间进行降序排序
        select * from emp order by age asc , entrydate desc;
        注意：
        • 如果是升序, 可以不指定排序方式ASC ;
        • 如果是多字段排序，当第一个字段值相同时，才会根据第二个字段进行排序 ;
        ```
- 分页查询  
    语法  
    `SELECT 字段列表 FROM 表名 LIMIT 起始索引, 查询记录数 ; 1`
    ```bash
    # 查询第2页员工数据, 每页展示10条记录 --------> (页码-1)*页展示记录数
    select * from emp limit 10, 10;
    注意事项:
    • 起始索引从0开始，起始索引 = （查询页码 - 1）* 每页显示记录数。
    • 分页查询是数据库的方言，不同的数据库有不同的实现，MySQL中是LIMIT。
    • 如果查询的是第一页数据，起始索引可以省略，直接简写为 limit 10。 
    ```
- DQL 执行顺数
    ```bash
    FROM
        表名列表
    WHERE
        条件列表
    GROUP BY
        分组字段列表
    HAVING
        分组后条件列表
    SELECT
        字段列表
    ORDER BY
        排序字段列表
    LIMIT
        分页参数
    
    # 在查询时，我们给emp表起一个别名 e，然后在select 及 where中使用该别名。
    select e.name , e.age from emp e where e.age > 15 order by age asc;
    # 执行上述SQL语句后，我们看到依然可以正常的查询到结果，此时就说明： from 先执行, 然后 where 和 select 执行。

    #我们可以给select后面的字段起别名，然后在 where 中使用这个别名，然后看看是否可以执行成功。
    select e.name ename , e.age eage from emp e where eage > 15 order by age asc;
    # 报错！由此我们可以得出结论: from 先执行，然后执行 where ， 再执行select。

    select e.name ename , e.age eage from emp e where e.age > 15 order by eage asc;
    # 结果执行成功。 那么也就验证了: order by 是在select 语句之后执行的。

    # 综上所述，我们可以看到DQL语句的执行顺序为： from ... where ... group by ...having ... select ... order by ... limit ...
    ```


### *DCL*
- 介绍  
    DCL英文全称是Data Control Language(数据控制语言)，用来管理数据库**用户**、控制数据库的**访问权限**。  
    主要是**数据库管理人员 DBA** 在使用。
    
- DCL 管理用户  
    ```
    注意事项:
    • 在MySQL中需要通过用户名@主机名的方式，来唯一标识一个用户。
    • 主机名可以使用 % 通配。
    • 这类SQL开发人员操作的比较少，主要是DBA（ Database Administrator 数据库管理员）使用。
    ```
    1. 查询用户  
        `use mysql`   
        `select * from mysql.user;`  本机存放数据库名称等信息的表
    2. 创建用户  
        `CREATE USER '用户名'@'主机名' IDENTIFIED BY '密码';`
    3. 修改用户密码  
        `ALTER USER '用户名'@'主机名' IDENTIFIED WITH mysql_native_password BY '新密码' ;`
    4.  删除用户
        `DROP USER '用户名'@'主机名' ;`
    ```bash
    # 创建用户itcast, 只能够在当前主机localhost访问, 密码123456;
    create user 'itcast'@'localhost' identified by '123456';
    # 创建用户zhao, 可以在任意主机访问该数据库, 密码123456;
    create user 'zhao'@'%' identified by '123456';
    # 修改用户zhao的访问密码为1234;
    alter user 'heima'@'%' identified with mysql_native_password by '1234';
    # 删除 itcast@localhost 用户
    drop user 'itcast'@'localhost';
    ```
- DCL 权限控制  
    |权限 |说明|
    |:---:|:---:|
    |ALL, ALL PRIVILEGES| 所有权限|
    |SELECT |查询数据|
    |INSERT| 插入数据|  
    |UPDATE |修改数据|
    |DELETE| 删除数据|
    |ALTER |修改表|
    |DROP |删除数据库/表/视图|
    |CREATE |创建数据库/表|
    1.  查询权限  
        `SHOW GRANTS FOR '用户名'@'主机名' ; 1`
    2.  授予权限  
        `GRANT 权限列表 ON 数据库名.表名 TO '用户名'@'主机名';`
    3. 撤销权限  
        `REVOKE 权限列表 ON 数据库名.表名 FROM '用户名'@'主机名';`
    ```bash
    # 查询 'heima'@'%' 用户的权限
    show grants for 'heima'@'%';
    # 授予 'heima'@'%' 用户itcast数据库所有表的所有操作权限
    grant all on itcast.* to 'heima'@'%';
    # 撤销 'heima'@'%' 用户的itcast数据库的所有权限
    revoke all on itcast.* from 'heima'@'%';

    注意事项：
    • 多个权限之间，使用逗号分隔
    • 授权时， 数据库名和表名可以使用 * 进行通配，代表所有。    
    ```

## ***函数***
> **函数** 是指一段可以直接被另一段程序调用的程序或代码。 也就意味着，这一段程序或代码在MySQL中
已经给我们提供了，我们要做的就是在合适的业务场景调用对应的函数完成对应的业务需求即可。
### 字符串函数
|函数| 功能|
|---------|--------|
|CONCAT(S1,S2,...Sn)| 字符串拼接，将S1，S2，... Sn拼接成一个字符串|
|LOWER(str)| 将字符串str全部转为小写|
|UPPER(str)| 将字符串str全部转为大写|
|LPAD(str,n,pad)|左填充，用字符串pad对str的左边进行填充，达到n个字符串长度|
|RPAD(str,n,pad)|右填充，用字符串pad对str的右边进行填充，达到n个字符串长度|
|TRIM(str) |去掉字符串头部和尾部的空格SUBSTRING(str,start,len) 返回从字符串str从start位置起的len个长度的字符串|
`select 函数`

### 数值函数
|函数 |功能|
|---------|--------|
|CEIL(x)| 向上取整|
|FLOOR(x) |向下取整|
|MOD(x,y) |返回x/y的模|
|RAND() |返回0~1内的随机数|
|ROUND(x,y) |求参数x的四舍五入的值，保留y位小数|
```bash
# 通过数据库的函数，生成一个六位数的随机验证码。
select lpad(round(rand()*1000000 , 0), 6, '0');
```

### 日期函数
|函数 |功能|
|---------|--------|
|CURDATE()| 返回当前日期|
|CURTIME()| 返回当前时间|
|NOW() |返回当前日期和时间|
|YEAR(date)| 获取指定date的年份|
|MONTH(date) |获取指定date的月份|
|DAY(date) |获取指定date的日期|
|DATE_ADD(date, INTERVAL expr type)|返回一个日期/时间值加上一个时间间隔expr后的时间值|
|DATEDIFF(date1,date2)|返回起始时间date1 和 结束时间date2之间的天数|

### 流程控制函数
|函数| 功能 |
|---------|--------|
|IF(value , t , f)|如果value为true，则返回t，否则返回f|
|IFNULL(value1 , value2)| 如果value1不为空，返回value1，否则返回value2|
|CASE WHEN [ val1 ] THEN [res1] ... ELSE [ default ] END|如果val1为true，返回res1，... 否则返回default默认值|
|CASE [ expr ] WHEN [ val1 ] THEN [res1] ... ELSE [ default ] END|如果expr的值等于val1，返回res1，... 否则返回default默认值|

## ***约束***
### ***概述***
-  **概念**  
    > 约束是作用于表中字段上的规则，用于限制存储在表中的数据。

-  **目的**  
    > 保证数据库中数据的正确、有效性和完整性。
- **分类**
    |约束 | 描述 |关键字|
    |---|---|---|
    |非空约束| 限制该字段的数据不能为null| NOT NULL|
    |唯一约束 |保证该字段的所有数据都是唯一、不重复的| UNIQUE|
    |主键约束 |主键是一行数据的唯一标识，要求非空且唯一|PRIMARY KEY|
    |默认约束 |保存数据时，如果未指定该字段的值，则采用默认值| DEFAULT|
    |检查约束(8.0.16版本之后)|保证字段值满足某一个条件 |CHECK|
    |外键约束|用来让两张表的数据之间建立连接，保证数据的一致性和完整性|FOREIGN KEY|

- **外键约数**
    > ***概念***  
        用来让两张表的数据之间建立连接，从而保证数据的一致性和完整性。
    - 添加外键
        ```bash
        # 创建时
        CREATE TABLE 表名(
            字段名 数据类型,
            ...
            [CONSTRAINT] [外键名称] FOREIGN KEY (外键字段名) REFERENCES 主表 (主表列名)
        );

        # 创建后添加
        ALTER TABLE 表名 ADD CONSTRAINT 外键名称 FOREIGN KEY (外键字段名) REFERENCES 主表 (主表列名) ;
        ```
    - 删除外键
        `ALTER TABLE 表名 DROP FOREIGN KEY 外键名称;`
    - 删除和更新行为  
        添加了外键之后，再删除父表数据时产生的约束行为，我们就称为删除/更新行为。具体的删除/更新行为有以下几种:  
        |行为 |说明|
        |---|---|
        |NO ACTION|当在父表中删除/更新对应记录时，首先检查该记录是否有对应外键，如果有则不允许删除/更新。 (与 RESTRICT 一致) 默认行为|
        |RESTRICT|当在父表中删除/更新对应记录时，首先检查该记录是否有对应外键，如果有则不允许删除/更新。 (与 NO ACTION 一致) 默认行为|
        |CASCADE|当在父表中删除/更新对应记录时，首先检查该记录是否有对应外键，如果有，则也删除/更新外键在子表中的记录。|
        |SET NULL|当在父表中删除对应记录时，首先检查该记录是否有对应外键，如果有则设置子表中该外键值为null（这就要求该外键允许取null）。|
        |SET DEFAULT|父表有变更时，子表将外键列设置成一个默认的值 (Innodb不支持)|
        `ALTER TABLE 表名 ADD CONSTRAINT 外键名称 FOREIGN KEY (外键字段) REFERENCES 主表名 (主表字段名) ON UPDATE CASCADE ON DELETE CASCADE;`


## ***多表查询***

### 多表关系：
- 一对多(多对一)
     - 在多的一方建立外键，指向一的一方的主键
- 多对多
    - 建立第三张中间表，中间表至少包含两个外键，分别关联两方主键 
- 一对一
    - 在任意一方加入外键，关联另外一方的主键，并且设置外键为唯一的(UNIQUE)
### 多表查询概述
> 笛卡尔积:  
>  笛卡尔乘积是指在数学中，两个集合A集合 和 B集合的所有组合情况。
> SQL语句中，去除无效的笛卡尔积:  
> `select * from emp , dept where emp.dept_id = dept.id;`
- 分类
    - 连接查询
        - 内连接：相当于查询A、B交集部分数据
        - 外连接：
        - 左外连接：查询左表所有数据，以及两张表交集部分数据
        - 右外连接：查询右表所有数据，以及两张表交集部分数据
        - 自连接：当前表与自身的连接查询，自连接必须使用表别名
    - 子查询
- 内连接
    1. 隐式内连接
        `SELECT 字段列表 FROM 表1 , 表2 WHERE 条件 ... ; 1`
    2. 显示内连接
        `SELECT 字段列表 FROM 表1 [ INNER ] JOIN 表2 ON 连接条件 ... ;` 
        ``` 
        一旦为表起了别名，就不能再使用表名来指定对应的字段了，此时只能够使用别名来指定字段
        ```
- 外连接
    ```
    注意：
    左外连接和右外连接是可以相互替换的，只需要调整在连接查询时SQL中，表结构的先后顺序就可以了。而我们在日常开发使用时，更偏向于左外连接。
    ```
    1. 左外连接
        `SELECT 字段列表 FROM 表1 LEFT [ OUTER ] JOIN 表2 ON 条件 ... ; `  
        左外连接相当于查询表1(左表)的所有数据，当然也包含表1和表2交集部分的数据。完全包含左表数据。
    2. 右外连接
        `SELECT 字段列表 FROM 表1 RIGHT [ OUTER ] JOIN 表2 ON 条件 ... ;`  
        右外连接相当于查询表2(右表)的所有数据，当然也包含表1和表2交集部分的数据。完全包含右表数据。

- 自连接  
    >   自连接查询，顾名思义，就是自己连接自己，也就是把一张表连接查询多次。

    `SELECT 字段列表 FROM 表A 别名A JOIN 表A 别名B ON 条件 ... ;`  
    对于自连接查询，可以是内连接查询，也可以是外连接查询。
    ```
    注意:
    在自连接查询中，必须要为表起别名，要不然我们不清楚所指定的条件、返回的字段，到底是哪一张表的字段。
    ```
- 联合查询
    > 对于union查询，就是把多次查询的结果合并起来，形成一个新的查询结果集。
    ```
    SELECT 字段列表 FROM 表A ...
    UNION [ ALL ]
    SELECT 字段列表 FROM 表B ....;
    ```
    -   对于联合查询的多张表的列数必须保持一致，字段类型也需要保持一致。
    -   union all 会将全部的数据直接合并在一起，union 会对合并之后的数据去重。
  
- 子查询
    1. 概念  
        SQL语句中嵌套SELECT语句，称为嵌套查询，又称子查询。  
        `SELECT * FROM t1 WHERE column1 = ( SELECT column1 FROM t2 ); 1`  
        子查询外部的语句可以是INSERT / UPDATE / DELETE / SELECT 的任何一个。
    2. 分类  
        根据子查询结果不同：
        - 标量子查询（子查询结果为单个值）
        - 列子查询(子查询结果为一列)
        - 行子查询(子查询结果为一行)
        - 表子查询(子查询结果为多行多列)  

        根据子查询位置不同:  
        - WHERE之后
        - FROM之后
        - SELECT之后

    - 标量子查询   
        子查询返回的结果是**单个值**（数字、字符串、日期等），最简单的形式，这种子查询称为标量子查询。常用的操作符：`=` `<>` `>` `>=` `<` `<=`
    - 列子查询
        子查询返回的结果是**一列**（可以是多行），这种子查询称为列子查询。
        |操作符| 描述|
        |---|---|
        |IN| 在指定的集合范围之内，多选一|
        |NOT IN |不在指定的集合范围之内|
        |ANY |子查询返回列表中，有任意一个满足即可|
        |SOME |与ANY等同，使用SOME的地方都可以使用ANY|
        |ALL |子查询返回列表的所有值都必须满足|
    - 行子查询  
        子查询返回的结果是**一行**（可以是多列），这种子查询称为行子查询。  
        常用的操作符：= 、<> 、IN 、NOT IN  
        `select * from emp where (salary,managerid) = (select salary, managerid from empwhere name = '张无忌');`
    - 表子查询  
        子查询返回的结果是**多行多列**，这种子查询称为表子查询。  
        常用的操作符：***IN***  
        `select * from emp where (job,salary) in ( select job, salary from emp where name ='鹿杖客' or name = '宋远桥' );`