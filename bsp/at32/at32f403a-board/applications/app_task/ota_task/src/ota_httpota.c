#include "tsp_httpota.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <rtthread.h>
#include <finsh.h>

#include "webclient.h"
#include <fal.h>
#include "at_device.h"
#include "blib.h"

#include "flashdb_table.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "tsp_http_ota"
#endif
#define LOG_LVL     LOG_LVL_DBG  

#include <ulog.h>  

const struct fal_partition *g_dl_part = RT_NULL;
static int g_begin_offset = 0;
static int g_file_size = 0;

void THttpOtaTaskCreate(tsp_httpota_t *self)
{
    self->http_buff_len = HTTP_OTA_BUFF_LEN;
    self->http_get_header_buff_size = GET_HEADER_BUFSZ;
    self->http_resport_buff_size = GET_RESP_BUFSZ;
    self->http_ota_download_delay = HTTP_OTA_DL_DELAY;
    
    self->ota_fsm = THTTP_OTA_FSM_NET_CONNECT;

    rt_strcpy(self->http_ota_url, HTTP_OTA_URL);
    rt_strcpy(self->ota_port_name, HTTP_OTA_PART_NAME);
}

static void print_progress(size_t cur_size, size_t total_size)
{
    #define LODDER_LEN 20
    static unsigned char progress_sign[LODDER_LEN + 1];
    uint8_t i, per = cur_size * 100 / total_size;

    if (per > 100)
    {
        per = 100;
    }

    for (i = 0; i < LODDER_LEN; i++)
    {
        if (i < (per/(100/LODDER_LEN)))
        {
            progress_sign[i] = '=';
        }
        else if (i == (per/(100/LODDER_LEN)))
        {
            progress_sign[i] = '>';
        }
        else
        {
            progress_sign[i] = ' ';
        }
    }

    progress_sign[sizeof(progress_sign) - 1] = '\0';

    LOG_I("Download: [%s] %03d%%\033[1A", progress_sign, per);
}

/* handle function, you can store data and so on */
static int http_ota_shard_download_handle(char *buffer, int length)
{
    int ret = RT_EOK;
    /* Write the data to the corresponding partition address */
    if (fal_partition_write(g_dl_part, g_begin_offset, (const uint8_t *)buffer, length) < 0)
    {
        LOG_E("Firmware download failed! Partition (%s) write data error!", g_dl_part->name);
        ret = -RT_ERROR;
        goto __exit;
    }
    g_begin_offset += length;

    print_progress(g_begin_offset, g_file_size);

__exit:
    rt_free(buffer);
    return ret;
}

static int http_ota_fw_download(const char* uri, const char* port)
{
    int ret = RT_EOK;
    struct webclient_session* session = RT_NULL;

    /* create webclient session and set header response size */
    session = webclient_session_create(GET_HEADER_BUFSZ);
    if (!session)
    {
        LOG_E("open uri failed.");
        ret = -RT_ERROR;
        goto __exit;
    }

    /* get the real data length */
    webclient_shard_head_function(session, uri, &g_file_size);

    if (g_file_size == 0)
    {
        LOG_E("Request file size is 0!");
        ret = -RT_ERROR;
        goto __exit;
    }
    else if (g_file_size < 0)
    {
        LOG_E("webclient GET request type is chunked.");
        ret = -RT_ERROR;
        goto __exit;
    }
    LOG_I("OTA file size is (%d)", g_file_size);
    LOG_I("\033[1A");

    /* Get download partition information and erase download partition data */
    if ((g_dl_part = fal_partition_find(port)) == RT_NULL)
    {
        LOG_E("Firmware download failed! Partition (%s) find error!", "download");
        ret = -RT_ERROR;
        goto __exit;
    }

    LOG_I("Start erase flash (%s) partition!", g_dl_part->name);

    if (fal_partition_erase(g_dl_part, 0, g_file_size) < 0)
    {
        LOG_E("Firmware download failed! Partition (%s) erase error!", g_dl_part->name);
        ret = -RT_ERROR;
        goto __exit;
    }
    LOG_I("Erase flash (%s) partition success!", g_dl_part->name);

    /* register the handle function, you can handle data in the function */
    webclient_register_shard_position_function(session, http_ota_shard_download_handle);

    /* the "memory size" that you can provide in the project and uri */
    ret = webclient_shard_position_function(session, uri, g_begin_offset, g_file_size, HTTP_OTA_BUFF_LEN);

    /* clear the handle function */
    webclient_register_shard_position_function(session, RT_NULL);

    if (ret == RT_EOK)
    {
        if (session != RT_NULL)
        {
            webclient_close(session);
            session = RT_NULL;
        }

        LOG_I("\033[0B");
        LOG_I("Download firmware to flash success.");
        LOG_I("System now will restart...");

        rt_thread_delay(rt_tick_from_millisecond(5));

        /* Reset the device, Start new firmware */
        extern void rt_hw_cpu_reset(void);
        rt_hw_cpu_reset();
    }
    else
    {
        LOG_E("Download firmware failed.");
    }

__exit:
    if (session != RT_NULL)
        webclient_close(session);
    g_begin_offset = 0;

    return ret;
}

void THttpOtaTaskRunOnce(tsp_httpota_t *self)
{
    RUN_IF_ELAPSED(last_show_info_time, 30000)
    {
        LOG_D("THttpOtaTaskRunOnce %d", self->ota_fsm);
        SYNC_TIME(last_show_info_time);
    }
    switch(self->ota_fsm)
    {
        case THTTP_OTA_FSM_NET_CONNECT:
        {
            RUN_IF_ELAPSED(last_check_net_time, 5000)
            {
                uint8_t link_state = 0;
                struct at_device *dev= at_device_get_first_initialized();
                at_device_control(dev, AT_DEVICE_CTRL_GET_SIGNAL, &link_state);
                if(link_state == 1)
                {
                    LOG_D("http net connect ok %d", link_state);
                    self->ota_fsm = THTTP_OTA_FSM_IDLE;
                }
                SYNC_TIME(last_check_net_time);
            }
        }
        break;
        case THTTP_OTA_FSM_IDLE:
        {
            RUN_IF_ELAPSED(last_check_ota_state, 30000)
            {
                uint32_t http_ota_state = 0;
                if(self->ota_err_time > 3)
                {
                    self->ota_err_time = 0;
                    TFlashDbSetValue(U32_START_OTA, 0, sizeof(uint32_t));
                    LOG_I("http ota url update error timeover url %s", http_ota_state, self->http_ota_url);
                }
                TFlashDbGetValue(U32_START_OTA, &http_ota_state);
                if(http_ota_state == 1)
                {
                    TFlashDbGetValue(STR_HTTPOTA_URL, self->http_ota_url);
                    LOG_I("http ota url update %d url %s", http_ota_state, self->http_ota_url);
                    self->ota_fsm = THTTP_OTA_FSM_UPDATE_URL;
                }
                SYNC_TIME(last_check_ota_state);
            }
        }
        break;
        case THTTP_OTA_FSM_UPDATE_URL:
        {
            if(self->http_ota_url[0] == 0)
            {
                LOG_E("http ota url is empty");
                self->ota_fsm = THTTP_OTA_FSM_IDLE;
            }
            else
            {
                self->ota_fsm = THTTP_OTA_FSM_DOWNLOAD;
            }
        }
        break;
        case THTTP_OTA_FSM_DOWNLOAD:
        {
            if(RT_EOK != http_ota_fw_download(self->http_ota_url, self->ota_port_name) )
            {
                self->ota_err_time++;
                self->ota_fsm = THTTP_OTA_FSM_IDLE;
            }
        }
        break;
    }
}

void THttpOtaTaskDestory(tsp_httpota_t *self)
{

}
