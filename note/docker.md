> # ***Docker***

- [x] Docker常用命令
- [ ] Dockerfile
> ## **Docker常用命令**
 1. ### **帮助类启动命令**
    - *启动docker*  
    `systemctl start docker`
    - *停止docker*  
    `systemctl stop docker`
    - *重启docker*  
    `systemctl restart docker`
    - *查看docker状态*  
    `systemctl status docker`
    - *开机启动*  
    `systemctl enable docker`
    - *查看docker概要信息*  
    `docker info`
    - *查看docker总体帮助文档*  
    `docker --help`
    - *查看docker命令帮助文档*  
    `docker 具体命令 --help`   
1. ### **镜像命令**
    - *查看本地镜像*  
        `docker images`  
        ```
        -a:列出本地所有的镜像（含历史镜像层）
        -q:只显示镜像ID
        ```
    - *搜索镜像*  
        `docker search [OPTIONS] 镜像名字/id`
        ```
        --limit:只列出N个镜像，默认25个
        ```
    - *从仓库拉取镜像*  
        `docker pull 镜像名字/id :[TAG]`
        ```
        不写TAG默认拉去最新版，等价于docker pull 镜像名字:latest
        ```
    - *查看镜像/容器/数据卷所占的空间*  
        `docker system df `
    - *删除镜像*  
        `docker rmi 镜像名字/id`   
        |  操作 |  命令  |  
        | :-----------: | ----------- |  
        | 删除单个 | `docker rmi -f 镜像id` |  
        | 删除多个 | `docker rmi -f 镜像id1:TAG  镜像id2：TAG ...`
        | 删除全部 | `docker rmi -f $(docker image -qa)`
2. ### **容器命令**
    - *创建容器*  
    `docker run [OPTIONS] image [COMMAND] [ARGS...]` 
        | OPTIONS    |    <center>作用   | 
        | :-----------: | ----------- |
        | `--name` |  为容器指定一个名称 |
        | `-d`     | 后台运行容器并返回容器id，也即启动守护式容器（后台运行）|
        | `-i`     | 以交互模式运行容器，通常与`-t`一起使用 |
        | `-t `    | 为容器重新分配一个伪输入终端。通常与`-i`一起使用，也即启动交互式容器（前台有伪终端等待交互） |
        | `-P`     | 随机端口映射，大写P | 
        | `-p `    | 指定端口映射，小写p |
        | `--gpus` |   在容器中能够使用的gpu，一般 `--gpus=all` |
        | `-net`   | host模式：使用 `--net=host` 指定。（使用本地主机IP）<br>none模式；使用 `--net=none` 指定。<br>bridge模式：使用 `--net=bridge` 指定，默认设置。<br>container模式：使用 `--net=container:NAME_or_ID` 指定 |  
        | `--restart` |   `--restart=no` 默认值，表示容器退出时，docker不自动重启容器<br>`--restart=on-failure:n `若容器的退出状态非0，则docker自动重启容器，还可以指定重启次数n，若超过指定次数未能启动容器则放弃<br>`--restart=always` 容器退出时总是重启<br>`--restart=unless-stopped` 容器退出时总是重启，但不考虑Docker守护进程启动时就已经停止的容器
    - *查看容器*   
    `docker ps [OPTIONS]`
        | OPTIONS    |    <center>作用   |
        | :-----------: | ----------- | 
        | `-a` | 列出当前所有正在运行的容器和历史上运行过的 |
        | `-l` | 显示最近创建的容器 |
        | `-n` | 显示最近创建的n个容器 |
        | `-q` | 静默模式，只显示容器编号 |
    - *启动停止运行的容器*  
    `docker start 容器名/ID`
    - *重启容器*   
    `docker restart 容器名/ID` 
    - *停止容器*  
    `docker stop 容器名/ID` 
    - 强制停止容器  
    `docker kill 容器名/ID` 
    - *进入正在运行的容器并以命令行交互*  
        1. `docker exec -it 容器名/ID bashshell `  
            ```
            exec 是在容器中打开新的终端，并且可以启动新的进程，用exit退出，不会导致容器的停止
            ```
        2. `docker attach 容器名/ID`  
            ```
            attach直接进入容器启动命令的终端，不会启动新的进程，用exit退出会导致容器的停止
            ```      
    - *两种退出容器方式*
        | 方式 | <center>效果 |
        | :----: | ---- | 
        |`exit`         | run进入容器，exit退出，容器停止 |
        |`ctrl`+`p`+`q` | run进入容器，ctrl+p+q退出，容器不停止|
    - *删除已停止的容器*  
    `docker rm 容器名/ID` 
    - *查看容器内部细节*  
    `docker inspect 容器名/ID` 
    - *查看容器日志*  
    `docker logs 容器名/ID`  
        进不去容器可以查看日志报错信息
    - *查看容器内运行的进程*  
    `docker top 容器名/ID` 
    - *容器与主机文件拷贝*  
    `docker cp 容器名/ID:path 本地主机path`  
    `docker cp 本地主机path 容器名/ID:path`
    - *导出容器的内容作为一个tar归档文件（对应import命令）*  
    `docker export 容器名/ID > 文件名.tar` 
    - *从tar包中创建一个新的文件系统再导入为镜像（对应export命令）*
    `cat 文件名.tar | docker import - 镜像用户/镜像名:TAG` 