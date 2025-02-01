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

#endif
