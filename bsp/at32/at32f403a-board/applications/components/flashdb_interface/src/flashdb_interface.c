#include "flashdb_interface.h"
#include "flashdb_table.h"
#include "ramdb_table.h"

#ifdef LOG_TAG 
#define LOG_TAG "fdb_interface"
#endif
#define LOG_LVL     LOG_LVL_DBG  

#include <ulog.h>  

fdb_interface_t g_fdbkv_interface = {0};

void TFlashDbMutexCreate(fdb_interface_t *self)
{
    self->flash_kvdb_mutex = rt_mutex_create("flash_kvdb_mutex", RT_IPC_FLAG_FIFO);
    if (self->flash_kvdb_mutex == RT_NULL)
    {
        LOG_E("flash_kvdb_mutex create failed!");
    }
}

static void TFlashDbMutexLock(void)
{
    rt_mutex_take(g_fdbkv_interface.flash_kvdb_mutex, RT_WAITING_FOREVER);
}

static void TFlashDbMutexUnlock(void)
{
    rt_mutex_release(g_fdbkv_interface.flash_kvdb_mutex);
}

void TRamDbMutexCreate(fdb_interface_t *self)
{
    self->ram_kvdb_mutex = rt_mutex_create("ram_kvdb_mutex", RT_IPC_FLAG_FIFO);
    if (self->ram_kvdb_mutex == RT_NULL)
    {
        LOG_E("ram_kvdb_mutex create failed!");
    }
}

static void TRamDbMutexLock(void)
{
    rt_mutex_take(g_fdbkv_interface.ram_kvdb_mutex, RT_WAITING_FOREVER);
}

static void TRamDbMutexUnlock(void)
{
    rt_mutex_release(g_fdbkv_interface.ram_kvdb_mutex);
}

int TFlashDbSetValue(char *key, void *value, size_t size)
{
	fdb_err_t rst = 0;
    struct fdb_kvdb* kvdb = &g_fdbkv_interface.flash_kvdb;
    struct fdb_default_kv_node* kv_table = g_fdbkv_interface.flash_kv_list.kvs;
    rt_uint32_t kv_num = g_fdbkv_interface.flash_kv_list.num;

	for(int i = 0; i < kv_num; i++)
	{
		if(!rt_strcmp(key, kv_table[i].key))
		{
			struct fdb_blob blob = {0};
			fdb_blob_make(&blob, value, size);
			
			rst = fdb_kv_set_blob(kvdb, key, &blob);
			if (rst == FDB_NO_ERR && size <= sizeof(rt_uint32_t)) {
				LOG_D("set the val '%s' value is %d", kv_table[i].key, *(rt_uint32_t *)value);
			} 
			else if(rst == FDB_NO_ERR && size > sizeof(rt_uint32_t)) {
				LOG_D("set the str '%s' len %d", kv_table[i].key, size);
                LOG_HEX(LOG_TAG, 16, (rt_uint8_t *)value, size);
			}
			else {
				LOG_D("set the '%s' failed");
			}   
		}
	}
    return rst;
}

size_t TFlashDbGetValue(char *key, void *value)
{
	size_t rst = 0;
    struct fdb_kvdb* kvdb = &g_fdbkv_interface.flash_kvdb;
    struct fdb_default_kv_node* kv_table = g_fdbkv_interface.flash_kv_list.kvs;
    rt_uint32_t kv_num = g_fdbkv_interface.flash_kv_list.num;

	for(int i = 0; i < kv_num; i++)
	{
		if(!rt_strcmp(key, kv_table[i].key))
		{
			struct fdb_blob blob = {0};
			fdb_blob_make(&blob, value, kv_table[i].value_len);
			
			rst = fdb_kv_get_blob(kvdb, key, &blob);
			if (rst <= sizeof(rt_uint32_t)) {
				LOG_D("get the val '%s' value is %d", kv_table[i].key, *(rt_uint32_t *)value);
			} 
			else if(rst > sizeof(rt_uint32_t)){
				LOG_D("get the str '%s' len %d", kv_table[i].key, rst);
                LOG_HEX(LOG_TAG, 16, (rt_uint8_t *)value, rst);
			}
			else {
				LOG_D("get the '%s' failed");
			}   
		}
	}
    return rst;
}

int TRamDbSetValue(char *key, void *value, size_t size)
{
	fdb_err_t rst = 0;
    struct fdb_kvdb* kvdb = &g_fdbkv_interface.ram_kvdb;
    struct fdb_default_kv_node* kv_table = g_fdbkv_interface.ram_kv_list.kvs;
    rt_uint32_t kv_num = g_fdbkv_interface.ram_kv_list.num;

	for(int i = 0; i < kv_num; i++)
	{
		if(!rt_strcmp(key, kv_table[i].key))
		{
			struct fdb_blob blob = {0};
			fdb_blob_make(&blob, value, size);
			
			rst = fdb_kv_set_blob(kvdb, key, &blob);
			if (rst == FDB_NO_ERR && size <= sizeof(rt_uint32_t)) {
				LOG_D("set the val '%s' value is %d", kv_table[i].key, *(rt_uint32_t *)value);
			} 
			else if(rst == FDB_NO_ERR && size > sizeof(rt_uint32_t)) {
				LOG_D("set the str '%s' len %d", kv_table[i].key, size);
                LOG_HEX(LOG_TAG, 16, (rt_uint8_t *)value, size);
			}
			else {
				LOG_D("set the '%s' failed");
			}   
		}
	}
    return rst;
}

size_t TRamDbGetValue(char *key, void *value)
{
	size_t rst = 0;
    struct fdb_kvdb* kvdb = &g_fdbkv_interface.ram_kvdb;
    struct fdb_default_kv_node* kv_table = g_fdbkv_interface.ram_kv_list.kvs;
    rt_uint32_t kv_num = g_fdbkv_interface.ram_kv_list.num;

	for(int i = 0; i < kv_num; i++)
	{
		if(!rt_strcmp(key, kv_table[i].key))
		{
			struct fdb_blob blob = {0};
			fdb_blob_make(&blob, value, kv_table[i].value_len);
			
			rst = fdb_kv_get_blob(kvdb, key, &blob);
			if (rst <= sizeof(rt_uint32_t)) {
				LOG_D("get the val '%s' value is %d", kv_table[i].key, *(rt_uint32_t *)value);
			} 
			else if(rst > sizeof(rt_uint32_t)){
				LOG_D("get the str '%s' len %d", kv_table[i].key, rst);
                LOG_HEX(LOG_TAG, 16, (rt_uint8_t *)value, rst);
			}
			else {
				LOG_D("get the '%s' failed");
			}   
		}
	}
    return rst;
}

int TFlashDbTableCreate(fdb_interface_t *self, struct fdb_default_kv_node *fdb, rt_uint32_t num)
{
    if(fdb == RT_NULL)
        return -1;

    self->flash_kv_list.kvs = fdb;
    self->flash_kv_list.num = num;    
	return 0;
}

int TRamDbTableCreate(fdb_interface_t *self, struct fdb_default_kv_node *fdb, rt_uint32_t num)
{
    if(fdb == RT_NULL)
        return -1;

    self->ram_kv_list.kvs = fdb;
    self->ram_kv_list.num = num;    
	return 0;
}

int TFlashDbCompCreate(void)
{
    TFlashDbTableCreate(&g_fdbkv_interface, g_flash_kv_table, g_flash_kv_num );
    TFlashDbMutexCreate(&g_fdbkv_interface);
    fdb_kvdb_control(&g_fdbkv_interface.flash_kvdb, FDB_KVDB_CTRL_SET_LOCK, TFlashDbMutexLock);
    fdb_kvdb_control(&g_fdbkv_interface.flash_kvdb, FDB_KVDB_CTRL_SET_UNLOCK, TFlashDbMutexUnlock);
    fdb_kvdb_init(&g_fdbkv_interface.flash_kvdb, FDB_KVDB_NAME, FDB_KVDB_PATH, &g_fdbkv_interface.flash_kv_list, NULL);

	TRamDbTableCreate(&g_fdbkv_interface, g_ram_kv_table, g_ram_kv_num );
    TRamDbMutexCreate(&g_fdbkv_interface);
    fdb_kvdb_control(&g_fdbkv_interface.ram_kvdb, FDB_KVDB_CTRL_SET_LOCK, TRamDbMutexLock);
    fdb_kvdb_control(&g_fdbkv_interface.ram_kvdb, FDB_KVDB_CTRL_SET_UNLOCK, TRamDbMutexUnlock);
    fdb_kvdb_init(&g_fdbkv_interface.ram_kvdb, FDB_ATTE_NAME, FDB_ATTE_PATH, &g_fdbkv_interface.ram_kv_list, NULL);

    // rt_uint32_t get_val = 0;
    // TFlashDbGetValue(U32_BOOT_COUNT, &get_val);
    // get_val += 1;
    // TFlashDbSetValue(U32_BOOT_COUNT, &get_val, sizeof(rt_uint32_t));

    // rt_uint32_t get_val2 = 0;
    // TRamDbGetValue(RAM_U32_BOOT_COUNT, &get_val2);
	// get_val2=get_val;
    // TRamDbSetValue(RAM_U32_BOOT_COUNT, &get_val2, sizeof(rt_uint32_t));
    // TRamDbGetValue(RAM_U32_BOOT_COUNT, &get_val2);


//    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_LOCK, kvdb_lock);
//    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_UNLOCK, kvdb_unlock);
//    fdb_tsdb_init(&tsdb, "log", "fdb_tsdb1", get_time, 128, NULL);
}
INIT_ENV_EXPORT(TFlashDbCompCreate);
