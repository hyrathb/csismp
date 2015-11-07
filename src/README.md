CSISMP
==========================

## 简介
        **2015 亚马逊·“种子杯”编程 PK 复赛试题 CSISMP**

            by lkx810, hyrathb, AHEADer in Hust UniqueStudio
        
## 使用方法
        
        设定编辑Config.txt, 运行SeedCup.exe 程序将监听字典序最小的网卡.
        **需要root权限, 对应网卡需要开启**

## 编译运行环境
        archlinux* 4.2.3-1-ARCH 
        gcc version 5.2.0 
        libevent 2.0.22-1 
        
        **编译用到的链接库**:
            POSIX Threading Library 和 libevent
        **编译参数**:
            -lpthread -levent
        **编译方法**:
            Makefile
        
        *注意:
            程序使用linux网络库,可以允许在所有linux平台上.
            其他linux平台可能需要正确链接libevent库.
        
## 目录结构



## 实现功能

#### 基本功能

        + 正确读取配置文件并输出配置文件
        + 监听字典序最小的网卡的 CSISMP 数据报文
        + 保持 CSISMP 报文会话
        + 解析 CSISMP 数据报文
        + CSISMP 报文错误识别
        + 构造 CSISMP 数据报文
        + 回应 CSISMP 报文
        + 定时发送 CSISMP 同步报文
    
#### 设计优化

        + libevent 事件响应 和 POSIX 多线程处理 实现异步多线程
        + 使用 c语言 和 raw_socket 高效地监听收发 CSISMP 报文
        + 对于初始化各种情况处理分析
        + 使用变长结构体成员等使程序除了多线程开销外尽可能节省内存花费
        
## 程序逻辑
        
        + 主程序使用 pthread 开启三个子线程:
            1. read :用于监听socket, 监听到socket可读, 开启新线程处理解析包  
            2. sync :用于每30s发送同步报文
            3. delay:用于每5s清理过期session


