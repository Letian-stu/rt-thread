/*
 * @Descripttion: 
 * @version: 
 * @Author: letian
 * @Date: 2024-10-29 22:59:50
 * @LastEditTime: 2024-10-31 23:05:14
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <ulog.h>  

#include "drv_spi.h"
#include "spi_flash_sfud.h"
#include "fal.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "main"
#endif
#define LOG_LVL     LOG_LVL_DBG   

       
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


int main(void)
{
    rt_spi_w25Q128_init();
    fal_init();
    while (1)
    {
        rt_thread_mdelay(1000);
    }
}
