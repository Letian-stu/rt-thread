#include "pmu_task.h"
#include "pmu_task_manage.h"

#ifdef LOG_TAG 
#define LOG_TAG "pmu_task"
#endif
#define LOG_LVL     LOG_LVL_DBG   

#include <ulog.h>  

pmu_task_t g_pmu_task = {0};

static void PmuTaskCallBack(rt_uint8_t event, void *param)
{
    pmu_task_t *self = (pmu_task_t *)param;
    LOG_D("PmuTaskCallBack %x fsm %d", event, self->task_manage->task_manage_fsm);
}

static void PmuTaskEventCreate(pmu_task_t *self, rt_uint8_t event, pmu_task_event_cb cb)
{
    if (self == RT_NULL && cb == RT_NULL)
    {
        LOG_E("PmuTaskEventCreate self is NULL");
        return;
    }
    
    switch (event)
    {
    case PMU_TASK_CREATE_EVENT:{
        self->PmuCreateCb = cb;
    }
    break;
    case PMU_TASK_START_EVENT:{
        self->PmuStartCb = cb;
    }
    break;
    case PMU_TASK_STOP_EVENT:{
        self->PmuStopCb = cb;
    }
    break;
    case PMU_TASK_DESTORY_EVENT:{
        self->PmuDestoryCb = cb;
    }
    break;
    default:
        LOG_E("Not available event");
    break;
    }
}

static void PmuTaskEventRun(pmu_task_t *self, rt_uint8_t event)
{
    switch (event)
    {
    case PMU_TASK_CREATE_EVENT:{
        if(self->PmuCreateCb != RT_NULL)
            self->PmuCreateCb(event, self);
    }
    break;
    case PMU_TASK_START_EVENT:{
        if(self->PmuStartCb != RT_NULL)
            self->PmuStartCb(event, self);
    }
    break;
    case PMU_TASK_STOP_EVENT:{
        if(self->PmuStopCb != RT_NULL)
            self->PmuStopCb(event, self);
    }
    break;
    case PMU_TASK_DESTORY_EVENT:{
        if(self->PmuDestoryCb != RT_NULL)
            self->PmuDestoryCb(event, self);
    }
    break;
    default:
        LOG_E("Not available event");
    break;
    }
}

static pmu_task_t *PmuTaskeGet(void)
{
    return (pmu_task_t *)(&g_pmu_task);
}

void PmuTaskCreate(pmu_task_t *self)
{
    PmuTaskEventCreate(self, PMU_TASK_CREATE_EVENT, PmuTaskCallBack);
    PmuTaskEventCreate(self, PMU_TASK_START_EVENT, PmuTaskCallBack);
    PmuTaskEventCreate(self, PMU_TASK_STOP_EVENT, PmuTaskCallBack);
    PmuTaskEventCreate(self, PMU_TASK_DESTORY_EVENT, PmuTaskCallBack);

    self->task_manage = PmuTaskManageGet();
}

void PmuTaskControl(rt_uint8_t task_run_mask)
{
    pmu_task_t *self = (pmu_task_t *)PmuTaskeGet();
    if(self != RT_NULL)
    {
        self->last_task_mask = self->run_task_mask;
        self->run_task_mask = task_run_mask;
    }
}

void PmuTaskRunOnce(void)
{
    pmu_task_t *self = (pmu_task_t *)PmuTaskeGet();
    switch (self->task_manage->next_task_manage_fsm)
    {
    case PMU_TASK_IDLE:{
        
    }
    break;
    case PMU_TASK_FSM_CREATE:{
        PmuTaskManageCreate(self->task_manage);
		self->task_num = self->task_manage->task_num;
		for(int i = 0; i < self->task_num; i++)
		{
			self->all_task_mask |= (1 << i);
			self->run_task_mask |= (1 << i);
			self->last_task_mask |= (1 << i);
		}
        PmuTaskEventRun(self, PMU_TASK_CREATE_EVENT);
    }
    break;
    case PMU_TASK_FSM_STAETRUN:{
        PmuTaskManageStartRun(self->task_manage);
        PmuTaskEventRun(self, PMU_TASK_START_EVENT);
    }
    break;
    case PMU_TASK_FSM_RUNNING:{
        if(self->last_task_mask != self->run_task_mask)
		{
			PmuTaskManageRunning(self->task_manage, self->run_task_mask);
			self->last_task_mask = self->run_task_mask;
		}
    }
    break;
    case PMU_TASK_FSM_STOP:{
        PmuTaskManageStop(self->task_manage);
        PmuTaskEventRun(self, PMU_TASK_STOP_EVENT);
    }
    break;
    case PMU_TASK_FSM_DESTORY:{
        PmuTaskManageDestory(self->task_manage);
        PmuTaskEventRun(self, PMU_TASK_DESTORY_EVENT);
    }
    break;
    }
}


