#include "tcloud_mqtt.h"
#include <rtthread.h>
#include "at_device.h"
#include "blib.h"

#ifdef LOG_TAG 
#define LOG_TAG "TCloud"
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
    LOG_D("mqtt sub callback: topic [%d][%s] rdata [%d][%s]",
               msg_data->topicName->lenstring.len,
               msg_data->topicName->lenstring.data,
               msg_data->message->payloadlen,
               (char *)msg_data->message->payload);
    
    tcloud_user_data_t *user_data = (tcloud_user_data_t *)c->user_data;

    tcloud_recv_mag_t *msg = &user_data->recv_msg;

    msg->topic_len = msg_data->topicName->lenstring.len;
    msg->recv_len = msg_data->message->payloadlen;
    rt_memcpy(msg->topic_name, msg_data->topicName->lenstring.data, msg->topic_len);
    rt_memcpy(msg->recv_data, msg_data->message->payload, msg->recv_len);

    int res = rt_sem_release(user_data->msg_sem);
    if(RT_EOK != res)
    {
        LOG_D("mqtt recv msg release err", res);
    }
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
    client->condata.keepAliveInterval = 60*3;
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

    client->user_data = &self->mqtt_user_data;

    self->mqtt_user_data.msg_sem = rt_sem_create("mqtt_recv_sem", 0, RT_IPC_FLAG_FIFO);
    if(self->mqtt_user_data.msg_sem == NULL)
    {
        LOG_E("no memory for MQTT recv msg!");
        return;
    }
}

int TCloudMqttSessionIsConnect(tcloud_mqtt_t *self)
{
    return self->mqtt_client.isconnected;
}

void ThingsCloudMqttSessionCreate(tcloud_mqtt_t *self)
{
    TCloudBaseInfoCfg(self);
    TCloudAttrSyncTableInit(&self->attr_table);
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
            if(!self->mqtt_session_is_init)
            {
                ThingsCloudMqttCreate(self);
                paho_mqtt_start(&self->mqtt_client);      
            }
            self->mqttclient_session_state = MQTTCLIENT_SESSION_CONNECTING;      
        }
        break;
        case MQTTCLIENT_SESSION_CONNECTING:{
            if(TCloudMqttSessionIsConnect(self))
            {
                self->mqttclient_session_state = MQTTCLIENT_SESSION_SYNCATTR;
            }else{
                self->mqttclient_session_state = MQTTCLIENT_SESSION_IDLE;
            }
        }
        break;
        case MQTTCLIENT_SESSION_SYNCATTR:{
            TCloudAttrSyncTableCreate(&self->attr_table);
            paho_mqtt_publish(&self->mqtt_client, QOS1, MQTT_SUBTOPIC_ATTRUP, self->attr_table.fdb_kv_json_str);
            TCloudAttrSyncTableFree(&self->attr_table);
            self->mqttclient_session_state = MQTTCLIENT_SESSION_CONNECTED;
        }
        break;
        case MQTTCLIENT_SESSION_CONNECTED:{
            tcloud_recv_mag_t *recv_msg = &self->mqtt_user_data.recv_msg;
            int result = rt_sem_take(self->mqtt_user_data.msg_sem, 0);
            if (result == RT_EOK)
            {
                LOG_D("mqtt recv topic len %d name [%s]", recv_msg->topic_len, recv_msg->topic_name);
                LOG_D("mqtt recv msg len %d data [%s]", recv_msg->recv_len, recv_msg->recv_data);
                if(0 == rt_memcmp(MQTT_SUBTOPIC_ATTRRESP, recv_msg->topic_name, recv_msg->topic_len))
                {
                    LOG_I("mqtt recv topic %s", MQTT_SUBTOPIC_ATTRRESP);
                }
                else if(0 == rt_memcmp(MQTT_SUBTOPIC_ATTRPUSH, recv_msg->topic_name, recv_msg->topic_len))
                {
                    LOG_I("mqtt recv topic %s", MQTT_SUBTOPIC_ATTRPUSH);
                    LOG_I("write attr num %d", TCloudAtteGenerateDbTable(self, recv_msg->recv_data));
                }
                else if(0 == rt_memcmp(MQTT_SUBTOPIC_EVENTRESP, recv_msg->topic_name, recv_msg->topic_len))
                {
                    LOG_I("mqtt recv topic %s", MQTT_SUBTOPIC_EVENTRESP);

                }
                else if(0 == rt_memcmp(MQTT_SUBTOPIC_CMDSEND, recv_msg->topic_name, recv_msg->topic_len))
                {
                    LOG_I("mqtt recv topic %s", MQTT_SUBTOPIC_CMDSEND);

                }
                else if(0 == rt_memcmp(MQTT_SUBTOPIC_CMDRESP, recv_msg->topic_name, recv_msg->topic_len))
                {
                    LOG_I("mqtt recv topic %s", MQTT_SUBTOPIC_CMDRESP);

                }
                else if(0 == rt_memcmp(MQTT_SUBTOPIC_SERIES, recv_msg->topic_name, recv_msg->topic_len))
                {
                    LOG_I("mqtt recv topic %s", MQTT_SUBTOPIC_SERIES);

                }
                rt_memset(&self->mqtt_user_data.recv_msg, 0x00, sizeof(self->mqtt_user_data.recv_msg));
            }
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

