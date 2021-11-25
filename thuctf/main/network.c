#include <string.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"

#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

static const char *TAG = "network";

// char network_flag_1[] = "flag{this_is_network_flag1}"; // THUCTF{M4k3_A_w1rele55_h0t5p0ts}
// char network_flag_2[] = "flag{this_is_network_flag2}"; // THUCTF{Sn1ffer_N3tw0rk_TrAffic_In_7h4_Main_r0aD}
// char network_flag_3[] = "flag{this_is_network_flag3}"; // THUCTF{Y0u_cAn_s3nd_4nd_sNiff3r_802.11_r4w_pAckag3}

char network_flag_1[33] = {0};
char network_flag_2[49] = {0};
char network_flag_3[52] = {0};


char network_flag_1_en[] = "\xe6\x62\xad\x8d\x58\xd8\x98\x6c\xe0\xb3\xf4\x4b\x75\xa8\xc9\x02\xcb\xbf\x33\xc3\x4f\xda\x76\x61\x39\x4c\x1f\x86\x5c\xcb\x21\x82\xf2\x18\xa8\xfe\xbc\x27\xfe\x06\x2d\xfd\xf7\x27\xf4\x83\xee\x1b\x49\xc9\xc1\x39\x93\x8e\x66\x8e\x11\x30\x0c\x49\x95\x27\xe8\x1c";
char network_flag_2_en[] = "\x5f\xcd\x46\xa9\x11\x96\xc5\xe6\xf7\x9e\xbb\xf8\x5d\x9c\x07\x11\xaf\x66\x42\x02\x2b\x7d\xd0\xfe\x54\x5b\x30\xff\xba\xaf\xed\xa2\x99\x58\x94\x16\xc8\x79\xcb\xb3\xb1\x8c\x98\x4f\xeb\xf3\xc0\x4b\x4e\xa0\x70\xe9\x5a\x9a\x70\x3d\x99\xba\xb1\x56\x85\xf1\x78\x6f"; 
char network_flag_3_en[] = "\xb6\x8e\xc4\x76\xf6\xe0\xe0\xd0\xae\x7a\x5a\xca\xa2\xd7\x96\x19\x88\xea\xba\x02\x88\xb1\x17\xf0\x42\x5a\x63\x7a\x4f\x1e\xff\x72\x25\x77\x1e\x77\x25\x10\xf9\x35\x6b\x3c\x34\xbf\xf0\xdf\x55\xa5\xf3\x97\x2e\x17\x2e\x76\x21\x16\xcc\x9e\x87\xc9\x06\x12\x9e\x7f";


int open_next_tasks = 0;

void get_random(char * a,int b);
void connect_wifi(char *a, char *b);

void network_init(){
    char ssid[0x10] = {0};
    char pass[0x10] = {0};
    get_random(ssid,6);
    get_random(pass,8);
    printf("[+] network task I: I will connect a wifi -> ssid: %s , password %s \n",ssid,pass);
    connect_wifi(ssid,pass);
}

static void network_tcp()
{
    
    char addr_str[128];
    struct sockaddr_in dest_addr;

    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(3333);

    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    ESP_LOGI(TAG, "Socket created");

    bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    ESP_LOGI(TAG, "Socket bound, port %d", 3333);

    listen(listen_sock, 1);
    while (1) {

        ESP_LOGI(TAG, "Socket listening");
        struct sockaddr_storage source_addr;
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);
        char buffer[100];
        while(recv(sock,buffer,0x10,0)){
            if(strstr(buffer,"getflag")){
                send(sock, network_flag_1, strlen(network_flag_1), 0);
                break;
            }else{
                send(sock, "error\n", strlen("error\n"), 0);
            }
            vTaskDelay(1000 / portTICK_RATE_MS);
        }
        open_next_tasks = 1;
        shutdown(sock, 0);
        close(sock);
    }
}

void network_http()
{
    char  fmt[]  = "GET / HTTP/1.0\r\n"
                        "Host: www.baidu.com:80\r\n"
                        "User-Agent: esp-idf/1.0 esp32\r\n"
                        "flag: %s\r\n"
                        "\r\n";
    
    char request[200];
    sprintf(request,fmt,network_flag_2);

    const struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_STREAM,
    };
    struct addrinfo *res;
    struct in_addr *addr;
    int s;

    while(1) {
        if(open_next_tasks){
            printf("[+] network task II : send the second flag to baidu\n");
            getaddrinfo("www.baidu.com", "80", &hints, &res);
            addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
            ESP_LOGI("network", "DNS lookup succeeded. IP=%s", inet_ntoa(*addr));
            s = socket(res->ai_family, res->ai_socktype, 0);
            connect(s, res->ai_addr, res->ai_addrlen);
            freeaddrinfo(res);
            write(s, request, strlen(request));
            close(s);
        }
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}


static void network_wifi()
{
    static const char ds2ds_pdu[] = {
    0x48, 0x03, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xE8, 0x65, 0xD4, 0xCB, 0x74, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x60, 0x94, 0xE8, 0x65, 0xD4, 0xCB, 0x74, 0x1C, 0x26, 0xB9,
    0x0D, 0x02, 0x7D, 0x13, 0x00, 0x00, 0x01, 0xE8, 0x65, 0xD4, 0xCB, 0x74,
    0x1C, 0x00, 0x00, 0x26, 0xB9, 0x00, 0x00, 0x00, 0x00,
    };  

    char pdu[200]={0};
    memcpy(pdu,ds2ds_pdu,sizeof(ds2ds_pdu));
    memcpy(pdu+sizeof(ds2ds_pdu),network_flag_3,sizeof(network_flag_3));

    while(1) {
        if(open_next_tasks){
            printf("[+] network task III : send raw 802.11 package contains the third flag\n");
            esp_wifi_80211_tx(ESP_IF_WIFI_STA, pdu, sizeof(ds2ds_pdu)+sizeof(network_flag_3), true);
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}