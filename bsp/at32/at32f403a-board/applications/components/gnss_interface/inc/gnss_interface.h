#ifndef _TASK_GNSS_
#define _TASK_GNSS_

#include <rtthread.h>
#include <rtdevice.h>
#include "nmea/nmea.h"

typedef struct TGnssRxMsg
{
    rt_device_t dev_handle;
    rt_size_t size;
}TGnssRxMsg_t;

typedef struct TCompGnss
{
    rt_bool_t is_open;                          //是否开启

    rt_device_t gnss_dev_handle;                //串口设备句柄
    struct serial_configure *gnss_dev_config;   //串口配置
    struct rt_messagequeue gnss_rx_mq_handle;   //消息队列句柄
    char gnss_rx_msg_pool[128];                 //消息队列缓冲区
    rt_thread_t gnss_sync_thread;               //gnss获取任务

    nmeaINFO gnss_nmea_info;                    //nmea 解码后得到的信息
    nmeaPARSER gnss_nmea_parser;                //nmea 解码时的数据结构

    rt_uint32_t rx_length;                      //串口接收数据长度
    char gnss_recv_data[512];                  //串口接收数据缓冲区
    TGnssRxMsg_t msg;                           //中断消息回调接收

    uint32_t print_info_time;                   //打印信息时间
    uint32_t last_print_time;                   //上次打印时间    
    char log_info[256];                         //打印缓存数据
}TCompGnss_t;

void TCompGnssCreate(TCompGnss_t *self);

void TCompGnssDestory(TCompGnss_t *self);

void TCompGnssRunOnce(TCompGnss_t *self);

#endif
