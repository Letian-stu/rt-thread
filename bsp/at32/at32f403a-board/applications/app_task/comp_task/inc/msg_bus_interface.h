#ifndef _MSG_BUS_INTERFACE_H_
#define _MSG_BUS_INTERFACE_H_

#include <rtthread.h>

#define MSG_BUS_TIMEOUT (20)

enum task_msg_name{
    MSG_BUS_EVENT_RUN1S = 0,
    MSG_BUS_EVENT_TASKSUSPED,
    MSG_BUS_EVENT_DATA_TRANS,
    MSG_BUS_EVENT_START_HTTP_OTA,
    TASK_MSG_COUNT,
};

extern enum task_msg_name g_msg_bus_event_id[];

void TMsgBusCreate(void);

#endif
