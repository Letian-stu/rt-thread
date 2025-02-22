#ifndef _FLASHDB_TABLE_H_
#define _FLASHDB_TABLE_H_

#include "flashdb_interface.h"
#include "flashdb.h"

#define TYPE_U8     "U08_"
#define TYPE_16     "U16_"
#define TYPE_32     "U32_"
#define TYPE_FT     "UFT_"
#define TYPE_STR    "STR_"

#define U32_BOOT_COUNT            "U32_BOOT_COUNT"
#define U32_SOFT_VSERION          "U32_SOFT_VSERION"
#define U32_HARD_VSERION          "U32_HARD_VSERION"
#define STR_VEHICLEVIN            "STR_VEHICLEVIN"
#define U32_START_OTA             "U32_START_OTA"
#define STR_HTTPOTA_URL           "STR_HTTPOTA_URL"

typedef struct fdb_default_kv_node fdb_kv_table_t;

extern const fdb_kv_table_t g_kv_table[];
extern const size_t g_kv_num;

#endif
