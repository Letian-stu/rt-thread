#ifndef _COMPONENT_TASK_H_
#define _COMPONENT_TASK_H_

#include <rtthread.h>
#include "task_msg_bus.h"

#include "gnss_interface.h"

typedef struct component_task{
    rt_uint8_t task_event;

    TCompGnss_t gnss_handle;
}component_task_t;

extern component_task_t g_component_task;

void TComponent_Task(void *param);

#endif
