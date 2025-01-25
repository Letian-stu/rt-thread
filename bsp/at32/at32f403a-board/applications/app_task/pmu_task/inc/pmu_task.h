#ifndef _PMU_TASK_H_
#define _PMU_TASK_H_

#include <rtthread.h>
#include "pmu_task_manage.h"

enum{
    PMU_TASK_CREATE_EVENT = 0,
    PMU_TASK_START_EVENT,
    PMU_TASK_STOP_EVENT,
    PMU_TASK_DESTORY_EVENT,
};

typedef void (*pmu_task_event_cb)(rt_uint8_t event, void *param);

typedef struct pmu_task_event
{
    pmu_task_manage_t   *task_manage;   //任务管理
    rt_uint8_t          task_num;       //任务数量
    rt_uint32_t         all_task_mask;  //所有任务掩码
    rt_uint32_t         run_task_mask;  //运行任务掩码
    rt_uint32_t         last_task_mask; //上一次运行任务掩码
    pmu_task_event_cb   PmuCreateCb;    //创建回调
    pmu_task_event_cb   PmuStartCb;     //开始回调
    pmu_task_event_cb   PmuStopCb;      //停止回调
    pmu_task_event_cb   PmuDestoryCb;   //销毁回调
}pmu_task_t;

extern pmu_task_t g_pmu_task;

void PmuTaskCreate(pmu_task_t *self);

void PmuTaskControl(rt_uint8_t task_run_mask);

void PmuTaskRunOnce(void);

#endif
