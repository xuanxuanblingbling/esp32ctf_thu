#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "hardware.c"
#include "bluetooth.c"
#include "network.c"
#include "mqtt.c"

#include "connect.c"
#include "mode.c"


void app_main(void)
{   
    // init
    nvs_init();
    flag_init();
    checkmode_gpio_setup();
    hardware_gpio_setup();
    hardware_uart_setup();

    if(check()){

        printf("[+] now task : hardware, bluetooh, network\n");

        // hardware THUCTF{DuMp_the_b1n_by_espt00l.py_Ju5t_1n_0ne_Lin3} flag{you_will_never_kown_this_flag}
        xTaskCreate(hardware, "THUCTF{DuMp_the_b1n_by_espt00l.py_Ju5t_1n_0ne_Lin3}", 2048, NULL, 10, NULL);

        // bluetooth
        bt_app_gap_start_up();
        xTaskCreate(bt_loop, "bt_loop", 2048, NULL, 10, NULL);
        
        // network
        network_init();
        xTaskCreate(network_tcp, "network_tcp", 2048, NULL, 10, NULL);
        xTaskCreate(network_http, "network_http", 2048, NULL, 10, NULL);
        xTaskCreate(network_wifi, "network_wifi", 2048, NULL, 10, NULL);

    }else{

        printf("[+] now task : MQTT\n");

        // MQTT
        connect_wifi("Huawei@123","dcba1347");
        mqtt_app_start("mqtt://mqtt.esp32ctf.xyz");
    }
}

//➜   python ~/Desktop/esp/esp-idf2/components/esptool_py/esptool/esptool.py --baud 115200 --port /dev/tty.usbserial-14420 read_flash 0x10000 0x310000 dump.bin