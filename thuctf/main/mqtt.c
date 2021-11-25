#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "mqtt_client.h"

// char mqtt_flag_1[] = "flag{this_is_mqtt_flag1}"; // THUCTF{#_1s_God_in_MQTT}
// char mqtt_flag_2[] = "flag{this_is_mqtt_flag2}"; // THUCTF{attAck_t0_th3_dev1ce_tcp_r3cV_ch4nnel}
// char mqtt_flag_3[] = "flag{this_is_mqtt_flag3}"; // THUCTF{0ver_the_Air_y0u_c4n_a77ack_t0_1ntranet_d3v1ce}

char mqtt_flag_1[25] = {0};
char mqtt_flag_2[46] = {0};
char mqtt_flag_3[55] = {0};

char mqtt_flag_1_en[] = "\x01\x31\x9d\x1c\xc7\x80\x66\x1d\x01\xe3\x81\x7e\xaa\x90\x0a\x6e\x8a\xb5\xbc\xc1\xe0\x4e\xf3\x86\xcf\x2a\xa2\x11\xdf\xec\x8e\x56\x99\xac\x2e\x74\x77\x22\xdf\x5e\xa6\xd9\xd2\x7c\xbc\x24\x69\xca\xd5\x8c\xcf\xe4\x95\x96\xbf\x27\x06\x97\xe3\x4d\x32\x79\xf2\x2b";
char mqtt_flag_2_en[] = "\xdf\x91\x34\x02\x0a\xf2\xa5\xac\x5e\x61\x73\x53\x63\x6d\xd6\x2d\x63\x72\x7d\xfb\x5a\xa3\x02\x80\xf2\xf5\x91\xc7\xbc\x5b\xf0\xf0\x10\x49\xf8\x67\x63\x8f\xdc\x14\x91\xdf\x03\xe9\xcb\x7f\xaa\x49\x2e\xb2\xff\xf0\x2b\x66\x78\x81\x37\x01\xfd\x1a\x2b\xff\xe9\xc3";
char mqtt_flag_3_en[] = "\x6a\x4a\xde\xc4\x4c\x49\x58\x2e\x09\xc4\xe0\x4e\xdf\x9b\xcd\xb4\x55\x8f\x44\xa2\xca\x10\x31\x1b\x46\x1a\x5e\x66\xac\x67\x27\x07\x37\xae\x0c\x71\x91\x81\x2f\x50\x4b\x2b\xa2\x69\x6c\xad\xea\x8f\x10\xc3\x85\x2a\x2f\x62\xc3\xbb\xe3\xed\x31\x41\x15\x43\x5e\x99";

char topic_1[] = "/topic/flag1";
char topic_2[100] = {0};

void get_random(char * a, int b);

static void http_get_task(char * webserver,char * httpdata)
{
    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s;

    getaddrinfo(webserver, "80", &hints, &res);
    addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
    ESP_LOGI("network", "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));
    s = socket(res->ai_family, res->ai_socktype, 0);
    ESP_LOGI("network", "... allocated socket");
    connect(s, res->ai_addr, res->ai_addrlen);
    ESP_LOGI("network", "... connected");
    freeaddrinfo(res);
    write(s, httpdata, strlen(httpdata));
    close(s);
}


void mqtt_data_hander(int length,char * data){
    
    char l[10];
    char url[500] = {0};
    char out[500] = {0};
    char httpdata[500]={0};
    char flagdata[500]={0};
    char tag3[] = " [+] MQTT task III: ";
    sprintf(flagdata,"%s%s%s",mqtt_flag_2,tag3,mqtt_flag_3);

    int a = 46;

    char * p = strnstr(data,"?",length);
    if(p){
        int data_length = p - data;
        snprintf(l,length - data_length,"%s",p+1);
        a = atoi(l);
        length = data_length;
    }

    sprintf(url,"%.*s",length, data);

    char fmt[] = "GET / HTTP/1.0\r\n"
                 "User-Agent: esp-idf/1.0 esp32\r\n"
                 "flag: %s\r\n"
                 "\r\n";

    if( a < (int)(sizeof(mqtt_flag_2) + sizeof(tag3) - 1 ) ){
        memcpy(out,flagdata,a & 0xff);
        sprintf(httpdata,fmt,out);
        http_get_task(url,httpdata);
    }          
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI("mqtt", "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, "/topic/flag1", mqtt_flag_1, 0, 1, 0);
            printf("[+] MQTT task I: publish successful, msg_id=%d\n", msg_id);
            
            msg_id = esp_mqtt_client_subscribe(client, topic_2, 0);
            printf("[+] MQTT task II: subscribe successful, msg_id=%d\n", msg_id);

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI("mqtt", "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI("mqtt", "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI("mqtt", "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI("mqtt", "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI("mqtt", "MQTT_EVENT_DATA");
            printf("[+] MQTT task II: topic ->  %.*s\r\n", event->topic_len, event->topic);
            printf("[+] MQTT task II: data -> %.*s\r\n", event->data_len, event->data);
            mqtt_data_hander(event->data_len,event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI("mqtt", "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI("mqtt", "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD("mqtt", "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

esp_mqtt_client_handle_t client ;

static void mqtt_app_start(char * broker)
{
    char r[10]={0};
    get_random(r,6);
    sprintf(topic_2,"%s%s","/topic/flag2/",r);

    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = broker,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);

    while(1){
        printf("[+] MQTT task II: I send second flag to baidu\n");
        esp_mqtt_client_publish(client, topic_2, "www.baidu.com?46", 0, 1, 0);
        vTaskDelay(10000 / portTICK_RATE_MS);
    }
}