#ifndef _OTA_TASK_H_
#define _OTA_TASK_H_

#include <rtthread.h>
#include "task_msg_bus.h"
#include "tsp_httpota.h"


typedef struct Ota_task{
    rt_uint8_t task_event;              //任务事件
    rt_int16_t msg_bus_subscriber_id;   //消息总线订阅者ID
    task_msg_args_t msg_bus_args;       //消息总线参数

    tsp_httpota_t http_ota_session;

}Ota_task_t;

extern Ota_task_t g_Ota_task;

void TOta_Task(void *param);

#endif
