># Ubuntu 系统安装

## 网站
- [ubuntu 官方镜像地址](https://cn.ubuntu.com/download/desktop)
- [cmake下载地址](https://cmake.org/download/)
- [hiredis下载地址](https://github.com/redis/hiredis)
- [redis++下载地址](https://github.com/sewenew/redis-plus-plus)
- [redis.conf下载地址](https://redis.io/topics/config/)
## 系统内设置
1. 中文输入法
     `sudo apt-get install ibus-pinyin`  
     安装完成，进入“设置”，点击“键盘”，添加“输入源”；（有的安装完成输入法后，会默认添加中文输入法）  
     单击“汉语”，选择“中文（智能拼英）”  
     中英文输入法快捷键 `shift/win + space`
2. c++环境包安装
    ```bash
    # 安装编译器
    sudo apt install gcc g++ build-essential 

    # 安装cmake
    不推荐 sudo apt install cmake 版本太老 

    - 检查是否有cmake  
    which cmake

    - 安装openssl
    sudo apt-get install libssl-dev

    使用`curl` 或 `wget` 下载，或直接下载cmake安装包

    - 解压
    tar -zxvf cmake-xxx.tar.gz

    - 检查
    cd cmake-xxx  && ./bootstrap

    - 编译构造并安装
    make -j8 \
    sudo make install

    - 查看版本
    cmake --version
    ```

1. 阿里云一键安装docker
    ```bash
    curl -fsSL https://get.docker.com -o get-docker.sh

    sudo sh get-docker.sh --mirror=Aliyun

    sudo systemctl start docker //启动docker

    sudo systemctl enable docker //开机启动docker

    # 设置stable存储库和GPG密钥：
    distribution=$(. /etc/os-release;echo $ID$VERSION_ID) \
        && curl -s -L https://nvidia.github.io/nvidia-docker/gpgkey | sudo apt-key add - \
        && curl -s -L https://nvidia.github.io/nvidia-docker/$distribution/nvidia-docker.list | sudo tee /etc/apt/sources.list.d/nvidia-docker.list

    sudo apt-get update

    # 安装nvidia-docker
    sudo apt-get install -y nvidia-docker2

    # 设置默认运行时后，重新启动Docker守护程序以完成安装：
    sudo systemctl restart docker

    #测试nvidia-docker是否安装成功
    sudo docker run --rm --gpus all nvidia/cuda:11.0-base nvidia-smi
    ```
3. redis
    ```bash
    # 安装 hiredis
    git clone https://github.com/redis/hiredis.git

    cd hiredis

    make

    make install

    # 安装 redis++
    git clone https://github.com/sewenew/redis-plus-plus.git

    cd redis-plus-plus

    mkdir build

    cd build

    cmake ..

    make

    make install

    cd ..

    测试
    ./build/test/test_redis++ -h host -p port -a auth

    # 修改redis.conf
    1. 开启redis验证 （可选）
        requirepass xxx

    2. 注释掉 bind 127.0.0.1 （*必做）
        # bind 127.0.0.1

    3. 将daemonize yes注释掉或改成 daemonize no ,该配置与docker run -d 冲突，会导致容器启动失败 （*必做）
        daemonize no

    4.redis数据持久化 （可选）
        appendonly yes
    
    # docker 启动redis
        sudo docker run -p 6379:6379 --name redis --privileged=true -v /app/redis/redis.conf:/etc/redis/redis.conf -v /app/redis/data:/data -d redis redis-server /etc/redis/redis.conf
    
    redis++ 编译时 需要链接 lhiredis lredis++ pthread 库
    ```