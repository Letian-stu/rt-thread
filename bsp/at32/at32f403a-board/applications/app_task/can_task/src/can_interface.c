#include "can_interface.h"
#include <rtthread.h>
#include "rtdevice.h"
#include "drivers/can.h"

#include <ulog.h>  

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "can_interface"
#endif
#define LOG_LVL     LOG_LVL_DBG  

void TCanDeviceCreate(can_interface_t *self, CanRxCallback cb)
{
    rt_err_t res;

    rt_sem_init(&self->rx_sem, "can_rx_sem", 0, RT_IPC_FLAG_FIFO);
    self->can_dev = rt_device_find(self->can_name);
    if (!self->can_dev)
    {
        LOG_E("find %s failed!", self->can_name);
        return RT_ERROR;
    }
    res = rt_device_open(self->can_dev, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
    RT_ASSERT(res == RT_EOK);

    rt_device_set_rx_indicate(self->can_dev, cb);

    struct rt_can_filter_item items[] =
    {
        RT_CAN_FILTER_ITEM_INIT(0x000, 0, 0, 0, 0x000, RT_NULL, RT_NULL),	
        RT_CAN_FILTER_ITEM_INIT(0x00000000, 1, 0, 0, 0x00000000, RT_NULL, RT_NULL),
    };
    struct rt_can_filter_config cfg = {sizeof(items)/sizeof(items[0]), 1, items};

    res = rt_device_control(self->can_dev, RT_CAN_CMD_SET_FILTER, &cfg);
    RT_ASSERT(res == RT_EOK);

    res = rt_device_control(self->can_dev, RT_CAN_CMD_SET_BAUD, self->baudrate);
    RT_ASSERT(res == RT_EOK);
}

void TCanDeviceDestory(can_interface_t *self)
{
    rt_device_close(self->can_dev);
    rt_sem_detach(&self->rx_sem);
}
