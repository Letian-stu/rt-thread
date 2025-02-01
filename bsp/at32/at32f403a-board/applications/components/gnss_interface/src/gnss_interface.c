#include "gnss_interface.h"
#include "drivers/serial.h"
#include <stdio.h>

#define DBG_TAG "gnss_task"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

/* -------------------------------------------------------------------------- */
/*                              define workspace                              */
/* -------------------------------------------------------------------------- */
#define TCOMP_GNSS_DEV_PORT     ("uart2")
#define TCOMP_GNSS_ENABLE_LOG	(0)
#define TCOMP_GNSS_DEBUG        (0)

/* -------------------------------------------------------------------------- */
/*                             variable workspace                             */
/* -------------------------------------------------------------------------- */
static struct rt_messagequeue rx_mq;


/* -------------------------------------------------------------------------- */
/*                               static function                              */
/* -------------------------------------------------------------------------- */
#if TCOMP_GNSS_DEBUG
static void TCompGnssRxTraceCallback(const char *str, int str_size)
{
    rt_kprintf("nmea trace:");
    for (int i = 0; i < str_size; i++)
        rt_kprintf("%c", str[i]);
}

static void TCompGnssRxErrorCallback(const char *str, int str_size)
{
    rt_kprintf("nmea error:");
    for (int i = 0; i < str_size; i++)
        rt_kprintf("%c", str[i]);
}
#endif

static rt_err_t TCompGnssRxCallback(rt_device_t dev, rt_size_t size)
{
    TGnssRxMsg_t msg;
    rt_err_t result;
    msg.dev_handle = dev;
    msg.size = size;

    result = rt_mq_send(&rx_mq, &msg, sizeof(msg));
    // if (result == -RT_EFULL)
    // {
    //     rt_kprintf("gnss message queue full");
    // }
    return result;
}

/* -------------------------------------------------------------------------- */
/*                               global function                              */
/* -------------------------------------------------------------------------- */
void TCompGnssCreate(TCompGnss_t *self)
{
    static struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    self->gnss_dev_handle = rt_device_find(TCOMP_GNSS_DEV_PORT);
    RT_ASSERT(self->gnss_dev_handle != RT_NULL);

    rt_mq_init(&rx_mq, "gnss_rx_mq", self->gnss_rx_msg_pool, \
        sizeof(self->msg), sizeof(self->gnss_rx_msg_pool), RT_IPC_FLAG_FIFO);

    config.baud_rate = BAUD_RATE_9600;
    rt_device_control(self->gnss_dev_handle, RT_DEVICE_CTRL_CONFIG, &config);

    rt_device_open(self->gnss_dev_handle, RT_DEVICE_FLAG_DMA_RX); 
    rt_device_set_rx_indicate(self->gnss_dev_handle, TCompGnssRxCallback); 

    nmea_zero_INFO(&self->gnss_nmea_info);
    nmea_parser_init(&self->gnss_nmea_parser);

#if TCOMP_GNSS_DEBUG
    nmea_property()->trace_func = &TCompGnssRxTraceCallback;
    nmea_property()->error_func = &TCompGnssRxErrorCallback;
#endif

    self->print_info_time = 10000;
    self->is_open = RT_TRUE;
}

void TCompGnssDestory(TCompGnss_t *self)
{
    RT_ASSERT(self != RT_NULL);

    self->is_open = RT_FALSE;

    nmea_parser_destroy(&self->gnss_nmea_parser);
    rt_device_close(self->gnss_dev_handle); 
    rt_mq_detach(&self->gnss_rx_mq_handle);
}

void TCompGnssRunOnce(TCompGnss_t *self)
{
    RT_ASSERT(self != RT_NULL);

    rt_memset(&self->msg, 0, sizeof(self->msg));
    rt_ssize_t result = rt_mq_recv(&rx_mq, &self->msg, sizeof(self->msg), 0);
    if (result > 0)
    {
        self->rx_length = rt_device_read(self->msg.dev_handle, 0, self->gnss_recv_data, self->msg.size);
        self->gnss_recv_data[self->rx_length] = '\0';
        // LOG_I("Gnss recv %d", self->rx_length);
        // LOG_HEX(DBG_TAG, 16, self->gnss_recv_data, self->rx_length);
        nmea_parse(&self->gnss_nmea_parser, (const char *)self->gnss_recv_data, self->rx_length, &self->gnss_nmea_info);

        if(rt_tick_get_millisecond() - self->last_print_time > self->print_info_time)
        {
            //info.lat lon中的格式为[degree][min].[sec/60]，使用以下函数转换成[degree].[degree]格式
            double deg_lat = nmea_ndeg2degree(self->gnss_nmea_info.lat);
            double deg_lon = nmea_ndeg2degree(self->gnss_nmea_info.lon);

#if TCOMP_GNSS_ENABLE_LOG
            LOG_D("===== Gnss Info =====");
            LOG_D("utc_time:%d-%02d-%02d,%d:%d:%d ", self->gnss_nmea_info.utc.year + 1900, self->gnss_nmea_info.utc.mon + 1,
                self->gnss_nmea_info.utc.day, self->gnss_nmea_info.utc.hour, self->gnss_nmea_info.utc.min, self->gnss_nmea_info.utc.sec);

            snprintf(self->log_info, sizeof(self->log_info), "wd:%f,jd:%f ", deg_lat, deg_lon);
            LOG_D("%s", self->log_info);
            snprintf(self->log_info, sizeof(self->log_info), "high:%f m ", self->gnss_nmea_info.elv);
            LOG_D("%s", self->log_info);
            snprintf(self->log_info, sizeof(self->log_info), "v:%f km/h ", self->gnss_nmea_info.speed);
            LOG_D("%s", self->log_info);
            snprintf(self->log_info, sizeof(self->log_info), "hangxiang:%f du ", self->gnss_nmea_info.direction);
            LOG_D("%s", self->log_info);
            snprintf(self->log_info, sizeof(self->log_info), "used GPS:%d,show GPS:%d ", self->gnss_nmea_info.satinfo.inuse, self->gnss_nmea_info.satinfo.inview);
            LOG_D("%s", self->log_info);
            snprintf(self->log_info, sizeof(self->log_info), "PDOP:%f,HDOP:%f,VDOP:%f ", self->gnss_nmea_info.PDOP, self->gnss_nmea_info.HDOP, self->gnss_nmea_info.VDOP);
            LOG_D("%s", self->log_info);
#endif
            self->last_print_time = rt_tick_get_millisecond();
        }
    }
}
