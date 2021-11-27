# ESP32 CTF 清华校赛版本

## 目录说明

- thuctf：esp32项目本体
- docker：MQTT 未授权未认证的broker配置
- attachment：给选手的说明
- test：一些测试脚本
- wp：题目解析

## 编译方法

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