#ifndef _FLASHDB_INTERFACE_H_
#define _FLASHDB_INTERFACE_H_

#include <rtthread.h>
#include "rtdevice.h"
#include "flashdb.h"

#define FDB_KVDB_NAME   ("env")  
#define FDB_KVDB_PATH   ("KVDB")         

#define FDB_ATTE_NAME   ("attr")  
#define FDB_ATTE_PATH   ("ATTR")         

typedef struct fdb_default_kv_node fdb_kv_table_t;

typedef struct {
    struct fdb_default_kv flash_kv_list;  
    struct fdb_kvdb flash_kvdb;
    rt_mutex_t flash_kvdb_mutex;   

    struct fdb_default_kv ram_kv_list;  
    struct fdb_kvdb ram_kvdb;
    rt_mutex_t ram_kvdb_mutex;   
    

    rt_mutex_t tsdb_mutex; 
    struct fdb_tsdb tsdb;

} fdb_interface_t;

void TFlashDbMutexCreate(fdb_interface_t *self);

void TRamDbMutexCreate(fdb_interface_t *self);

int TFlashDbSetValue(char *key, void *value, size_t size);

size_t TFlashDbGetValue(char *key, void *value);

int TRamDbSetValue(char *key, void *value, size_t size);

size_t TRamDbGetValue(char *key, void *value);

int TFlashDbTableCreate(fdb_interface_t *self, struct fdb_default_kv_node *fdb, rt_uint32_t num);

int TRamDbTableCreate(fdb_interface_t *self, struct fdb_default_kv_node *fdb, rt_uint32_t num);

#endif
