# ***煤矿***
## **使用阿里云安装 docker**
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

## **拉取triton镜像**
```docker
docker pull nvcr.io/nvidia/deepstream:6.1-triton 
docker run -it --net=host --gpus=all -v /newdisk:/home/ --restart=always --name meikuang nvcr.io/nvidia/deepstream:6.1-triton
```

## **容器内配置**
1. 在/workspace目录下创建triton_deploy/models文件夹作为模型仓库
2. 克隆tensorrtx到/opt/nvidia/  (一下步骤适用于yolov5模型)

    - `git clone https://github.com/wang-xinyu/tensorrtx.git`   
    - `git checkout yolov5-v6.0`

3. 在/tensorrtx/yolov5/yolov5.cpp中  
   1. 修改第15行 `BATCH_SIZE` 为 16
   2. 在第100行添加：   
    ```cpp
    auto optimizer = builder->createOptimizationProfile();  
    optimizer->setDimensions("data",OptProfileSelector::kOPT,Dims4{16,3,640,640});
    ```
4. 在 tensorrtx/yolov5/yololayer.h中  
   修改20行 `CLASS_NUM` 为 1（转face时） 2 (转helmet和mask时)
5. /tensorrtx/yolov5/目录下创建build文件夹  
   - 将需要转换的wts文件cp到build中
   - `cmake .. && make -j8`  
   - 模型转换操作，以人脸检测为例：  
    `./yolov5 -s yolov5s-face.wts yolov5s.face.engine s `
6. 将生成的engine文件改名为model.plan并放到对应模型的1目录下
7. 在/opt目录下  
    ```bash
    git clone https://github.com/PaddlePaddlePaddleDetection.git

    cd PaddleDetection

    pip install paddlepaddle-gpu==2.2.2 -i https://mirror.baidu.com/pypi/simple

    pip install onnx onnxruntime onnxruntime-gpu  nvidia-pyindex onnx-graphsurgeon paddle2onnx -i https://mirror.baidu.com/pypi/simple

    #行人检测模型转换
    python tools/export_model.py -c configs/pphuman/ppyoloe_crn_s_36e_pphuman.yml -o weights=https://paddledet.bj.bcebos.com/models/ppyoloe_crn_s_36e_crowdhuman.pdparams trt=True exclude_nms=True

    python deploy/third_engine/demo_onnx_trt/onnx_custom.py --onnx_file=output_inference/ppyoloe_crn_s_36e_pphuman/ppyoloe_crn_s_36e_pphuman.onnx --model_dir=output_inference/ppyoloe_crn_s_36e_pphuman/ --opset_version=11

    cd output_inference/ppyoloe_crn_s_36e_pphuman/

    /usr/src/tensorrt/bin/trtexec --onnx=ppyoloe_crn_s_36e_pphuman.onnx --workspace=5632 --saveEngine=ppyoloe_crn_s_36e_pphuman.engine --minShapes=image:1x3x640x640 --optShapes=image:16x3x640x640 --maxShapes=image:64x3x640x640 --verbose

    #打电话检测 由目标检测出人后对人进行分类
    下载 https://bj.bcebos.com/v1/paddledet/models/pipeline/PPHGNet_tiny_calling_halfbody.zip到任意路径 
    在 /PPHGNet_tiny_calling_halfbody 路径下 

    paddle2onnx --model_dir ./ --model_filename model.pdmodel --params_filename model.pdiparams --save_file model.onnx  --save_file model.onnx

    在生成onnx的路径下执行

    /usr/src/tensorrt/bin/trtexec --onnx=model.onnx --workspace=5632 --saveEngine=pphg224_t_calling.engine --minShapes=x:1x3x224x224 --optShapes=x:16x3x224x224 --maxShapes=x:64x3x224x224 --verbose 

    将生成的.engine模型移动到 /workspace/triton_deploy/models/pphg-tiny-call/1 重命名为 mode.plan 并替换现有的（如果存在） model.plan

    #抽烟检测 由目标检测出人后对人进行分类
    在PaddleDetection根目录下执行 python tools/export_model.py -c configs/pphuman/ppyoloe_crn_s_80e_smoking_visdrone.yml -o weights=https://bj.bcebos.com/v1/paddledet/models/pipeline/ppyoloe_crn_s_80e_smoking_visdrone.pdparams  trt=True exclude_nms=True

    然后执行

    python deploy/third_engine/demo_onnx_trt/onnx_custom.py --onnx_file=output_inference/ppyoloe_crn_s_80e_smoking_visdrone/ppyoloe_crn_s_80e_smoking_visdrone.onnx --model_dir=output_inference/ppyoloe_crn_s_80e_smoking_visdrone/ --opset_version=11

    在 output_inference/ppyoloe_crn_s_80e_smoking_visdrone/ 路径下如果存在onnx则直接执行该命令 
    
    /usr/src/tensorrt/bin/trtexec --onnx=ppyoloe_crn_s_80e_smoking_visdrone.onnx --workspace=5632 --saveEngine=ppyoloe_crn_s_80e_smoking_visdrone.engine --minShapes=image:1x3x640x640,scale_factor:1x2 --optShapes=image:16x3x640x640,scale_factor:16x2 --maxShapes=image:64x3x640x640,scale_factor:64x2 --verbose 
    
    将生成的.engine模型移动到/workspace/triton_deploy/models/ppyoloe-s-smoke/1重命名为mode.plan并替换现有的（如果存在）model.plan
    ```

## **服务器配置**
### 设置磁盘永久挂载
```bash
lsblk   #查看目录结构

mkfs -t ext4 /dev/sda1

sudo mount  /dev/sda1 /newdisk

df -h 

vim /etc/fstab

/dev/sda1 /newdisk ext4 default 0 2
```
### 若docker生成容器后才发现有些参数没有指定，可以使用 update  
- `sudo docker update --restart=always meikuang2.0`

### 测试ssh能否连上docker内

- `ssh root@172.0.0.1 -p 36022`