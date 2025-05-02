#!/bin/bash
# 保存为 install.sh
# 设置安装路径为当前目录
INSTALL_DIR=$(pwd)

# 检测Termux环境
if [ -d "/data/data/com.termux/files" ]; then
    echo "[Termux环境检测]"
    PKG_MGR="pkg"
    PYTHON_PKG="python"
    COMPILER_PKG="clang"
else
    echo "[Linux环境检测]"
    PKG_MGR="sudo apt-get"
    PYTHON_PKG="python3"
    COMPILER_PKG="g++"
fi

# 安装依赖
check_install() {
    echo "正在安装基础依赖..."
    if command -v $PKG_MGR &>/dev/null; then
        $PKG_MGR update -y
        $PKG_MGR install -y $PYTHON_PKG $COMPILER_PKG
    else
        echo "错误：无法自动安装依赖"
        echo "请手动安装："
        echo "- Python3"
        echo "- C++编译器 (g++/clang)"
        exit 1
    fi
}

# 验证Python环境
if ! command -v python3 &>/dev/null && ! command -v python &>/dev/null; then
    check_install
fi

# 编译过程
echo "开始编译Z语言编译器..."
g++ -std=c++17 -I. -O2 ZL.cpp -o $INSTALL_DIR/ZL 2> build.log

# 结果验证
if [ -f "$INSTALL_DIR/ZL" ]; then
    echo "编译成功！请执行以下操作："
    echo "1. 添加执行权限："
    echo "   chmod +x $INSTALL_DIR/ZL"
    echo "2. 临时执行："
    echo "   $INSTALL_DIR/ZL demo.z"
    echo "3. 永久使用（推荐）："
    echo "   echo 'export PATH=\"\$PATH:$INSTALL_DIR\"' >> ~/.bashrc"
    echo "   source ~/.bashrc"
else
    echo "编译失败，请查看build.log"
fi
