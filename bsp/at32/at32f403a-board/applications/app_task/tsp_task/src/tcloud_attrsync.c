#include "tcloud_attrsync.h"
#include "cJSON.h"
#include "flashdb_table.h"
#include <rtthread.h>
#include "flashdb_interface.h"

#ifdef LOG_TAG 
#define LOG_TAG "TCloud"
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

        if (rt_strncmp(key, TYPE_32, 4) == 0) 
        {
            rt_uint32_t value = 0;
            TFlashDbGetValue(entry->key, &value);
            cJSON_AddNumberToObject(root, key, value);
        } 
        else if (rt_strncmp(key, TYPE_STR, 4) == 0) 
        {
            char *str_value = rt_malloc(entry->value_len+1);
            if(str_value)
            {
                memset(str_value, 0x00, entry->value_len+1);
                TFlashDbGetValue(entry->key, str_value);
                cJSON_AddStringToObject(root, key, str_value);
                rt_free(str_value);
            }else{
                LOG_E("TCloud attr malloc %s", entry->key);
            }
        }
    }
    return root;
}

int TCloudAtteGenerateDbTable(tcloud_attrsync_t *self, char *data)
{
    cJSON *root = cJSON_Parse(data);
    if (!root) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr) {
            LOG_E("JSON parse error before: %s", error_ptr);
        }
        return -1; // 解析失败
    }

    int saved_count = 0;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, root) {
        const char *key = item->string;
        if (!key) continue; 

        if (strncmp(key, TYPE_32, 4) == 0) {
            if (!cJSON_IsNumber(item)) {
                LOG_W("Key '%s' is not a number, skip", key);
                continue;
            }
            rt_uint32_t value = (rt_uint32_t)item->valueint;
            int ret = TFlashDbSetValue((char*)key, &value, sizeof(rt_uint32_t));
            if (ret == 0) 
            {
                saved_count++;
                LOG_D("Saved U32 key: %s = %u", key, value);
            } 
            else 
            {
                LOG_E("Failed to save U32 key: %s (err=%d)", key, ret);
            }
        } 
        else if (strncmp(key, TYPE_STR, 4) == 0) 
        {
            if (!cJSON_IsString(item)) 
            {
                LOG_W("Key '%s' is not a string, skip", key);
                continue;
            }
            const char *str_value = item->valuestring;
            if (!str_value) 
            {
                LOG_W("Key '%s' has empty string, skip", key);
                continue;
            }
            size_t str_len = strlen(str_value) + 1;
            int ret = TFlashDbSetValue((char*)key, (void*)str_value, str_len);
            if (ret == 0) 
            {
                saved_count++;
                LOG_D("Saved STR key: %s = '%s'", key, str_value);
            } 
            else 
            {
                LOG_E("Failed to save STR key: %s (err=%d)", key, ret);
            }
        } else {
            LOG_W("Unknown key type: %s, skip", key);
        }
    }
    cJSON_Delete(root);
    return saved_count;
}


void TCloudAttrSyncTableInit(tcloud_attrsync_t *self)
{
    self->kv_table = (fdb_kv_table_t *)g_kv_table;
    self->fdb_kv_num = g_kv_num;
    LOG_D("TCloud attr report num %d", self->fdb_kv_num);
}

void TCloudAttrSyncTableCreate(tcloud_attrsync_t *self)
{
    cJSON *generate_json = TCloudAtteGenerateJsonTable(self);
    if (!generate_json) {
        LOG_E("TCloud attr Failed to create JSON");
        return;
    }
    self->fdb_kv_json_str = cJSON_PrintUnformatted(generate_json);
    if (self->fdb_kv_json_str) 
    {
        LOG_D("TCloud attr Generated JSON: [%s]", self->fdb_kv_json_str);
    }
    cJSON_Delete(generate_json);
}

void TCloudAttrSyncTableFree(tcloud_attrsync_t *self)
{
    if(self->fdb_kv_json_str != NULL)
        cJSON_free(self->fdb_kv_json_str);
}
