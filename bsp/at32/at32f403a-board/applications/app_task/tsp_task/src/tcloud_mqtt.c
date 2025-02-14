#include "tcloud_mqtt.h"
#include "at_device.h"
#include "blib.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "tcloud"
#endif
#define LOG_LVL     LOG_LVL_DBG  

#include <ulog.h>  

static void TCloudBaseInfoCfg(tcloud_mqtt_t *self)
{
    self->mqttclient_session_state = MQTTCLIENT_SESSION_IDLE;
    rt_strcpy(self->mqtt_host_url, MQTT_HOST_URL);
    rt_strcpy(self->mqtt_username, MQTT_HOST_USERNAME);
    rt_strcpy(self->mqtt_password, MQTT_HOST_PASSWORD);
    rt_strcpy(self->mqtt_client_id, MQTT_CLIEN_ID);
}

static void TCloudSubCb(MQTTClient *c, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt sub callback: %.*s %.*s",
               msg_data->topicName->lenstring.len,
               msg_data->topicName->lenstring.data,
               msg_data->message->payloadlen,
               (char *)msg_data->message->payload);
}

static void TCloudSubDefaultCb(MQTTClient *c, MessageData *msg_data)
{
    *((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
    LOG_D("mqtt sub default callback: %.*s %.*s",
               msg_data->topicName->lenstring.len,
               msg_data->topicName->lenstring.data,
               msg_data->message->payloadlen,
               (char *)msg_data->message->payload);
}

static void TCloudConnectCb(MQTTClient *c)
{
    LOG_D("inter TCloudConnectCb!");
}

static void TCloudOnlineCb(MQTTClient *c)
{
    LOG_D("inter TCloudOnlineCb!");
}

static void TCloudOfflineCb(MQTTClient *c)
{
    LOG_D("inter TCloudOnlineCb!");
}

static void ThingsCloudMqttCreate(tcloud_mqtt_t *self)
{
    MQTTPacket_connectData condata = MQTTPacket_connectData_initializer;
    MQTTClient *client = (MQTTClient *)&self->mqtt_client;
    /* config MQTT context param */
    client->isconnected = 0;
    client->uri = self->mqtt_host_url;

    /* config connect param */
    rt_memcpy(&client->condata, &condata, sizeof(condata));
    client->condata.clientID.cstring = self->mqtt_client_id;
    client->condata.keepAliveInterval = 30;
    client->condata.cleansession = 1;
    client->condata.username.cstring = self->mqtt_username;
    client->condata.password.cstring = self->mqtt_password;

    /* config MQTT will param. */
    client->condata.willFlag = 1;
    client->condata.will.qos = QOS0;
    client->condata.will.retained = 0;
    client->condata.will.topicName.cstring = MQTT_SUBTOPIC_SERIES;
    client->condata.will.message.cstring = MQTT_WILLMSG;

    /* rt_malloc buffer. */
    client->buf_size = client->readbuf_size = 1024;
    client->buf = rt_calloc(1, client->buf_size);
    client->readbuf = rt_calloc(1, client->readbuf_size);
    if (!(client->buf && client->readbuf))
    {
        LOG_E("no memory for MQTT client buffer!");
        return;
    }

    /* set event callback function */
    client->connect_callback = TCloudConnectCb;
    client->online_callback = TCloudOnlineCb;
    client->offline_callback = TCloudOfflineCb;

    /* set subscribe table and event callback */

    for(int i = 0; i < MAX_MESSAGE_HANDLERS; i++)
    {
        client->messageHandlers[i].callback = TCloudSubCb;
        client->messageHandlers[i].qos = QOS1;
    }

    client->messageHandlers[0].topicFilter = rt_strdup(MQTT_SUBTOPIC_ATTRRESP);
    client->messageHandlers[1].topicFilter = rt_strdup(MQTT_SUBTOPIC_ATTRPUSH);
    client->messageHandlers[2].topicFilter = rt_strdup(MQTT_SUBTOPIC_EVENTRESP);
    client->messageHandlers[3].topicFilter = rt_strdup(MQTT_SUBTOPIC_CMDSEND);
    client->messageHandlers[4].topicFilter = rt_strdup(MQTT_SUBTOPIC_CMDRESP);
    client->messageHandlers[5].topicFilter = rt_strdup(MQTT_SUBTOPIC_SERIES);

    /* set default subscribe event callback */
    client->defaultMessageHandler = TCloudSubDefaultCb;
}

int TCloudMqttSessionIsConnect(tcloud_mqtt_t *self)
{
    return self->mqtt_client.isconnected;
}

void ThingsCloudMqttSessionCreate(tcloud_mqtt_t *self)
{
    TCloudBaseInfoCfg(self);
    TcloudAttrSyncTableInit(&self->attr_table);
}

void ThingsCloudMqttSessionRunOnce(tcloud_mqtt_t *self)
{
    RUN_IF_ELAPSED(last_show_info_time, 30000)
    {
        LOG_D("ThingsCloudMqttSessionRunOnce %d", self->mqttclient_session_state);
        SYNC_TIME(last_show_info_time);
    }
    switch(self->mqttclient_session_state)
    {
        case MQTTCLIENT_SESSION_IDLE:{
            //todo 添加时间判断
            RUN_IF_ELAPSED(last_check_net_time, 5000)
            {
                uint8_t link_state = 0;
                struct at_device *dev= at_device_get_first_initialized();
                at_device_control(dev, AT_DEVICE_CTRL_GET_SIGNAL, &link_state);
                if(link_state == 1)
                {
                    LOG_D("http net connect ok %d", link_state);
                    self->mqttclient_session_state = MQTTCLIENT_SESSION_INIT;
                }
                SYNC_TIME(last_check_net_time);
            }
        }
        break;
        case MQTTCLIENT_SESSION_INIT:{
            ThingsCloudMqttCreate(self);
            paho_mqtt_start(&self->mqtt_client);
            self->mqttclient_session_state = MQTTCLIENT_SESSION_SYNCATTR;
        }
        break;
        case MQTTCLIENT_SESSION_SYNCATTR:{
            TcloudAttrSyncTableCreate(&self->attr_table);
            if(TCloudMqttSessionIsConnect(self))
            {
                paho_mqtt_publish(&self->mqtt_client, QOS1, MQTT_SUBTOPIC_ATTRUP, self->attr_table.fdb_kv_json_str);
                self->mqttclient_session_state = MQTTCLIENT_SESSION_CONNECTING;
            }else{

            }
            TcloudAttrSyncTableFree(&self->attr_table);
        }
        break;
        case MQTTCLIENT_SESSION_CONNECTING:{
            // if(0 ==  )
            // {
            //     self->mqttclient_session_state = MQTTCLIENT_SESSION_CONNECTED;
            // }else{
            //     self->mqttclient_session_state = MQTTCLIENT_SESSION_IDLE;
            // }
        }
        break;
        case MQTTCLIENT_SESSION_CONNECTED:{
            
        }
        break;
        case MQTTCLIENT_SESSION_DISCONNECT:{

        }
        break;
    }
}

void ThingsCloudMqttSessionDestory(tcloud_mqtt_t *self)
{

}

