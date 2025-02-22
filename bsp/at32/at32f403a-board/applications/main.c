/*
 * @Descripttion: 
 * @version: 
 * @Author: letian
 * @Date: 2024-10-29 22:59:50
 * @LastEditTime: 2025-02-08 22:59:20
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"

#include "drv_spi.h"
#include "spi_flash_sfud.h"
#include "fal.h"
#include "flashdb.h"
#include "dfs_fs.h"
 
#ifdef LOG_TAG 
#define LOG_TAG "main"
#endif
#define LOG_LVL     LOG_LVL_DBG   

#include <ulog.h>  

#include "at32f403a_407.h"

static int ota_app_vtor_reconfig(void)
{
    #define RT_APP_PORT_ADDR 0x08040000
    #define NVIC_VTOR_MASK 0xFFFFFF80

    SCB->VTOR = RT_APP_PORT_ADDR;
    return 0;
}
INIT_BOARD_EXPORT(ota_app_vtor_reconfig);


int rt_spi_w25Q128_init(void)
{
    /* 向系统注册SPI1总线上spi10从设备 */
    rt_hw_spi_device_attach("spi2", "spi20", GPIOB, GPIO_PINS_12); 

    /* 将w25Q128抽象为块设备，以便后续挂载文件系统 */
    if(rt_sfud_flash_probe("w25q64", "spi20"))
    {
        LOG_I("rt sfud flash probe success");
        return RT_EOK;
    }else {
        LOG_I("rt sfud flash probe fail");
        return -RT_ERROR;
    }
}
INIT_DEVICE_EXPORT(rt_spi_w25Q128_init);

int rt_fal_init(void)
{
	fal_init(); 
	return 0;	
}
INIT_COMPONENT_EXPORT(rt_fal_init);


#define FS_PARTITION_NAME "fs"

int dfs_mount_init(void)
{
    struct rt_device *blk_dev = RT_NULL;
    blk_dev = fal_blk_device_create(FS_PARTITION_NAME);
    if (!blk_dev)
    {
        LOG_E("Can't create a mtd device on '%s' partition.", FS_PARTITION_NAME);
    }
    else
    {
        if (dfs_mount(FS_PARTITION_NAME, "/", "elm",0,0) == 0)
        {
            LOG_I("Filesystem initialized!");
        }
        else
        {
            dfs_mkfs("elm", FS_PARTITION_NAME);
            if (dfs_mount(FS_PARTITION_NAME, "/", "elm", 0, 0) == 0)
            {
                LOG_I("Filesystem initialized!");
            }
            else
            {
                LOG_E("Failed to initialize filesystem!");
            }
        }
    }
}
INIT_ENV_EXPORT(dfs_mount_init);

void show_version(void)
{
	#define HARDWAREVERSION "V1.0.0"
	#define SOFTWAREVERSION "V1.0.0"
	LOG_I("Hard Version %s", HARDWAREVERSION);
	LOG_I("Soft Version %s", SOFTWAREVERSION);
}

#include "pmu_task.h"
#include "pmu_task_manage.h"
#include "task_msg_bus.h"
#include "msg_bus_interface.h"

int main(void)
{
	show_version();
    PmuTaskCreate(&g_pmu_task);
	rt_uint32_t tick = 0;
    while (1)
    {
        PmuTaskRunOnce();
        rt_thread_mdelay(1000);
        task_msg_publish(MSG_BUS_EVENT_RUN1S, RT_NULL);
    }
}
