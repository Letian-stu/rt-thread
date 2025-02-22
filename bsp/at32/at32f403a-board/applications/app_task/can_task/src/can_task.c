#include <rtthread.h>
#include "pmu_task.h"
#include "msg_bus_interface.h"
#include "can_task.h"
#include "can_interface.h"

#ifdef LOG_TAG 
#define LOG_TAG "TCan_Task"
#endif
#define LOG_LVL     LOG_LVL_DBG  

#include <ulog.h>  

can_task_t g_can_task = {0};

static rt_err_t TCan1RxCallback(rt_device_t dev, rt_size_t size)
{
    can_interface_t *self = (can_interface_t *)&g_can_task.can1_interface;
    rt_sem_release(&self->rx_sem);
    return RT_EOK;
}

static rt_err_t TCan2RxCallback(rt_device_t dev, rt_size_t size)
{
    can_interface_t *self = (can_interface_t *)&g_can_task.can2_interface;
    rt_sem_release(&self->rx_sem);
    return RT_EOK;
}

static void TCanCreate(can_task_t *self)
{
    LOG_D("TCanCreate");
    self->msg_bus_subscriber_id = task_msg_subscriber_create2(g_msg_bus_event_id, TASK_MSG_COUNT);

    #define CAN1_DEV_NAME       "can1"
    rt_strncpy(self->can1_interface.can_name, CAN1_DEV_NAME, RT_NAME_MAX);
    self->can1_interface.baudrate = CAN500kBaud;
    TCanDeviceCreate(&self->can1_interface, TCan1RxCallback);

    #define CAN2_DEV_NAME       "can2"
    rt_strncpy(self->can2_interface.can_name, CAN2_DEV_NAME, RT_NAME_MAX);
    self->can2_interface.baudrate = CAN500kBaud;
    TCanDeviceCreate(&self->can2_interface, TCan2RxCallback);
}

static void TCanRunOnce(can_task_t *self)
{
    can_interface_t *can1_interface = &self->can1_interface;
    rt_err_t rst = rt_sem_take(&can1_interface->rx_sem, 0);
    if (rst == RT_EOK)
    {
        struct rt_can_msg rxmsg = {0};
        rxmsg.hdr_index = -1;
        if( rt_device_read(can1_interface->can_dev, 0, &rxmsg, sizeof(rxmsg)) > 0)
        {
            LOG_D("can1 recv data id:%8x len:%d", rxmsg.id, rxmsg.len);
            LOG_HEX("can1 recv data", 8, rxmsg.data, 8);
            rt_device_write(can1_interface->can_dev, 0, &rxmsg, sizeof(rxmsg));
        }
    }

    can_interface_t *can2_interface = &self->can2_interface;
    rst = rt_sem_take(&can2_interface->rx_sem, 0);
    if (rst == RT_EOK)
    {
        struct rt_can_msg rxmsg = {0};
        rxmsg.hdr_index = -1;
        if( rt_device_read(can2_interface->can_dev, 0, &rxmsg, sizeof(rxmsg)) > 0)
        {
            LOG_D("can2 recv data id:%8x len:%d", rxmsg.id, rxmsg.len);
            LOG_HEX("can2 recv data", 8, rxmsg.data, 8);
            rt_device_write(can2_interface->can_dev, 0, &rxmsg, sizeof(rxmsg));
        }
    }

}

static void TCanDestory(can_task_t *self)
{
    LOG_D("TCanDestory");
    TCanDeviceDestory(&self->can1_interface);
    TCanDeviceDestory(&self->can2_interface);
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
