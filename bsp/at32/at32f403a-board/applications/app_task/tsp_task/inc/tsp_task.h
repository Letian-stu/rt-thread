#ifndef _TSP_TASK_H_
#define _TSP_TASK_H_

#include <rtthread.h>
#include "task_msg_bus.h"

typedef struct tsp_task{
    rt_uint8_t task_event;              //任务事件
    rt_int16_t msg_bus_subscriber_id;   //消息总线订阅者ID
    task_msg_args_t msg_bus_args;       //消息总线参数

}tsp_task_t;

extern tsp_task_t g_tsp_task;

void TTsp_Task(void *param);

#endif
