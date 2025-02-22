#ifndef _TSP_HTTPOTA_H_
#define _TSP_HTTPOTA_H_

#include <rtthread.h>

#define HTTP_OTA_BUFF_LEN           4096
#define GET_HEADER_BUFSZ            1024
#define GET_RESP_BUFSZ              1024
#define HTTP_OTA_DL_DELAY           (10 * 1000)

#define HTTP_OTA_URL_MAX_LEN        (64)

#define HTTP_OTA_URL                "http://k3m35k.natappfree.cc/ota-v1.1.0.rbl"
#define HTTP_OTA_PART_NAME          "OTA"

enum{
    THTTP_OTA_FSM_NET_CONNECT = 0,
    THTTP_OTA_FSM_IDLE,
    THTTP_OTA_FSM_UPDATE_URL,
    THTTP_OTA_FSM_DOWNLOAD,
};

typedef struct tsp_httpota
{
    uint16_t http_buff_len;
    uint16_t http_get_header_buff_size;
    uint16_t http_resport_buff_size;
    uint16_t http_ota_download_delay;
    char http_ota_url[HTTP_OTA_URL_MAX_LEN];

    char ota_port_name[32];

    uint8_t ota_err_time;
    uint32_t last_check_ota_state;
    uint32_t last_check_net_time;
    uint32_t last_show_info_time;
    uint8_t ota_fsm;
}tsp_httpota_t;

void THttpOtaTaskCreate(tsp_httpota_t *self);

void THttpOtaTaskRunOnce(tsp_httpota_t *self);

void THttpOtaTaskDestory(tsp_httpota_t *self);

#endif
