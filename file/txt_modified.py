import os
import re
# 路径
path = r'F:/EOC/coal/dataset/labels/'
# 文件列表
files = []
for file in os.listdir(path):
    if file.endswith(".txt"):
        files.append(path+file)
# 逐文件读取-修改-重写
for file in files:
    with open(file, 'r') as f:
        new_data = re.sub('^6', '0', f.read(), flags=re.MULTILINE)    # 将列中的6替换为0
    with open(file, 'w') as f:
        f.write(new_data)
