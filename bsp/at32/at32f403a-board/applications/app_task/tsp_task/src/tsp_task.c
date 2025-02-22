#include <rtthread.h>
#include "pmu_task.h"
#include "msg_bus_interface.h"
#include "tsp_task.h"

#ifdef LOG_TAG 
#define LOG_TAG "TTsp_Task"
#endif
#define LOG_LVL     LOG_LVL_DBG  

#include <ulog.h>  

tsp_task_t g_tsp_task = {0};

static void TTspCreate(tsp_task_t *self)
{
    LOG_D("TTspCreate");
    ThingsCloudMqttSessionCreate(&self->mqtt_session);
    self->msg_bus_subscriber_id = task_msg_subscriber_create2(g_msg_bus_event_id, TASK_MSG_COUNT);
}

static void TTspRunOnce(tsp_task_t *self)
{
    //LOG_D("TTspRunOnce");
}

static void TTspDestory(tsp_task_t *self)
{
    LOG_D("TTspDestory");
}

void TTsp_Task(void *param)
{
    tsp_task_t *self = (tsp_task_t *)param;
    TTspCreate(self);
    while(1)
    {
        if (task_msg_wait_until(self->msg_bus_subscriber_id, MSG_BUS_TIMEOUT, &self->msg_bus_args) == RT_EOK)
        {
            switch(self->msg_bus_args->msg_name)
            {
                case MSG_BUS_EVENT_RUN1S:{
                    ThingsCloudMqttSessionRunOnce(&self->mqtt_session);
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
        TTspRunOnce(self);
    }
    TTspDestory(self);
}



