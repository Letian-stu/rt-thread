#include "flashdb_table.h"

rt_uint32_t o_u32_boot_count               = 0;
rt_uint32_t o_u32_soft_version             = 0;
rt_uint32_t o_u32_hard_version             = 0;
char o_str_vehiclevin[17] = {0};
rt_uint32_t o_u32_start_ota                = 0;
char o_str_httpota_url[64] = {0};

const fdb_kv_table_t g_flash_kv_table[] = {
    { O_U32_BOOT_COUNT         , &o_u32_boot_count         , sizeof(o_u32_boot_count) },
    { O_U32_SOFT_VERSION       , &o_u32_soft_version       , sizeof(o_u32_soft_version) },
    { O_U32_HARD_VERSION       , &o_u32_hard_version       , sizeof(o_u32_hard_version) },
    { O_STR_VEHICLEVIN         , &o_str_vehiclevin         , sizeof(o_str_vehiclevin) },
    { O_U32_START_OTA          , &o_u32_start_ota          , sizeof(o_u32_start_ota) },
    { O_STR_HTTPOTA_URL        , &o_str_httpota_url        , sizeof(o_str_httpota_url) },
};

const size_t g_flash_kv_num = sizeof(g_flash_kv_table) / sizeof(g_flash_kv_table[0]);
