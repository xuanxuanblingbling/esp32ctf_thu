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

```python
[+] hardware task I : hit 1
[+] hardware task I : hit 2
[+] hardware task I : hit 3
[+] hardware task I : THUCTF{Ev3ryth1ng_st4rt_fr0m_GPIO_!!!}
```

### task2

- 题面：在GPIO18处构造出1w个上升沿
- 解法：用杜邦线将GPIO18与板子的TX相接，利用串口一直有数据输出，自动构造上升沿：

```python
[+] hardware task II : trigger 9491
[+] hardware task II : trigger 9971
[+] hardware task II : trigger 10085
[+] hardware task II : THUCTF{AuT0++_is_th3_r1ght_w4y_hhhhhh}
```

### task3

- 题目：在另一个串口寻找第三个flag
- 解法：分析代码，第二个串口的TX、RX分别为4、5号引脚，接到串口转换器，然后用串口工具查看即可（发的串口转换器芯片为CH340，Linux、OSX免驱，WIN10需要手动装驱动）

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

## 蓝牙题目

```
[+] bluetooth task I : Please change your bluetooth device name to uunpyagw

I (43491) GAP: [+] bluetooth task I : Device found: a8:e5:44:3d:db:2e
I (43511) GAP: [+] bluetooth task I : Found a target device, address a8:e5:44:3d:db:2e, name uunpyagw

bluetooth task I : THUCTF{b1u3t00th_n4me_a1s0_c4n_b3_An_aTT4ck_surfAce}
```

```
[+] bluetooth task II : BLE device name is jlprw
[+] bluetooth task II : Please find the second flag in the ADV package from this BLE device jlprw
```

```
Python 3.8.7 (tags/v3.8.7:6503f05, Dec 21 2020, 17:59:51) [MSC v.1928 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> bytes.fromhex('06096A6C70727710FD5448554354467B416456443437617D') 
b'\x06\tjlprw\x10\xfdTHUCTF{AdVD47a}
```

```
THUCTF{AdVD47a}
```

```
I (389661) GATT: GATT_WRITE_EVT, conn_id 0, trans_id 1, handle 42
I (389661) GATT: GATT_WRITE_EVT, value len 15, value :
I (389671) GATT: 54 48 55 43 54 46 7b 41 64 56 44 34 37 61 7d 
[+] bluetooth task III : THUCTF{AdVD47a}
[+] bluetooth task III : you can read the third flag this time
```

## MQTT

```
mqtt_app_start("mqtt://mqtt.esp32ctf.xyz");
```

https://mqttfx.jensd.de/index.php/download


```
[+] now task : MQTT
```


```
THUCTF{#_1s_God_in_MQTT}
```

```
root@vultr:~# nc -vvv  -l -p 80
nc: getnameinfo: Temporary failure in name resolution
Connection received on 221.218.140.166 18214
GET / HTTP/1.0
User-Agent: esp-idf/1.0 esp32
flag: THUCTF{attAck_t0_th3_dev1ce_tcp_r3cV_ch4nnel}

```

```
root@vultr:~# nc -vvv  -l -p 80
nc: getnameinfo: Temporary failure in name resolution
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