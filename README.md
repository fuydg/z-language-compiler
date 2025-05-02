# Z语言编译器 v1.2.1(Fix) 使用指南

📢 本项目支持自由修改和商用，欢迎二次创作！  
👨💻 原作者Telegram：`TG_KXDSJXSW` 或 `Funingna2333`

## 📂 文件结构说明
请按以下结构放置文件：
```
修复无法执行文件的问题
```

## 📂 文件结构说明
请按以下结构放置文件：
```
我的Z语言/
├── install.sh     # 安装脚本
├── ZL.cpp         # 编译器源码
├── zlang.h        # 头文件
├── build.sh       # 编译脚本
├── DEV.md         # 开发文档
└── ZL             # 已编译的主程序（安装后自动生成）
```


## 🛠️ 安装教程
### 1. 一键安装
复制以下命令到终端执行：
```bash
bash install.sh （部分Linux发行版需要带有sudo）
```

### 2. 安装后配置
- **临时使用**：  
  ```bash
  ./ZL demo.z
  ```
- **永久使用（推荐）**：  
  ```bash
  echo 'export PATH="$PATH:'$(pwd)'"' >> ~/.bashrc
  source ~/.bashrc
  # 之后可直接运行：ZL demo.z
  ```


## 🚀 快速体验
### 示例程序 `demo.z`
```z
> 你好世界示例
定义函数 斐波那契(项数):
    如果 项数 <= 1:
        返回 项数
    否则:
        返回 斐波那契(项数-1) + 斐波那契(项数-2)

打印("第10项是：" + 字符串(斐波那契(10)))   # 输出 55
```

运行命令：  
```bash
./ZL demo.z
```


## ❓ 常见问题
### Q1: Windows 如何使用？
1. 安装 WSL（Windows Subsystem for Linux）  
   在 PowerShell 中执行：
   ```powershell
   wsl --install
   ```
2. 按本文档步骤在 WSL 中操作即可

### Q2: 如何开发新程序？
1. 用文本编辑器创建 `.z` 文件，例如 `hello.z`：
   ```z
   打印("你好，群友！")
   ```
2. 运行程序：  
   ```bash
   ZL hello.z
   ```

### Q3: 编译失败怎么办？
- 检查是否安装 C++ 编译器和 Python3
- 查看 `build.log` 中的错误日志
- Termux 用户需执行：`pkg install clang python`


## 📜 开发说明
- 修改源码后重新编译：  
  ```bash
  g++ -std=c++17 -I. -O2 ZL.cpp -o ZL
  ```
- 更多细节请参考 `DEV.md`


```