<div align='center'>
  <h1>Shell脚本 笔记</h1>
</div>

- [Shell基础](#shell基础)
  - [Shebang](#shebang)
  - [变量](#变量)
    - [本地变量](#本地变量)
    - [Shell子串](#shell子串)
  - [父子shell](#父子shell)
  - [条件测试](#条件测试)
  - [if语句](#if语句)
  - [函数](#函数)


# Shell基础
## Shebang
告诉系统用什么shell解释器执行脚本
```bash
#!/bin/bash
#!/bin/per1
#!/bin/ python
```
## 变量
### 本地变量
shell变量定义时不需要加 `$` ,取出时才加  
本地变量只在用户当前 `shell` 生存期中有效  
- 变量定义  
    1. 变量名要求:字母、数字、下划线组成、可以是字母或是下划线开头
       - shell
       - shell_script
       - _shell_script
    2. 变量严格区大小写
    3. 赋值可以不加引号，加单引号或双引号
        - num=1
        - num='1'
        - num="1"
    >注：变量与值之间不得有空格，不能以数字开头，不能有小写符号
- 取出变量值  
    `$var` / `${vart}`
    1. 单引号，所见即所得，强引用  (识别为普通字符串) 
    2. 双引号，输出引号里所有内容，识别特殊符号，弱引用 (变量名会替换为=变量值)
    3. 无引号，连续的符号可以不加引号，有空格则有歧义，最好使用双引号
    4. 反引号，引用命令执行结果，等于$()用法
- 特殊变量  
shell的特殊变量，用在如脚本，函数传递参数使用，有如下特殊的，位置参数变量  
    - `$0` 获取shell脚本文件名，以及脚本路径
    - `$n` 获取shell脚本的第n个参数,n在1~9之间， 如$1 ,$2, $9，大于9则需要写，${10}，参数空格隔开
    - `$#` 获取执行的shell脚本后面的参数总个数 (不包括脚本名称)
    - `$*` 获取shel1脚本所有参数，不加引号等同于`$@`作用，加上引号"$*"作用是接收所有参数为单个字符串,"$1 $2.."
    - `$@` 不加引号，效果同上，加引号，是接收所有参数为独立字符串，如"$1" "$2" "$3" 空格保留
    ```bash
    $*和$@的区别你了解吗?

    当$*和$@不被双引号" "包围时，它们之间没有任何区别，都是将接收到的每个参数看做一份数据，彼此之间以空格来分隔。

    但是当它们被双引号" "包含时，就会有区别了:
    "$*"会将所有的参数从整体上看做一份数据，而不是把每个参数都看做一份数据。
    "$@"仍然将每个参数都看作一份数据，彼此之间是独立的。

    比如传递了5个参数，那么对于"$*"来说，这5个参数会合并到一起形成一份数据，它们之间是无法分割的;而对于"$@"来说，这5个参数是相互独立的，它们是5份数据。

    如果使用echo直接输出"$*"和"$@"做对比，是看不出区别的;但如果使用for 循环来逐个输出数据，立即就能看出区别来。
                for var in "$*"                         for var in "$@"
                do                                      do
                    echo "$var"                             echo "$var"
                done                                    done
                     整体                                     独立
    ```
- 特殊状态变量
脚本执行结束会返回一个数字，称为返回值。
    - `&?` 0:成功      1-255:错误码
    - `$$` 当前shell脚本的进程号
    - `$!` 上一次后台进程的PID
    - `$_` 再此之前执行的命令的最后一个参数   
`man bash` 中 查询 `/Special Para` 可查看所有特殊变量

### Shell子串
Shell子串用法
`a*c`  匹配开头为a，中间任意个字符，结尾为c的字符串，下面说的最长最短就是匹配到的字符串的长短
```bash
${变量}                 返回变量值
${#变量}                返回变量长度，字符长度
${变量:start}           返回变量start数值之后的字符
${变量:start:length}    提取offset之后的length限制的字符
${变量#word}            从变量开头删除最短匹配的word子串             开头必须为word的开头
${变量##word}           从变量开头，删除最长匹配的word              开头必须为word的开头
${变量%word}            从变量结尾删除最短的word                结尾必须为word的结尾
${变量/pattern/string}  用string代替 第一个匹配的pattern        结尾必须为word的结尾
${变量//pattern/string} 用string代替所有的pattern
```
- 统计变量子串长度
    ```bash
    root@DESKTOP-NGKBUB9:~# name=zhaogming
    root@DESKTOP-NGKBUB9:~# echo "${name}" | wc -L      l统计行数，L统计最长的行中的字符个数
    9
    root@DESKTOP-NGKBUB9:~# expr length "${name}"
    9
    root@DESKTOP-NGKBUB9:~# echo ${#name}          最快的方式
    9
    ```
- 特殊shell扩展变量  
    下面四个扩展变量都属于对变量的值进行判断处理
    ```bash
    # 如果parameter变量值为空，返回word字符串， 赋值给result变量,不为空正常赋值
    result=${parameter:-word}

    # 如果para变量为空，则word替代变量值，且返回其值,不为空正常赋值
    result=${parameter:=word}

    # 如果para变量为空，word当作stderr输出，否则输出变量值
    用于设置变量为空导致错误时，返回的错误信息
    ${parameter:?word}

    # 如果para变量为空，什么都不做，否则word返回给res
    $res={parameter:+word}
    ```
    案例
    ```bash
    find xargs 搜索，且删除
    # 删除7天以上的过期数据
    find 需要搜索的目录 -name 你要搜索的文件名字 -type 文件类型-mtime +7 |xargs rm -f
    #如果${dir_path}为空，就会在当前目录，搜索，删除
    find ${dir_path} -name '* . tar.gz' -type f -mtime +7 |xargs rm -f
    #变量扩展的改进
    find ${dir_ path:=/data/mysql_back_data/} -name '*.tar.gz' -type f -mtime +7 | xargs rm -f
    ```
## 父子shell
- 三种执行脚本方式  
  1. `bash/sh + 脚本名`   **开启subshell**
  2. `./脚本名`           **开启subshell**
  3. `source/. + 脚本名`  **当前shell**
  ```bash
  echo var='var' > var_test.sh
  bash var_test.sh
  echo $var     # 无输出，新开子shell执行，父shell无法保留变量
  source var_test.sh
  echo $var     # 输出var，source / . 都是在当前shell加载脚本，可以保留变量。
- 查看父子shell
    1. `pstree`
    2. `ps -ef --forest`  
        `-f` 显示 UID/PID/PPID
        `-e` 列出所有进程信息
- 进程列表
    `$BASH_SUBSHELL`结果为0说明是在当前shell执行  
    - 执行一系列shell命令  
    `ls;cd~;pwd;echo $BASH_SUBSHELL`  在当前shell运行  
    通过进程列表的方式，也就是将命令在小括号内执行  
    `(ls;cd~;pwd;echo $BASH_SUBSHELL)` 会开启子shell执行  

## shell 内置外置命令
**内置命令**: 在系统启动时就加载入内存，常驻内存，执行效率更高，但是占用资源  
**外置命令**: 用户需要从硬盘中读取程序文件，再读入内存加载
- 可以通过 `type` 命令查看命令是否是内置命令 
    ```bash
    root@DESKTOP-NGKBUB9:/home/zhaogming/github/Leetcode#  type cd
    cd is a shell builtin
    root@DESKTOP-NGKBUB9:/home/zhaogming/github/Leetcode# type pstree
    pstree is /usr/bin/pstree
    ```
- `compgen -b` 列出所有内置命令 
## shell 算数运算命令
shell的一些基础命令，只支持整数运算，小数运算需要如bc这样的命令才支持
- 双小括号 `(())`  **效率高**
![Alt text](assert/%E5%8F%8C%E5%B0%8F%E6%8B%AC%E5%8F%B7.png)
- `let`  
    可以让不支持数值运算的shell变量转义为可以进行计算。
- `expr`  
    支持基本的计算，基于空格传入参数，但不是很好用因为在shell中一些元字符是有含义的，用于计算需要转义
- `bc`  
    bc命令是当做计算器用的 
    - bc结合管道符  
    `echo "3.2*5" | bc`
- `tr` 命令替换，将空格替换为运算符，方便计算  
    `echo {1.1000} | tr " " "+"`
- `seq`  
    `seq 100` 原本是以换行为分隔符，列出1-100,但可以使用 `-s` 指定分隔符为运算符`seq -s "+" 100 | bc`
- `awk`  
    `echo "3.2 2.2" | awk '{print ($1*$2)}'`
- `$[]`
    `echo $[num*4]`
## for循环


基本语法
```bash
for num in {1..100}
do 
    echo $num
done

# 写在一行
for num in {1..100};do echo $num;done
```

统计运行时间
```bash
root@DESKTOP-NGKBUB9:~# time for num in {1..100000};do echo $num &>/dev/null;done
real    0m0.373s    实际运行时间
user    0m0.259s    用户态运行时间
sys     0m0.113s    内核态巡行时间
```
## 条件测试
- `read`命令  
让用户从终端输入  
    `-p` 指定输入提示  
    `-t` 指定超时时间  

能够提供条件测试的语法，有如下
- test命令
- \[ ] 中括号
  ![Alt text](./assert/%E6%9D%A1%E4%BB%B6%E6%B5%8B%E8%AF%95%E5%B8%B8%E7%94%A8%E8%AF%AD%E6%B3%95.png)
1. test  
test 命令最短的定义可能是评估⼀个表达式；如果条件为真，则返回⼀个 0 值。如果表达式不为真，则返回⼀个⼤于 0 的值 — 也可以将其称为假值。检查最后所执⾏命令的状态的最简便⽅法是使⽤ `$?` 值。
- 语法
    ```bash
    1. 关于某个⽂件名的『类型』侦测(存在与否)，如 test -e filename
    -e 该『⽂件名』是否存在？(常⽤)
    -f 该『⽂件名』是否为⽂件(file)？(常⽤)
    -d 该『⽂件名』是否为⽬录(directory)？(常⽤)
    -b 该『⽂件名』是否为⼀个 block device 装置？
    -c 该『⽂件名』是否为⼀个 character device 装置？
    -S 该『⽂件名』是否为⼀个 Socket ⽂件？
    -p 该『⽂件名』是否为⼀个 FIFO (pipe) ⽂件？
    -L 该『⽂件名』是否为⼀个连结档？

    2. 关于⽂件的权限侦测，如 test -r filename
    -r 侦测该⽂件名是否具有『可读』的属性？
    -w 侦测该⽂件名是否具有『可写』的属性？
    -x 侦测该⽂件名是否具有『可执⾏』的属性？
    -u 侦测该⽂件名是否具有『SUID』的属性？
    -g 侦测该⽂件名是否具有『SGID』的属性？
    -k 侦测该⽂件名是否具有『Sticky bit』的属性？
    -s 侦测该⽂件名是否为『⾮空⽩⽂件』？

    3. 两个⽂件之间的⽐较，如： test file1 -nt file2
    -nt (newer than)判断 file1 是否⽐ file2 新
    -ot (older than)判断 file1 是否⽐ file2 旧
    -ef 判断 file2 与 file2 是否为同⼀⽂件，可⽤在判断 hard link 的判定上。 主要意义在判定，两个⽂件是否均指向同⼀个 inode 哩！

    4. 关于两个整数之间的判定，例如 test n1 -eq n2
    -eq 两数值相等 (equal)
    -ne 两数值不等 (not equal)
    -gt n1 ⼤于 n2 (greater than)
    -lt n1 ⼩于 n2 (less than)
    -ge n1 ⼤于等于 n2 (greater than or equal)
    -le n1 ⼩于等于 n2 (less than or equal)

    5. 判定字符串的数据
    test -z string 判定字符串是否为 0 ？若 string 为空字符串，则为true
    test -n string 判定字符串是否⾮为 0 ？若 string 为空字符串，则为false。
    注： -n 亦可省略
    test str1 = str2 判定 str1 是否等于 str2 ，若相等，则回传 true
    test str1 != str2 判定 str1 是否不等于 str2 ，若相等，则回传false

    6. 多重条件判定，
    例如： test -r filename -a -x filename -a (and)两状况同时成⽴！例如 test -r file -a -x file，则 file 同时具有 r 与 x 权限时，才回传 true。
    -o (or)两状况任何⼀个成⽴！例如 test -r file -o -x file，则 file 具有 r 或 x 权限时，就可回传 true。
    ! 反相状态，如 test ! -x file ，当 file 不具有 x 时，回传 true
    ```
2. 中括号[  ]   
中括号两边的空格不能省略  
条件测试中使用变量必须使用双引号  
`&&` 前面一个不成立，不会运行后面的条件  
`||` 前面一个不成立，才会运行后面的条件   
双中括号 [[  ]]比单中括号功能强大，支持正则表达式  

- 文件测试表达式
 ![Alt text](./assert/%E6%96%87%E4%BB%B6%E6%B5%8B%E8%AF%95%E8%A1%A8%E8%BE%BE%E5%BC%8F.png)

- 字符串测试
![Alt text](./assert/%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%B5%8B%E8%AF%95.png)
`if test "$USER" = "$user"; then ` 
字符串的测试，⼀定要添加 **双引号**  
⽐较符号的两端，⼀定得有**空格**  
`!=` 和 `=` ⽤于⽐较两个字符串是否相同  

- 整数比较符测试
![Alt text](./assert/%E6%95%B4%E6%95%B0%E6%AF%94%E8%BE%83%E7%AC%A6%E6%B5%8B%E8%AF%95.png)
在`[]`中使用时大于小于符号需要添加转移符号，在`[[ ]]` 中不需要

- 逻辑操作符
  ![Alt text](./assert/%E9%80%BB%E8%BE%91%E6%93%8D%E4%BD%9C%E7%AC%A6.png)

- 条件测试总结
  ![Alt text](./assert/%E6%9D%A1%E4%BB%B6%E6%B5%8B%E8%AF%95%E6%80%BB%E7%BB%93.png)


## if语句
语法
```bash
if <条件表达式>
then
代码
fi

#简写
if <条件表达式>;then
代码
fi

#双分⽀
if <条件表达式>
 then
 if <条件表达式>
 then
 指令
 fi
fi

if <条件表达式>
 then
 代码1
else
 代码2
fi

if <条件表达式>
then
 代码1
elif <条件表达式>
then
 代码2
elif <条件表达式>
then
 代码3
else
 代码3
fi
```
`[]` `test` `[[]]` `(())` 都可以作为条件表达式

## 函数
定义函数
```bash
# 标准shell语法
function 函数名(){
 代码。。。
 return 返回值
}
# 简写，省去括号，不建议这么⽤
function 函数名{
 代码。。。
 return 返回值
}
# 更简化的写法，
函数名(){
 代码。。。
 return 返回值
}    
```