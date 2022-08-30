# ESP32 IoT CTF 清华校赛版

一个以ESP32为底座的新手向CTF IoT赛题，包括基本的硬件操作，串口调试，网络通信，WIFI，蓝牙，MQTT，固件提取等，总共13个flag。其实就是我们仨这几年学到的一些经验，以及海特西湖论剑那张板子上的部分思路。淼哥对此赛题评价是：没有一点弯，纯训练。欢迎大家来玩～

## 目录说明


| 目录       | 说明                          |
| ---------- | ----------------------------- |
| thuctf     | ESP32项目本体                 |
| docker     | 未授权未认证的MQTT broker镜像 |
| attachment | 给选手的说明                  |
| test       | 测试脚本                      |
| wp         | 题目解析                       |

## 编译方法

已验证的环境如下：

- 工具版本：ESP-IDF v4.2.2-250-gf65845ef51-dirty、ESP-IDF v4.3.1
- 板子型号：ESP32-WROOM-32D

首先按照官方文档在自己的开发环境上安装好IDF：[https://github.com/espressif/esp-idf](https://github.com/espressif/esp-idf)

```
➜  git clone https://github.com/xuanxuanblingbling/esp32ctf_thu.git
➜  cd esp32ctf_thu/thuctf/
➜  idf.py menuconfig 
➜  idf.py build 
➜  idf.py flash 
```

其中menuconfig设置：

```
Serial flasher config  --->  Flash size (4 MB) 
Partition Table        --->  Partition Table (Custom partition table CSV)
```