#include "pmu_task_manage.h"

#include "component_task.h"
#include "tsp_task.h"
#include "can_task.h"

#include <ulog.h>  
 
#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "pmu_task"
#endif
#define LOG_LVL     LOG_LVL_DBG   

pmu_task_group_t pmu_task_groups[] = {
    {RT_NULL,   PMU_TASK_ID_COMPONENT, "TComponent_Task",  TComponent_Task,     &g_component_task,  2*1024, 10, 10},
    {RT_NULL,   PMU_TASK_ID_TSP,       "TTsp_Task",        TTsp_Task,           &g_tsp_task,    	2*1024, 10, 10},
    {RT_NULL,   PMU_TASK_ID_CAN,       "TCan_Task",        TCan_Task,           &g_can_task,    	2*1024, 10, 10},
};

pmu_task_manage_t g_pmu_task_manage = {
    .task_manage_fsm = PMU_TASK_FSM_CREATE,
    .next_task_manage_fsm = PMU_TASK_FSM_CREATE,
};

pmu_task_manage_t *PmuTaskManageGet(void)
{
    return (pmu_task_manage_t *)(&g_pmu_task_manage);
}

void PmuTaskManageCreate(pmu_task_manage_t *self)
{
    if(self == RT_NULL)
        return;

    self->task_manage_fsm = PMU_TASK_FSM_CREATE;
    self->next_task_manage_fsm = PMU_TASK_FSM_STAETRUN;
    self->run_task_num = 0;
    self->task_num = PMU_ARRAY_LEN(pmu_task_groups);
    self->task_group = (pmu_task_group_t *)pmu_task_groups;

    for(int i = 0; i < self->task_num; i++)
    {
        if(self->task_group[i].task != RT_NULL)
        {
            self->task_group[i].thread_handle = rt_thread_create(self->task_group[i].task_name, 
                                                                    self->task_group[i].task, 
                                                                    self->task_group[i].self, 
                                                                    self->task_group[i].stack_size,
                                                                    self->task_group[i].priority, 
                                                                    self->task_group[i].tick);
            if(self->task_group[i].thread_handle == RT_NULL)
            {
                LOG_E("pmu create %s task failed", self->task_group[i].task_name);
            }else{
                LOG_D("pmu create %s task success", self->task_group[i].task_name);
            }
        }
    }
}

void PmuTaskManageStartRun(pmu_task_manage_t *self)
{
    if(self == RT_NULL)
        return;

    self->task_manage_fsm = PMU_TASK_FSM_STAETRUN;
    self->next_task_manage_fsm = PMU_TASK_FSM_RUNNING;
    for(int i = 0; i < self->task_num; i++)
    {
        if(self->task_group[i].thread_handle != RT_NULL)
        {
            self->run_task_num++;
            rt_thread_startup(self->task_group[i].thread_handle);
        }
    }
}

void PmuTaskManageRunning(pmu_task_manage_t *self, rt_uint32_t task_run_mask)
{
    if(self == RT_NULL)
        return;

    for(int i = 0; i < self->task_num; i++)
    {
        if(((task_run_mask >> i)&0x01) == 0)
        {
            LOG_D("suspend %s task", self->task_group[i].task_name);
        }
        else if(((task_run_mask >> i)&0x01) == 1)
        {
            LOG_D("resume %s task", self->task_group[i].task_name);
        }
    }
}

void PmuTaskManageStop(pmu_task_manage_t *self)
{
    if(self == RT_NULL)
        return;

    self->task_manage_fsm = PMU_TASK_FSM_STOP;
    self->next_task_manage_fsm = PMU_TASK_FSM_DESTORY;
}

void PmuTaskManageDestory(pmu_task_manage_t *self)
{
    if(self == RT_NULL)
        return;

    self->task_manage_fsm = PMU_TASK_FSM_DESTORY;
    self->next_task_manage_fsm = PMU_TASK_IDLE;
    for(int i = 0; i < self->task_num; i++)
    {
        if(self->task_group[i].thread_handle != RT_NULL)
        {
            rt_thread_delete(self->task_group[i].thread_handle);
            self->task_group[i].thread_handle = RT_NULL;
        }
    }
}

