#ifndef _FLASHDB_TABLE_H_
#define _FLASHDB_TABLE_H_

#include "flashdb_interface.h"
#include "flashdb.h"

#define U32_BOOT_COUNT          "U32_BOOT_COUNT"
#define U32_SOFT_VSERION        "U32_SOFT_VSERION"
#define U32_HARD_VSERION        "U32_HARD_VSERION"
#define U32_DEVICE_SN           "U32_DEVICE_SN"
#define U32_MANUFACTURE_TIME    "U32_MANUFACTURE_TIME"
#define STR_VEHICLEVIN          "STR_VEHICLEVIN"

rt_uint32_t u32_boot_count = 0;
rt_uint32_t u32_soft_vserion = 0;
rt_uint32_t u32_hard_vserion = 0;
rt_uint32_t u32_device_sn = 0;
rt_uint32_t u32_manufacture_time = 0;
char str_vehiclevin[17] = {0};

struct fdb_default_kv_node g_kv_table[] = {
    {U32_BOOT_COUNT,          &u32_boot_count,        sizeof(u32_boot_count)},
    {U32_SOFT_VSERION,        &u32_soft_vserion,      sizeof(u32_soft_vserion)},
    {U32_HARD_VSERION,        &u32_hard_vserion,      sizeof(u32_hard_vserion)},
    {U32_DEVICE_SN,           &u32_device_sn,         sizeof(u32_device_sn)},
    {U32_MANUFACTURE_TIME,    &u32_manufacture_time,  sizeof(u32_manufacture_time)},
    {STR_VEHICLEVIN,          &str_vehiclevin,        sizeof(str_vehiclevin)},
};

#endif
