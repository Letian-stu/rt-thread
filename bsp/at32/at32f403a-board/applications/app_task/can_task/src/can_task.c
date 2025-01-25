#include <rtthread.h>
#include "pmu_task.h"
#include "msg_bus_interface.h"
#include "can_task.h"

#include <ulog.h>  

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "TCan_Task"
#endif
#define LOG_LVL     LOG_LVL_DBG  

can_task_t g_can_task = {0};

static void TCanCreate(can_task_t *self)
{
    LOG_D("TCanCreate");
    self->msg_bus_subscriber_id = task_msg_subscriber_create2(g_msg_bus_event_id, TASK_MSG_COUNT);
}

static void TCanRunOnce(can_task_t *self)
{
    //LOG_D("TCanRunOnce");
}

static void TCanDestory(can_task_t *self)
{
    LOG_D("TCanDestory");
}

void TCan_Task(void *param)
{
    can_task_t *self = (can_task_t *)param;
    TCanCreate(self);
    while(1)
    {
        if (task_msg_wait_until(self->msg_bus_subscriber_id, MSG_BUS_TIMEOUT, &self->msg_bus_args) == RT_EOK)
        {
            switch(self->msg_bus_args->msg_name)
            {
                case MSG_BUS_EVENT_RUN1S:{
                    
                }
                break;
                case MSG_BUS_EVENT_TASKSUSPED:{
                    
                }
                break;
                case MSG_BUS_EVENT_DATA_TRANS:{
                    
                }
                break;
            }
            task_msg_release(self->msg_bus_args);
        }
        TCanRunOnce(self);
    }
    TCanDestory(self);
}



