#include "tcloud_attrsync.h"
#include "cJSON.h"
#include "flashdb_table.h"
#include <rtthread.h>
#include "flashdb_interface.h"

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "tcloud"
#endif
#define LOG_LVL     LOG_LVL_DBG  

#include <ulog.h>  


static cJSON* TCloudAtteGenerateJsonTable(tcloud_attrsync_t *self)
{
    cJSON *root = cJSON_CreateObject();
    if (!root) {
        return NULL;
    }

    for (size_t i = 0; i < self->fdb_kv_num; i++) {
        fdb_kv_table_t *entry = &g_kv_table[i];
        const char *key = entry->key;

        if (rt_strncmp(key, "U32_", 4) == 0) 
        {
            rt_uint32_t value = 0;
            TFlashDbGetValue(entry->key, &value);
            cJSON_AddNumberToObject(root, key, value);
        } 
        else if (rt_strncmp(key, "STR_", 4) == 0) 
        {
            char *str_value = rt_malloc(entry->value_len);
            if(str_value)
            {
                TFlashDbGetValue(STR_VEHICLEVIN, str_value);
                cJSON_AddStringToObject(root, key, str_value);
                rt_free(str_value);
            }
        }
    }
    return root;
}

void TcloudAttrSyncTableInit(tcloud_attrsync_t *self)
{
    self->kv_table = (fdb_kv_table_t *)g_kv_table;
    self->fdb_kv_num = sizeof(g_kv_table)/sizeof(g_kv_table[0]);
    LOG_D("tcloud attr report num %d", self->fdb_kv_num);
}

void TcloudAttrSyncTableCreate(tcloud_attrsync_t *self)
{
    cJSON *generate_json = TCloudAtteGenerateJsonTable(self);
    if (!generate_json) {
        LOG_E("tcloud attr Failed to create JSON");
        return;
    }
    self->fdb_kv_json_str = cJSON_PrintUnformatted(generate_json);
    if (self->fdb_kv_json_str) 
    {
        LOG_D("tcloud attr Generated JSON: [%s]", self->fdb_kv_json_str);
    }
    cJSON_Delete(generate_json);
}

void TcloudAttrSyncTableFree(tcloud_attrsync_t *self)
{
    if(self->fdb_kv_json_str != NULL)
        cJSON_free(self->fdb_kv_json_str);
}

