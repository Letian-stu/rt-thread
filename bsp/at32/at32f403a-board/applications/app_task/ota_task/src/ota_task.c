#include <rtthread.h>
#include "pmu_task.h"
#include "msg_bus_interface.h"
#include "Ota_task.h"

#ifdef LOG_TAG 
#define LOG_TAG "TOta_Task"
#endif
#define LOG_LVL     LOG_LVL_DBG  

#include <ulog.h>  

Ota_task_t g_Ota_task = {0};

static void TOtaCreate(Ota_task_t *self)
{
    LOG_D("TOtaCreate");
    self->msg_bus_subscriber_id = task_msg_subscriber_create2(g_msg_bus_event_id, TASK_MSG_COUNT);

    THttpOtaTaskCreate(&self->http_ota_session);
}

static void TOtaRunOnce(Ota_task_t *self)
{
    //LOG_D("TOtaRunOnce");

}

static void TOtaDestory(Ota_task_t *self)
{
    LOG_D("TOtaDestory");
}

void TOta_Task(void *param)
{
    Ota_task_t *self = (Ota_task_t *)param;
    TOtaCreate(self);
    while(1)
    {
        if (task_msg_wait_until(self->msg_bus_subscriber_id, MSG_BUS_TIMEOUT, &self->msg_bus_args) == RT_EOK)
        {
            switch(self->msg_bus_args->msg_name)
            {
                case MSG_BUS_EVENT_RUN1S:{
                    THttpOtaTaskRunOnce(&self->http_ota_session);
                }
                break;
                case MSG_BUS_EVENT_TASKSUSPED:{
                    
                }
                break;
                case MSG_BUS_EVENT_DATA_TRANS:{

                }
                break;
                case MSG_BUS_EVENT_START_HTTP_OTA:{
                    
                }
                break;
            }
            task_msg_release(self->msg_bus_args);
        }
        TOtaRunOnce(self);
    }
    TOtaDestory(self);
}



