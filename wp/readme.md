# ESP32 CTF 清华校赛版本

## 硬件题目

> main/hardware.c

题目顺序开启

```
task1 -> task2 -> task3
```

### task1

- 题面：将GPIO18抬高，持续3s即可获得flag
- 解法：用杜邦线将GPIO18与3.3v或5v相接


![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174914261.png?raw=true)

```python
[+] hardware task I : hit 1
[+] hardware task I : hit 2
[+] hardware task I : hit 3
[+] hardware task I : THUCTF{Ev3ryth1ng_st4rt_fr0m_GPIO_!!!}
```

### task2

- 题面：在GPIO18处构造出1w个上升沿
- 解法：用杜邦线将GPIO18与板子的TX相接，利用串口一直有数据输出，自动构造上升沿：

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174841886.png?raw=true)

```python
[+] hardware task II : trigger 9491
[+] hardware task II : trigger 9971
[+] hardware task II : trigger 10085
[+] hardware task II : THUCTF{AuT0++_is_th3_r1ght_w4y_hhhhhh}
```

### task3

- 题目：在另一个串口寻找第三个flag
- 解法：分析代码，第二个串口的TX、RX分别为4、5号引脚，接到串口转换器，然后用串口工具查看即可（发的串口转换器芯片为CH340，Linux、OSX免驱，WIN10需要手动装驱动）

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174858734.png?raw=true)

```
Xshell 7 (Build 0090)
Copyright (c) 2020 NetSarang Computer, Inc. All rights reserved.

Type `help' to learn how to use Xshell prompt.
[C:\~]$ 

Connecting to COM6...
Connected.

THUCTF{UART_15_v3ry_imp0r7ant_1n_i0T}
```

## 网络题目

### task1

- 题目：连接板子目标端口，尝试获得flag
- 解法：首先要按照板子要求构造出wifi热点，然后连接板子的3333端口并发送getflag即可

```
[+] network task I: I will connect a wifi -> ssid: fmnlso , password glttosvt 
I (88071) esp_netif_handlers: sta ip: 192.168.43.19, mask: 255.255.255.0, gw: 192.168.43.1
I (88071) wifi connect: got ip:192.168.43.19
I (88071) wifi connect: connected to ap SSID:fmnlso password:glttosvt
I (88081) network: Socket created
I (88081) network: Socket bound, port 3333
I (88091) network: Socket listening
```

```
$ nc 192.168.43.19 3333
getflag
THUCTF{M4k3_A_w1rele55_h0t5p0ts}
```

### task2

- 题目：你知道他发给百度的flag么
- 解法：如果用手机构造热点不方便抓包，故用win或者mac的网络共享开启热点，然后对共享网络的网卡抓包即可

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174710945.png?raw=true)

### task3

- 题目：flag在空中
- 解法：使用kali以及外置网卡抓802.11裸包，即可看到有flag的报文


![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174659640.png?raw=true)

```
➜  airmon-ng start wlan0 
➜  airodump-ng wlan0mon
```

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174624863.png?raw=true)


如果是MAC可直接使用内置网卡抓包：

```
➜  sudo /System/Library/PrivateFrameworks/Apple80211.framework/Versions/A/Resources/airport -s
➜  sudo /System/Library/PrivateFrameworks/Apple80211.framework/Versions/A/Resources/airport en0 sniff 1 
```

## 蓝牙题目


### task1

- 题目：修改蓝牙名称并设置可被发现即可获得flag
- 解法：如题

```
[+] bluetooth task I : Please change your bluetooth device name to uunpyagw

I (43491) GAP: [+] bluetooth task I : Device found: a8:e5:44:3d:db:2e
I (43511) GAP: [+] bluetooth task I : Found a target device, address a8:e5:44:3d:db:2e, name uunpyagw

bluetooth task I : THUCTF{b1u3t00th_n4me_a1s0_c4n_b3_An_aTT4ck_surfAce}
```

### task2

- 题目：flag在空中
- 解法：通过第一关后，板子会由经典蓝牙切换到低功耗蓝牙，flag就在BLE的广播报文中，使用手机软件nRF connect即可获得：


```
[+] bluetooth task II : BLE device name is jlprw
[+] bluetooth task II : Please find the second flag in the ADV package from this BLE device jlprw
```

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174752464.png?raw=true)

```
Python 3.8.7 (tags/v3.8.7:6503f05, Dec 21 2020, 17:59:51) [MSC v.1928 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> bytes.fromhex('06096A6C70727710FD5448554354467B416456443437617D') 
b'\x06\tjlprw\x10\xfdTHUCTF{AdVD47a}
```

```
THUCTF{AdVD47a}
```

### task3


- 题目：分析GATT业务并获得flag
- 解法：连接此BLE，并对id为0xff的service写入task2的flag，再次读取即可获得flag


![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174742131.png?raw=true)


```
I (389661) GATT: GATT_WRITE_EVT, conn_id 0, trans_id 1, handle 42
I (389661) GATT: GATT_WRITE_EVT, value len 15, value :
I (389671) GATT: 54 48 55 43 54 46 7b 41 64 56 44 34 37 61 7d 
[+] bluetooth task III : THUCTF{AdVD47a}
[+] bluetooth task III : you can read the third flag this time
```

## MQTT

拔掉跳冒以切换方向，可以看到日志：

```
[+] now task : MQTT
```

阅读源码可以看到MQTT连接了一个域名（花了一块钱买了一年）：

```c
mqtt_app_start("mqtt://mqtt.esp32ctf.xyz");
```

这个域名对应的服务器上启了一个为未授权未认证的MQTT broker，也就是本项目中的那个docker。对于MQTT的收发包，推荐工具：[MQTT.fx](https://mqttfx.jensd.de/index.php/download)


### task1 

- 题目：分析GATT业务并获得flag
- 解法：可以直接连接broker，井号为通配符，直接订阅所有主题，即可获得flag

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127175028179.png?raw=true)

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127175038978.png?raw=true)

```
THUCTF{#_1s_God_in_MQTT}
```

### task2 

- 题目：分析GATT业务并获得flag
- 解法：向flag2目标主题发送伪造IP即可

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127175056665.png?raw=true)


```
root@vultr:~# nc -vvv  -l -p 80
Connection received on 221.218.140.166 18214
GET / HTTP/1.0
User-Agent: esp-idf/1.0 esp32
flag: THUCTF{attAck_t0_th3_dev1ce_tcp_r3cV_ch4nnel}

```
### task3

- 题目：分析GATT业务并获得flag
- 解法：判断长度时有符号，比较时相当于无符号，故长度为-1即可绕过大小限制，带出位于flag2后的flag3


![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127175117275.png?raw=true)



```
root@vultr:~# nc -vvv  -l -p 80
Connection received on 221.218.140.166 18263
GET / HTTP/1.0
User-Agent: esp-idf/1.0 esp32
flag: THUCTF{attAck_t0_th3_dev1ce_tcp_r3cV_ch4nnel} 
[+] MQTT task III: THUCTF{0ver_the_Air_y0u_c4n_a77ack_t0_1ntranet_d3v1ce}

```

## 固件彩蛋

```
➜   python ~/Desktop/esp/esp-idf2/components/esptool_py/esptool/esptool.py --baud 115200 --port /dev/tty.usbserial-14420 read_flash 0x10000 0x310000 dump.bin
```

```
esptool.py --baud 115200  read_flash 0x10000 0x310000 dump.bin
```

```
$ sudo apt install binutils
$ strings ./dump.bin  | grep "THUCTF{"
THUCTF{DuMp_the_b1n_by_espt00l.py_Ju5t_1n_0ne_Lin3}
```