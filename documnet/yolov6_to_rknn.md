# yolov6 转 rknn rk3568量化部署流程
## 环境准备
1. yolov6环境
    - 拉取yolov6v3版本仓库  
        `git clone https://github.com/meituan/YOLOv6 --branch v3 --depth=1`
    - 根据 `requirement.txt` 文件创建yolov6 conda 环境
2. rknntoolkit环境
    - 在PC端拉取rknn-toolkit2仓库  
        `git clone https://github.com/rockchip-linux/rknn-toolkit2.git  --depth=1`
    - 根据 `rknn-toolkit2/doc/requirements_cp38-1.4.0.txt`文件创建rknn conda 环境(python==3.8,文件中有百度源换源路径，`-i` + 路径 很快)  
        如果安装包时报错，先将numpy单独安装后，在一起装requirement。
    - 根据 `rknn-toolkit2/packages/rknn_toolkit2-1.4.0_22dcfef4-cp38-cp38-linux_x86_64.whl` 安装rknntoolkit包。  
        `pip install rknn_toolkit2-1.4.0_22dcfef4-cp38-cp38-linux_x86_64.whl`
## yolov6源码修改
- 注释`YOLOv6/yolov6/models/effidehead.py/`第113、121、122行代码
  ```python
     for i in range(self.nl):
                b, _, h, w = x[i].shape
                l = h * w
                x[i] = self.stems[i](x[i])
                cls_x = x[i]
                reg_x = x[i]
                cls_feat = self.cls_convs[i](cls_x)
                cls_output = self.cls_preds[i](cls_feat)
                reg_feat = self.reg_convs[i](reg_x)
                reg_output = self.reg_preds[i](reg_feat)

                if self.use_dfl:
                    reg_output = reg_output.reshape([-1, 4, self.reg_max + 1, l]).permute(0, 2, 1, 3)
                    reg_output = self.proj_conv(F.softmax(reg_output, dim=1))

                # cls_output = torch.sigmoid(cls_output)
                cls_score_list.append(cls_output.reshape([b, self.nc, l]))
                reg_dist_list.append(reg_output.reshape([b, 4, l]))

            cls_score_list = torch.cat(cls_score_list, axis=-1).permute(0, 2, 1)
            reg_dist_list = torch.cat(reg_dist_list, axis=-1).permute(0, 2, 1)

            # pred_bboxes = dist2bbox(reg_dist_list, anchor_points, box_format='xywh')
            # pred_bboxes *= stride_tensor

  ```
- 修改`YOLOv6/yolov6/layers/common.py/`第36行代码
  ```python
  class Conv(nn.Module):
    '''Normal Conv with SiLU activation'''
    def __init__(self, in_channels, out_channels, kernel_size, stride, groups=1, bias=False):
        super().__init__()
        padding = kernel_size // 2
        self.conv = nn.Conv2d(
            in_channels,
            out_channels,
            kernel_size=kernel_size,
            stride=stride,
            padding=padding,
            groups=groups,
            bias=bias,
        )
        self.bn = nn.BatchNorm2d(out_channels)
        # self.act = nn.SiLU()
        self.act = nn.ReLU()

    def forward(self, x):
        return self.act(self.bn(self.conv(x)))

    def forward_fuse(self, x):
        return self.act(self.conv(x))
  ```
- 将`YOLOv6/yolov6/layers/common.py/`RepVGG模块替换为下面的RepVGG模块
    ```python
    class RepVGGBlock(nn.Module):
        '''RepVGGBlock is a basic rep-style block, including training and deploy status
        This code is based on https://github.com/DingXiaoH/RepVGG/blob/main/repvgg.py
        '''
        def __init__(self, in_channels, out_channels, kernel_size=3,
                    stride=1, padding=1, dilation=1, groups=1, padding_mode='zeros', deploy=False, use_se=False):
            super(RepVGGBlock, self).__init__()
            """ Initialization of the class.
            Args:
                in_channels (int): Number of channels in the input image
                out_channels (int): Number of channels produced by the convolution
                kernel_size (int or tuple): Size of the convolving kernel
                stride (int or tuple, optional): Stride of the convolution. Default: 1
                padding (int or tuple, optional): Zero-padding added to both sides of
                    the input. Default: 1
                dilation (int or tuple, optional): Spacing between kernel elements. Default: 1
                groups (int, optional): Number of blocked connections from input
                    channels to output channels. Default: 1
                padding_mode (string, optional): Default: 'zeros'
                deploy: Whether to be deploy status or training status. Default: False
                use_se: Whether to use se. Default: False
            """
            self.deploy = deploy
            self.groups = groups
            self.in_channels = in_channels
            self.out_channels = out_channels

            assert kernel_size == 3
            assert padding == 1

            padding_11 = padding - kernel_size // 2

            self.nonlinearity = nn.ReLU()

            if use_se:
                raise NotImplementedError("se block not supported yet")
            else:
                self.se = nn.Identity()

            if deploy:
                self.rbr_reparam = nn.Conv2d(in_channels=in_channels, out_channels=out_channels, kernel_size=kernel_size, stride=stride,
                                            padding=padding, dilation=dilation, groups=groups, bias=True, padding_mode=padding_mode)

            else:
                # self.rbr_identity = nn.BatchNorm2d(num_features=in_channels) if out_channels == in_channels and stride == 1 else None
                # self.rbr_dense = conv_bn(in_channels=in_channels, out_channels=out_channels, kernel_size=kernel_size, stride=stride, padding=padding, groups=groups)
                # self.rbr_1x1 = conv_bn(in_channels=in_channels, out_channels=out_channels, kernel_size=1, stride=stride, padding=padding_11, groups=groups)
                self.rbr_identity = weight_bn(in_channels) if out_channels == in_channels and stride == 1 else None
                self.rbr_dense = weight_conv_bn(in_channels=in_channels, out_channels=out_channels, kernel_size=kernel_size, stride=stride, padding=padding, groups=groups)
                self.rbr_1x1 = weight_conv_bn(in_channels=in_channels, out_channels=out_channels, kernel_size=1, stride=stride, padding=padding_11, groups=groups)

        def forward(self, inputs):
            '''Forward process'''
            if hasattr(self, 'rbr_reparam'):
                return self.nonlinearity(self.se(self.rbr_reparam(inputs)))

            if self.rbr_identity is None:
                id_out = 0
            else:
                id_out = self.rbr_identity(inputs)

            return self.nonlinearity(self.se(self.rbr_dense(inputs) + self.rbr_1x1(inputs) + id_out))

        def get_equivalent_kernel_bias(self):
            kernel3x3, bias3x3 = self._fuse_bn_tensor(self.rbr_dense)
            kernel1x1, bias1x1 = self._fuse_bn_tensor(self.rbr_1x1)
            kernelid, biasid = self._fuse_bn_tensor(self.rbr_identity)
            return kernel3x3 + self._pad_1x1_to_3x3_tensor(kernel1x1) + kernelid, bias3x3 + bias1x1 + biasid

        def _avg_to_3x3_tensor(self, avgp):
            channels = self.in_channels
            groups = self.groups
            kernel_size = avgp.kernel_size
            input_dim = channels // groups
            k = torch.zeros((channels, input_dim, kernel_size, kernel_size))
            k[np.arange(channels), np.tile(np.arange(input_dim), groups), :, :] = 1.0 / kernel_size ** 2
            return k
        
        def _pad_1x1_to_3x3_tensor(self, kernel1x1):
            if kernel1x1 is None:
                return 0
            else:
                return torch.nn.functional.pad(kernel1x1, [1, 1, 1, 1])

        # def _fuse_bn_tensor(self, branch):
        #     if branch is None:
        #         return 0, 0
        #     if isinstance(branch, nn.Sequential):
        #         kernel = branch.conv.weight
        #         running_mean = branch.bn.running_mean
        #         running_var = branch.bn.running_var
        #         gamma = branch.bn.weight
        #         beta = branch.bn.bias
        #         eps = branch.bn.eps
        #     else:
        #         assert isinstance(branch, nn.BatchNorm2d)
        #         if not hasattr(self, 'id_tensor'):
        #             input_dim = self.in_channels // self.groups
        #             kernel_value = np.zeros((self.in_channels, input_dim, 3, 3), dtype=np.float32)
        #             for i in range(self.in_channels):
        #                 kernel_value[i, i % input_dim, 1, 1] = 1
        #             self.id_tensor = torch.from_numpy(kernel_value).to(branch.weight.device)
        #         kernel = self.id_tensor
        #         running_mean = branch.running_mean
        #         running_var = branch.running_var
        #         gamma = branch.weight
        #         beta = branch.bias
        #         eps = branch.eps
        #     std = (running_var + eps).sqrt()
        #     t = (gamma / std).reshape(-1, 1, 1, 1)
        #     return kernel * t, beta - running_mean * gamma / std

        def _fuse_bn_tensor(self, branch):
            if branch is None:
                return 0, 0
            if len(branch) == 3:
                kernel = branch.conv.weight
            else:
                if not hasattr(self, 'id_tensor'):
                    input_dim = self.in_channels // self.groups
                    kernel_value = np.zeros((self.in_channels, input_dim, 3, 3), dtype=np.float32)
                    for i in range(self.in_channels):
                        kernel_value[i, i % input_dim, 1, 1] = 1
                    self.id_tensor = torch.from_numpy(kernel_value).to(branch.bn.weight.device)
                kernel = self.id_tensor
            weight = branch.weight.weight.data
            running_mean = branch.bn.running_mean
            running_var = branch.bn.running_var
            gamma = branch.bn.weight
            beta = branch.bn.bias
            eps = branch.bn.eps
            std = (running_var + eps).sqrt()
            t = (gamma / std).reshape(-1, 1, 1, 1)
            return weight.unsqueeze(-1).unsqueeze(-1).unsqueeze(-1) * kernel * t, weight * (
                        beta - running_mean * gamma / std)

        def switch_to_deploy(self):
            if hasattr(self, 'rbr_reparam'):
                return
            kernel, bias = self.get_equivalent_kernel_bias()
            self.rbr_reparam = nn.Conv2d(in_channels=self.rbr_dense.conv.in_channels, out_channels=self.rbr_dense.conv.out_channels,
                                        kernel_size=self.rbr_dense.conv.kernel_size, stride=self.rbr_dense.conv.stride,
                                        padding=self.rbr_dense.conv.padding, dilation=self.rbr_dense.conv.dilation, groups=self.rbr_dense.conv.groups, bias=True)
            self.rbr_reparam.weight.data = kernel
            self.rbr_reparam.bias.data = bias
            for para in self.parameters():
                para.detach_()
            self.__delattr__('rbr_dense')
            self.__delattr__('rbr_1x1')
            if hasattr(self, 'rbr_identity'):
                self.__delattr__('rbr_identity')
            if hasattr(self, 'id_tensor'):
                self.__delattr__('id_tensor')
            self.deploy = True
    ```
    
## pt -> onnx
```python
try:
    LOGGER.info('\nStarting to export ONNX...')
    export_file = args.weights.replace('.pt', '.onnx')  # filename
    with BytesIO() as f:
        torch.onnx.export(model, img, f, verbose=False, opset_version=12,
                          training=torch.onnx.TrainingMode.EVAL,
                          do_constant_folding=True,
                          input_names=['images'],
                          output_names=['outputs'])
        f.seek(0)
        # Checks
        onnx_model = onnx.load(f)  # load onnx model
        onnx.checker.check_model(onnx_model)  # check onnx model
        # Fix output shape
        if args.end2end and not args.ort:
            shapes = [args.batch_size, 1, args.batch_size, args.topk_all, 4,
                      args.batch_size, args.topk_all, args.batch_size, args.topk_all]
            for i in onnx_model.graph.output:
                for j in i.type.tensor_type.shape.dim:
                    j.dim_param = str(shapes.pop(0))
    if args.simplify:
        try:
            import onnxsim
            LOGGER.info('\nStarting to simplify ONNX...')
            onnx_model, check = onnxsim.simplify(onnx_model)
            assert check, 'assert check failed'
        except Exception as e:
            LOGGER.info(f'Simplifier failure: {e}')
    onnx.save(onnx_model, export_file)
    LOGGER.info(f'ONNX export success, saved as {export_file}')
except Exception as e:
    LOGGER.info(f'ONNX export failure: {e}')
```
## onnx -> rknn (int8量化)
- 进入`rknn-toolkit2/examples/onnx/yolov5/` 复制一份 `test.py` 并修改onnx模型路径和`dataset.txt`，并根据模型输出重写后处理预处理等操作。   
    [参考代码](https://github.com/LostmanMing/yolov6_rknn/tree/python)  
    [rknn api 文档地址](https://github.com/rockchip-linux/rknn-toolkit2/tree/master/doc)
- rknn主要配置如下：
    ```python
    rknn.config(mean_values=[[0, 0, 0]], std_values=[[255, 255, 255]], target_platform='rk3568', quantized_algorithm='normal')
    ``` 
    连板测试精度配置（结果误差太大时进行，需要将板子与 pc 使用 usb 连接，并可以获取 `device_id`）  
    连板功能需要手动开启 `rknn_server`,具体操作见 [npu文档](https://github.com/rockchip-linux/rknpu2/blob/master/rknn_server_proxy.md) (注：可以ssh连板后无需使用串口)
    ```python
    rknn.list_devices() # 获取device_id
    ret = rknn.init_runtime(target='rk3568',device_id='f087bc7dc513cb4f')
    # 精度分析工具
    rknn.accuracy_analysis(inputs=[img],output_dir="./snapshot_rk",target='rk3568',device_id='f087bc7dc513cb4f')
    ```
    > 注： 连板和不连板各运行一次，查看保存下来的 error_analysis.txt 中 runtime_error 误差具体在哪出现，再对模型进行修改，可以将模型结果在误差出现前就返回，后面的操作全部写在后处理中。