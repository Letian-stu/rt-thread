#ifndef _TCLOUD_MQTT_H_
#define _TCLOUD_MQTT_H_

#include <rtthread.h>
#include "paho_mqtt.h"
#include "tcloud_attrsync.h"

enum{
    MQTTCLIENT_SESSION_IDLE = 0,
    MQTTCLIENT_SESSION_INIT,
    MQTTCLIENT_SESSION_SYNCATTR,
    MQTTCLIENT_SESSION_CONNECTING,
    MQTTCLIENT_SESSION_CONNECTED,
    MQTTCLIENT_SESSION_DISCONNECT,
};

#define MQTT_CLIEN_ID       "letian_dev_001"
#define MQTT_HOST_URL       "tcp://sh-1-mqtt.iot-api.com:1883"
#define MQTT_HOST_USERNAME  "4kd8xhlyqty47ril"
#define MQTT_HOST_PASSWORD  "vO5r4RDRoy"

#define MQTT_WILLMSG        "Goodbye!"


//下发
#define MQTT_SUBTOPIC_ATTRRESP      "attributes/response"        //接收属性上报的响应
#define MQTT_SUBTOPIC_ATTRPUSH      "attributes/push"            //接收属性获取的响应
#define MQTT_SUBTOPIC_EVENTRESP     "event/response/+"           //接收下发的属性
#define MQTT_SUBTOPIC_CMDSEND       "command/send/+"             //接收下发的命令
#define MQTT_SUBTOPIC_CMDRESP       "command/reply/response/+"   //接收命令回复的响应
#define MQTT_SUBTOPIC_SERIES        "attributes/series/response" //接收离线属性上报的响应	

#define MQTT_SUBTOPIC_CUSTOM        "data/custom/set"            //接收自定义数据下发

//上报
#define MQTT_SUBTOPIC_ATTRUP        "attributes"


typedef struct tcloud_recv_mag{
    uint32_t topic_len;
    char topic_name[64];
    uint32_t recv_len;
    char recv_data[512];
}tcloud_recv_mag_t;

typedef struct tcloud_user_data{
    rt_sem_t msg_sem;
    tcloud_recv_mag_t recv_msg;
}tcloud_user_data_t;

typedef struct tcloud_mqtt
{
    uint8_t mqtt_session_is_init;
    uint8_t mqttclient_session_state;
    MQTTClient mqtt_client;
    tcloud_user_data_t mqtt_user_data;

    tcloud_attrsync_t attr_table;

    char mqtt_host_url[64];

    char mqtt_username[32];
    char mqtt_password[32];

    char mqtt_client_id[10];

    uint32_t last_check_net_time;
    uint32_t last_show_info_time;
}tcloud_mqtt_t;

int TCloudMqttSessionIsConnect(tcloud_mqtt_t *self);

void TCloudMqttSessionCreate(tcloud_mqtt_t *self);

void TCloudMqttSessionRunOnce(tcloud_mqtt_t *self);

void TCloudMqttSessionDestory(tcloud_mqtt_t *self);

#endif
