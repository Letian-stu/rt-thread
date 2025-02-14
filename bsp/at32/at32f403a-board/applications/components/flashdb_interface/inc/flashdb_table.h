#ifndef _FLASHDB_TABLE_H_
#define _FLASHDB_TABLE_H_

#include "flashdb_interface.h"
#include "flashdb.h"

#define TYPE_U8     "U08_"
#define TYPE_16     "U16_"
#define TYPE_32     "U32_"
#define TYPE_FT     "UFT_"
#define TYPE_STR    "STR_"

#define KV_DB_NUM (6)

#define U32_BOOT_COUNT          "U32_BOOT_COUNT"
#define U32_SOFT_VSERION        "U32_SOFT_VSERION"
#define U32_HARD_VSERION        "U32_HARD_VSERION"
#define U32_DEVICE_SN           "U32_DEVICE_SN"
#define U32_MANUFACTURE_TIME    "U32_MANUFACTURE_TIME"
#define STR_VEHICLEVIN          "STR_VEHICLEVIN"

typedef struct fdb_default_kv_node fdb_kv_table_t;

extern fdb_kv_table_t g_kv_table[KV_DB_NUM];

#endif
