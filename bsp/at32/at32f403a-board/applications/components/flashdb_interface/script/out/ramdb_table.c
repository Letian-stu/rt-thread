#include "ramdb_table.h"

rt_uint32_t a_u32_value1                   = 0;
rt_uint32_t a_u32_value2                   = 0;
rt_uint32_t a_u32_value3                   = 0;
rt_uint32_t a_u32_value4                   = 0;
char a_u32_array1[17] = {0};
char a_u32_array2[64] = {0};

const fdb_kv_table_t g_ram_kv_table[] = {
    { A_U32_VALUE1             , &a_u32_value1             , sizeof(a_u32_value1) },
    { A_U32_VALUE2             , &a_u32_value2             , sizeof(a_u32_value2) },
    { A_U32_VALUE3             , &a_u32_value3             , sizeof(a_u32_value3) },
    { A_U32_VALUE4             , &a_u32_value4             , sizeof(a_u32_value4) },
    { A_U32_ARRAY1             , &a_u32_array1             , sizeof(a_u32_array1) },
    { A_U32_ARRAY2             , &a_u32_array2             , sizeof(a_u32_array2) },
};

const size_t g_ram_kv_num = sizeof(g_ram_kv_table) / sizeof(g_ram_kv_table[0]);
