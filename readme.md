# 虚拟磁盘系统

*虚拟磁盘,由内存模拟物理磁盘，详细功能参阅需求文档*

## 目录说明

### src: 源代码目录
- build:    编译工程目录
- tools:    工具类目录
- base:     基础类目录
- cmd:      命令类目录
- handle:   底层处理类
- start:    启动入口管理目录

### build: vs2017工程文件

## 运行方法

*需要先安装cmake*
1. 执行./buildcmake.sh
2. cmake CMakeLists.txt
3. 执行make
4. 执行./disk启动服务
