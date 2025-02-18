#ifndef _TCLOUD_ATTRSYNC_H_
#define _TCLOUD_ATTRSYNC_H_

#include <rtthread.h>
#include "paho_mqtt.h"
#include "flashdb_table.h"

typedef struct tcloud_attrsync{
    fdb_kv_table_t *kv_table;
    uint32_t fdb_kv_num;

    char *fdb_kv_json_str;
}tcloud_attrsync_t;

int TCloudAtteGenerateDbTable(tcloud_attrsync_t *self, char *data);

void TCloudAttrSyncTableInit(tcloud_attrsync_t *self);

void TCloudAttrSyncTableCreate(tcloud_attrsync_t *self);

void TCloudAttrSyncTableFree(tcloud_attrsync_t *self);

#endif
