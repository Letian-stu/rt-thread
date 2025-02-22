#ifndef _RAMDB_TABLE_H_
#define _RAMDB_TABLE_H_

#include "flashdb_interface.h"
#include "flashdb.h"

#define A_TYPE_U8     "A_U08_"
#define A_TYPE_16     "A_U16_"
#define A_TYPE_32     "A_U32_"
#define A_TYPE_FT     "A_UFT_"
#define A_TYPE_STR    "A_STR_"

#define A_U32_VALUE1              "A_U32_VALUE1"
#define A_U32_VALUE2              "A_U32_VALUE2"
#define A_U32_VALUE3              "A_U32_VALUE3"
#define A_U32_VALUE4              "A_U32_VALUE4"
#define A_U32_ARRAY1              "A_U32_ARRAY1"
#define A_U32_ARRAY2              "A_U32_ARRAY2"

extern const fdb_kv_table_t g_ram_kv_table[];
extern const size_t g_ram_kv_num;

#endif
