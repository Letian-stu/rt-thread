/*
 * @Descripttion: 
 * @version: 
 * @Author: letian
 * @Date: 2024-10-29 22:59:50
 * @LastEditTime: 2024-11-07 22:35:52
 */
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"
#include <ulog.h>  

#include "drv_spi.h"
#include "spi_flash_sfud.h"
#include "fal.h"
#include "flashdb.h"
#include "dfs_fs.h"
 
#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "main"
#endif
#define LOG_LVL     LOG_LVL_DBG   

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
}
INIT_COMPONENT_EXPORT(rt_fal_init);

static uint32_t boot_count = 0;
struct fdb_kvdb kvdb = {0};
struct fdb_tsdb tsdb = {0};

extern void kvdb_basic_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_string_sample(fdb_kvdb_t kvdb);
extern void kvdb_type_blob_sample(fdb_kvdb_t kvdb);
extern void tsdb_sample(fdb_tsdb_t kvdb);

static struct fdb_default_kv_node default_kv_table[] = {
    {"boot_count", &boot_count, sizeof(boot_count)},
};

static fdb_time_t get_time(void)
{
    static int cur_times = 0;
    cur_times += 1;
    return cur_times;
}

int flashdb_init(void)
{
		fdb_err_t result = 0;
#ifdef FDB_USING_KVDB
    { /* KVDB Sample */
        struct fdb_default_kv default_kv;

        default_kv.kvs = default_kv_table;
        default_kv.num = sizeof(default_kv_table) / sizeof(default_kv_table[0]);
        /* set the lock and unlock function if you want */
//        fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_LOCK, lock);
//        fdb_kvdb_control(&kvdb, FDB_KVDB_CTRL_SET_UNLOCK, unlock);
        /* Key-Value database initialization
         *
         *       &kvdb: database object
         *       "env": database name
         * "fdb_kvdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         * &default_kv: The default KV nodes. It will auto add to KVDB when first initialize successfully.
         *        NULL: The user data if you need, now is empty.
         */
        result = fdb_kvdb_init(&kvdb, "env", "fdb_kvdb1", &default_kv, NULL);

        if (result != FDB_NO_ERR) {
            return -1;
        }

        /* run basic KV samples */
        kvdb_basic_sample(&kvdb);
        /* run string KV samples */
        kvdb_type_string_sample(&kvdb);
        /* run blob KV samples */
        kvdb_type_blob_sample(&kvdb);
    }
#endif /* FDB_USING_KVDB */

#ifdef FDB_USING_TSDB
    { /* TSDB Sample */
        /* set the lock and unlock function if you want */
//        fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_LOCK, lock);
//        fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_UNLOCK, unlock);
        /* Time series database initialization
         *
         *       &tsdb: database object
         *       "log": database name
         * "fdb_tsdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
         *              Please change to YOUR partition name.
         *    get_time: The get current timestamp function.
         *         128: maximum length of each log
         *        NULL: The user data if you need, now is empty.
         */
        result = fdb_tsdb_init(&tsdb, "log", "fdb_tsdb1", get_time, 128, NULL);
        /* read last saved time for simulated timestamp */
        uint32_t counts = 0;
        fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_GET_LAST_TIME, &counts);

        if (result != FDB_NO_ERR) {
            return -1;
        }

        /* run TSDB sample */
        tsdb_sample(&tsdb);
    }
#endif /* FDB_USING_TSDB */
}
INIT_ENV_EXPORT(flashdb_init);


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

int main(void)
{
		show_version();
    while (1)
    {
        rt_thread_mdelay(1000);
    }
}
