#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_

#include <rtthread.h>
#include "rtdevice.h"

typedef struct can_interface
{
    char                can_name[RT_NAME_MAX];      // CAN设备名称
    rt_uint32_t         baudrate;                   // 波特率

    struct rt_semaphore rx_sem;                     // 接收信号量
    rt_device_t         can_dev;                    // CAN设备
}can_interface_t;

typedef rt_err_t (*CanRxCallback)(rt_device_t dev, rt_size_t size);

void TCanDeviceCreate(can_interface_t *self, CanRxCallback cb);

void TCanDeviceDestory(can_interface_t *self);

#endif
