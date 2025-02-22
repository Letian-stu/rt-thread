#ifndef _FLASHDB_TABLE_H_
#define _FLASHDB_TABLE_H_

#include "flashdb_interface.h"
#include "flashdb.h"

#define O_TYPE_U8     "O_U08_"
#define O_TYPE_16     "O_U16_"
#define O_TYPE_32     "O_U32_"
#define O_TYPE_FT     "O_UFT_"
#define O_TYPE_STR    "O_STR_"

#define O_U32_BOOT_COUNT          "O_U32_BOOT_COUNT"
#define O_U32_SOFT_VERSION        "O_U32_SOFT_VERSION"
#define O_U32_HARD_VERSION        "O_U32_HARD_VERSION"
#define O_STR_VEHICLEVIN          "O_STR_VEHICLEVIN"
#define O_U32_START_OTA           "O_U32_START_OTA"
#define O_STR_HTTPOTA_URL         "O_STR_HTTPOTA_URL"

extern const fdb_kv_table_t g_flash_kv_table[];
extern const size_t g_flash_kv_num;

#endif
