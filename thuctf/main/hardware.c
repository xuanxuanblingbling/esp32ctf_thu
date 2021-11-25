#include <stdio.h>
#include <string.h>
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#define GPIO_INPUT_IO_0     18
#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_IO_0) )
#define ESP_INTR_FLAG_DEFAULT 0

#define ECHO_TEST_TXD  (GPIO_NUM_4)
#define ECHO_TEST_RXD  (GPIO_NUM_5)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

int trigger = 0;

// char hardware_flag_1[] = "flag{this_is_hardware_flag1}"; // THUCTF{Ev3ryth1ng_st4rt_fr0m_GPIO_!!!}
// char hardware_flag_2[] = "flag{this_is_hardware_flag2}"; // THUCTF{AuT0++_is_th3_r1ght_w4y_hhhhhh}
// char hardware_flag_3[] = "flag{this_is_hardware_flag3}"; // THUCTF{UART_15_v3ry_imp0r7ant_1n_i0T}

char hardware_flag_1[39] = {0}; // 
char hardware_flag_2[39] = {0};
char hardware_flag_3[38] = {0};

char hardware_flag_1_en[]="\x2b\x21\x9a\xe7\x5e\xca\x6b\x94\xe6\x03\x70\x08\x0f\xc5\x72\x4e\xda\x01\x45\xb8\x0c\xb3\xe3\xb0\xd5\xf4\xd3\x87\x5e\xa2\x2b\x52\x70\x8b\x68\xa0\xa5\x39\x4a\xd2\x23\xb1\xfc\x1d\x11\x3a\x61\xb7\x5d\x12\xf8\x6b\x5e\x22\xd6\x51\x17\x15\xf7\x62\x24\xb1\xe1\x05";
char hardware_flag_2_en[]="\x23\x9d\xfa\x61\xa8\x39\xf7\x26\x2c\x91\x55\x8c\xbe\x14\x77\x07\xd2\x01\xbb\xe9\x06\x1e\xc0\xcc\xf4\xa2\xf0\x45\x16\x9d\x3e\xc3\x9c\x0d\xde\x32\x40\x31\xb8\x82\xce\x05\xa8\xcb\xb4\xda\x2a\x0e\xf4\x73\x85\x99\xa6\xd1\x5a\xc2\xa2\x46\xc2\xed\x25\xcc\xf7\x1c";
char hardware_flag_3_en[]="\xf5\x47\x50\x55\x62\xed\xd1\xe9\x32\xc9\xea\x24\x21\x21\x0d\xde\xcb\x94\x1e\x66\x7b\xa2\xb0\x18\x64\x53\x25\xe2\x1e\x69\x86\x91\x8d\x86\xd4\x18\x04\x08\xc3\x1c\x04\xf8\x0a\x47\xd2\x54\x1e\x77\xf5\xf7\xbf\x23\x57\x31\x40\x89\x9a\x0f\x67\xd8\x4c\xfd\x32\x9c";


static void IRAM_ATTR gpio_isr_handler(void* arg){
    trigger++;
}

void hardware_uart_setup(){
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM_1, 1024 * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
}

void hardware_gpio_setup(){
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_INPUT_IO_0, gpio_isr_handler, (void*) GPIO_INPUT_IO_0);
}


void hardware_task1(){
    int hit = 0;
    while(1) {
        printf("[+] hardware task I : hit %d\n",hit);
        if(gpio_get_level(GPIO_INPUT_IO_0)){
            hit ++ ;
        }else{
            hit = 0;
        }
        if(hit>3){
            printf("[+] hardware task I : %s\n",hardware_flag_1);
            break;
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void hardware_task2(){
    trigger = 0;
    while(1){
        printf("[+] hardware task II : trigger %d\n",trigger);
        if(trigger > 1000){
            printf("[+] hardware task II : %s\n",hardware_flag_2);
            break;
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void hardware_task3(){
    printf("[+] hardware task III : find the third flag in another UART\n");
    while (1) {
        uart_write_bytes(UART_NUM_1, hardware_flag_3, strlen(hardware_flag_3));
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void hardware(){
    hardware_task1();
    hardware_task2();
    hardware_task3();
}