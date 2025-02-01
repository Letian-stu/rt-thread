#include <rtthread.h>
#include "pmu_task.h"
#include "component_task.h"

#include <ulog.h>  

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "component_task"
#endif
#define LOG_LVL     LOG_LVL_DBG   

component_task_t g_component_task;

static void TComponentCreate(component_task_t *self)
{
    LOG_D("TComponentCreate");

    TMsgBusCreate();

    TCompGnssCreate(&self->gnss_handle);
}

static void TComponentRunOnce(component_task_t *self)
{
    //LOG_D("TComponentRunOnce");
    TCompGnssRunOnce(&self->gnss_handle);
}

static void TComponentDestory(component_task_t *self)
{
    TCompGnssDestory(&self->gnss_handle);
    LOG_D("TComponentDestory");
}

void TComponent_Task(void *param)
{
    component_task_t *self = (component_task_t *)param;
    TComponentCreate(self);
    while(1)
    {
        TComponentRunOnce(self);
        rt_thread_mdelay(10);
    }
    TComponentDestory(self);
}
