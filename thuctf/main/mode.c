#include <stdio.h>
#include <string.h>
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_system.h"
#include "esp32/aes.h"

void get_random(char * output,int number){
    for(int i = 0; i<number ;i++){
        char a = esp_random() % 26 + 97;
        output[i] = a;
    }
}

void checkmode_gpio_setup(){
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = ((1ULL<<23) );
    io_conf.mode = GPIO_MODE_INPUT_OUTPUT;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    gpio_config(&io_conf);
}

int check(){
    gpio_set_level(23,1);
    vTaskDelay(1000 / portTICK_RATE_MS);
    return gpio_get_level(23) ? 0 : 1 ;
}

void decrypt_flag(char * c,char *m)
{
    char plaintext[64]={0};
    char encrypted[64]={0};

    memcpy(encrypted,c,64);

	uint8_t key[32]= "\x36\x48\xb4\x4b\x70\x3b\x7d\x35\xd4\xb2\x1b\x7a\xd9\xb7\xd0\xf4"
                     "\x5f\x3f\x1c\x74\x4e\xe6\xc9\x12\xe4\x24\xe3\x0d\xd8\x06\x92\x4a" ;

	uint8_t iv[16]=  "\x06\x28\x62\x69\x0f\x3c\x2f\x2f\xef\x35\xec\xff\xaf\x0a\x22\x59" ;

	esp_aes_context ctx;
	esp_aes_init( &ctx );
	esp_aes_setkey( &ctx, key, 256 );
	esp_aes_crypt_cbc( &ctx, ESP_AES_DECRYPT, sizeof(encrypted), iv, (uint8_t*)encrypted, (uint8_t*)plaintext );
    
    sprintf(m,"%s",plaintext);
	esp_aes_free( &ctx );
}

void flag_init(){
    decrypt_flag(hardware_flag_1_en,hardware_flag_1);
    decrypt_flag(hardware_flag_2_en,hardware_flag_2);
    decrypt_flag(hardware_flag_3_en,hardware_flag_3);

    decrypt_flag(mqtt_flag_1_en,mqtt_flag_1);
    decrypt_flag(mqtt_flag_2_en,mqtt_flag_2);
    decrypt_flag(mqtt_flag_3_en,mqtt_flag_3);

    decrypt_flag(bt_flag_1_en,bt_flag_1);
    decrypt_flag(bt_flag_2_en,bt_flag_2);
    decrypt_flag(bt_flag_3_en,bt_flag_3);

    decrypt_flag(network_flag_1_en,network_flag_1);
    decrypt_flag(network_flag_2_en,network_flag_2);
    decrypt_flag(network_flag_3_en,network_flag_3);
}