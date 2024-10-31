#ifndef _ATTR_H_
#define _ATTR_H_

#include "stdio.h"
#include "stdint.h"
#include "attr_defconfig.h"

typedef enum{
    ATTR_SUCCESS = 0,
    ATTR_FAIL,
    ATTR_TYPE_ERR,
}ATTR_ERR_E;

typedef struct _attr
{
    int8_t is_open; //是否开启

    int16_t attr_sum; //属性使用总数
    int16_t attr_len; //属性使用空间
}attr_t;

extern attr_t attr;

attr_t *attr_self(void);

int8_t attr_create(attr_t *self);

int8_t attr_rmeove(attr_t *self);

uint8_t attr_set_i8(uint32_t pid, int8_t value);

uint8_t attr_set_u8(uint32_t pid, uint8_t value);

uint8_t attr_set_i16(uint32_t pid, int16_t value);

uint8_t attr_set_u16(uint32_t pid, uint16_t value);

uint8_t attr_set_i32(uint32_t pid, int32_t value);

uint8_t attr_set_u32(uint32_t pid, uint32_t value);

uint8_t attr_set_float(uint32_t pid, float value);

uint8_t attr_set_mem(uint32_t pid, char *value, uint8_t len);

uint8_t attr_get_i8(uint32_t pid, int8_t *value);

uint8_t attr_get_u8(uint32_t pid, uint8_t *value);

uint8_t attr_get_i16(uint32_t pid, int16_t *value);

uint8_t attr_get_u16(uint32_t pid, uint16_t *value);

uint8_t attr_get_i32(uint32_t pid, int32_t *value);

uint8_t attr_get_u32(uint32_t pid, uint32_t *value);

uint8_t attr_get_float(uint32_t pid, float *value);

uint8_t attr_get_mem(uint32_t pid, char *value, uint8_t len);


#endif
