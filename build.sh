#!/bin/bash
# 保存为 ./zl/build.sh 后执行

# 进入工作目录
cd "$(dirname "$0")"

# 快速编译命令
g++ -std=c++17 -I. -O2 ZL.cpp -o ZL 2>&1 | grep -i error

# 结果提示
if [ -f ZL ]; then
    echo "编译成功！执行命令: ./zl/ZL 你的文件.z"
else
    echo "编译失败，请检查:"
    echo "1. 确保g++已安装"
    echo "2. 确认zl目录包含ZL.cpp和zlang.h"
fi
