# 常见问题
>Unpacking failed:文件"C:\Program Files\Docker\Docker\com.docker.service"正由另一进程使用，因此该进程 无法访问此文件。
如果报了这个错误的话
把这个进程杀了  
`tasklist | findstr docker`  
`taskkill -PID UID -F`


# 换盘
>`wsl -l -v`
```
  NAME                   STATE           VERSION
* Ubuntu                 Running         2
  docker-desktop         Running         2
  docker-desktop-data    Running         2
```
>**将docker-desktop 和 wsl 都关闭**（很重要）   
先导出数据  
`wsl --export docker-desktop-data E:\docker-desktop-data.tar`  
注销  
`wsl --unregister docker-desktop-data`  
导入目标盘  
`wsl --import docker-desktop-data D:\docker\wsl d:\docker-desktop-data.tar --version 2`



# 资料
[将docker-desktop-data移除系统盘](https://www.jianshu.com/p/5abc8ca39215)  
[进程无法访问此文件](https://blog.csdn.net/qq_55675216/article/details/125433466)