#include <rtthread.h>
#include "pmu_task.h"
#include "msg_bus_interface.h"
#include "component_task.h"

#include <ulog.h>  

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "component_task"
#endif
#define LOG_LVL     LOG_LVL_DBG   

typedef struct component_task{
    rt_uint8_t task_event;  //任务事件

}component_task_t;

static void TComponentCreate(component_task_t *self)
{
    LOG_D("TComponentCreate");

    TMsgBusCreate();
}

static void TComponentRunOnce(component_task_t *self)
{
    //LOG_D("TComponentRunOnce");
}

static void TComponentDestory(component_task_t *self)
{
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
