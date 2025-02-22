#include "flashdb_table.h"

rt_uint32_t u32_boot_count                 = 0;
rt_uint32_t u32_soft_version               = 0;
rt_uint32_t u32_hard_version               = 0;
char str_vehiclevin[17] = {0};
rt_uint32_t u32_start_ota                  = 0;
char str_httpota_url[64] = {0};

const fdb_kv_table_t g_kv_table[] = {
    { U32_BOOT_COUNT           , &u32_boot_count           , sizeof(u32_boot_count) },
    { U32_SOFT_VSERION         , &u32_soft_version         , sizeof(u32_soft_version) },
    { U32_HARD_VSERION         , &u32_hard_version         , sizeof(u32_hard_version) },
    { STR_VEHICLEVIN           , &str_vehiclevin           , sizeof(str_vehiclevin) },
    { U32_START_OTA            , &u32_start_ota            , sizeof(u32_start_ota) },
    { STR_HTTPOTA_URL          , &str_httpota_url          , sizeof(str_httpota_url) },
};

const size_t g_kv_num = sizeof(g_kv_table) / sizeof(g_kv_table[0]);
