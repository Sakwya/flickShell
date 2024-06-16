# flickShell
[![CMake](https://github.com/ecust-SE211/flickShell/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/ecust-SE211/flickShell/actions/workflows/cmake.yml)  

一个简单的Linux Shell实现，用于教学实践。
## 实现功能
- 命令执行
- 读取用户配置 `./flickshrc`
- 管道 `|`
- 重定向 `<` `>`
- 后台运行 `&`
- 命令补全 <kbd>tab</kbd>
- 内建命令
  - alias
  - cd
  - exit
  - export(未实现)
  - help
  - history
  - type
## 运行方法

### 使用CMake编译
```bash
mkdir build && cd build
cmake ..
make
```
如果需要安装，请继续执行
```bash
make install
```
需要管理员权限
### 设为默认Shell

打开`/etc/shells`  
在文件内加入`/usr/local/bin/flickShell`
```bash
chsh -s /usr/local/bin/flickShell
```


## 可能的问题

### 1. 没有readline库

Debian/Ubuntu 系统：

```bash
sudo apt-get install libreadline-dev  
```

Fedora 系统：

```bash
sudo dnf install readline-devel  
```

CentOS/RHEL 系统：

```bash
sudo yum install readline-devel  
```

### 2. 程序崩溃

目前设为默认Shell可能会导致部分软件无法使用，实测无法打开vscode。   
推测为没有实现一些内建指令导致的。  
※ __新版本尚未测试，不清楚是否会有这个问题__

## 关于
flickShell的命令执行与string处理部分参考了  
[ExpShell.cpp](https://github.com/z0gSh1u/expshell)
A simple shell for Linux.  
by z0gSh1u @ 2020-09  

Revised By Sakwya @ 2024-03  