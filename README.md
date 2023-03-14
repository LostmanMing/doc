># ***备忘、文档存档、笔记、命令等***
## 备忘 🗻
- 各种格式视频播放 [PotPlayer](http://potplayer.tv/?lang=zh_CN)
- 视频格式转换 [ffmpeg](https://www.ffmpeg.org/download.html#build-windows)
- [数据结构可视化网站](https://www.cs.usfca.edu/~galles/visualization/Algorithms.html)
- [git学习网站](https://learngitbranching.js.org/?locale=zh_CN)
- [git创建远程仓库](https://www.cnblogs.com/yanqingyang/p/9839664.html)
- [linux命令查询](https://wangchujiang.com/linux-command/)
- [cmake 学习网站](https://sfumecjf.github.io/cmake-examples-Chinese/)
## 文档 ✨
- [煤矿文档](/documnet/coal_config.md)
- [win下docker+wsl](documnet/docker_desktop换盘.md)
- [ubuntu 系统安装](documnet/ubuntu_installation.md)
  
## 笔记 🔥
- [docker笔记](note/docker.md)
- [MySQL笔记](note/mysql.md)
- [vim笔记](note/vim.md)
- [redis笔记](note/redis/Redis.md)
- [c++ 多线程笔记](note/MTthread.md)
- [设计模式](note/设计模式.md)

## 文件 🎉
- [视频裁剪脚本](file/video_cap.py)
- [视频转图片脚本](file/video_2_picture.py)
- [yolo格式标签修改脚本](file/txt_modified.py)
- [煤矿triton dockerfile](file/docker/dockerfile)
- [煤矿自动启动脚本](file/autostart.sh) 
## 命令 🏜️
- 实时查看文件尾部输出的日志
    ```bash
    `tail -f file`
    docker :
    `sudo docker logs -f --tail 20 sentinel_1`
    ```

- github代理，加速访问速度  
[设置代理](https://www.cnblogs.com/xuyaowen/p/linux-cmdline-http-https-tcp-proxy.html)
    ```bash
    # 临时设置代理：
    `export https_proxy=127.0.0.1:7890`
    `export http_proxy="127.0.0.1:7890`
    临时取消代理：
    unset http_proxy
    unset https_proxy
    查看代理：
    env | grep xxx
    # 如果想要使代理服务器永久生效，可以修改 /etc/profile文件 ，在profile 中添加或者删除。
    ```
- 将pip3环境中的包，添加到conda环境中
    ```bash
    # 在pip3环境中使用以下命令查看torch包的安装路径
    pip3 show torch
    #在输出中找到Location字段，记录下路径
    #在conda环境中使用以下命令添加路径到PYTHONPATH环境变量：
    export PYTHONPATH=$PYTHONPATH:~/.local/lib/python3.8/dist-packages
    #路径为show出的路径
    ```