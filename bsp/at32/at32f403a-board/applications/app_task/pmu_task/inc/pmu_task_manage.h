#ifndef _PMU_TASK_MANAGE_H_
#define _PMU_TASK_MANAGE_H_

#include <rtthread.h>

#define PMU_MAX_TASKS_NUM   (10)
#define PMU_ARRAY_LEN(arr)  (sizeof(arr) / sizeof((arr)[0]))

enum{
    PMU_TASK_ID_COMPONENT = 0,
    PMU_TASK_ID_GNSS,
    PMU_TASK_ID_TSP,
    PMU_TASK_ID_CAN,
    PMU_TASK_ID_OTA,
};

enum{
    PMU_TASK_IDLE = 0,      //空闲
    PMU_TASK_FSM_CREATE,    //初始化
    PMU_TASK_FSM_STAETRUN,  //开始运行
    PMU_TASK_FSM_RUNNING,   //运行中
    PMU_TASK_FSM_STOP,      //停止
    PMU_TASK_FSM_DESTORY,   //销毁
};
typedef struct pmu_task_group
{
    rt_thread_t thread_handle;
    rt_uint8_t  task_id;
    char        *task_name;
    void        (*task)(void *param);
    void        *self;
    rt_uint32_t stack_size;
    rt_uint8_t  priority;
    rt_uint32_t tick;
} pmu_task_group_t;

typedef struct pmu_task_manage
{
    rt_uint8_t task_manage_fsm;         //任务管理状态
    rt_uint8_t next_task_manage_fsm;    //下一个任务管理状态
    rt_uint8_t task_num;                //总任务数量
    rt_uint8_t run_task_num;            //运行任务数量
    pmu_task_group_t *task_group;       //任务组
} pmu_task_manage_t;

extern pmu_task_group_t pmu_task_groups[];

pmu_task_manage_t *PmuTaskManageGet(void);

void PmuTaskManageCreate(pmu_task_manage_t *self);

void PmuTaskManageStartRun(pmu_task_manage_t *self);

void PmuTaskManageRunning(pmu_task_manage_t *self, rt_uint32_t task_run_mask);

void PmuTaskManageStop(pmu_task_manage_t *self);

void PmuTaskManageDestory(pmu_task_manage_t *self);

#endif
