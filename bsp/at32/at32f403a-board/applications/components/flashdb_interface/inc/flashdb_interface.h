#ifndef _FLASHDB_INTERFACE_H_
#define _FLASHDB_INTERFACE_H_

#include <rtthread.h>
#include "rtdevice.h"
#include "flashdb.h"

#define FDB_KVDB_NAME   ("env")             
#define FDB_KVDB_PATH   ("fdb_kvdb1")         

typedef struct {
    struct fdb_default_kv kv_list;  
    rt_mutex_t kvdb_mutex;   
    struct fdb_kvdb kvdb;

    rt_mutex_t tsdb_mutex; 
    struct fdb_tsdb tsdb;

} fdb_interface_t;

void TFlashDbMutexCreate(fdb_interface_t *self);

int TFlashDbSetValue(char *key, void *value, size_t size);

size_t TFlashDbGetValue(char *key, void *value);

int TFlashDbTableCreate(fdb_interface_t *self, struct fdb_default_kv_node *fdb, rt_uint32_t num);

#endif
