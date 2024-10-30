/*
 * @Date: 2024-07-21 21:45:01
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-10-26 22:47:11
 * @FilePath: \attr\inc\attr_mem.h
 */
#ifndef _ATTR_MEM_H_
#define _ATTR_MEM_H_

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "attr_defconfig.h"

typedef struct _attr_mem
{
    int8_t is_open; //是否开启

    uint16_t data_size; //缓冲区总长度
    char *data; //缓冲区

    int8_t (*lock)(void);
    int8_t (*unlock)(void);
}attr_mem_t;

extern char g_attr_mem_data[ATTR_SUM_DATA_SIZE];

extern attr_mem_t g_attr_mem;

int8_t attr_mem_create(attr_mem_t *self);

int8_t attr_mem_remove(attr_mem_t *self);

uint8_t attr_mem_set(uint32_t pid, void *value, uint8_t len);

uint8_t attr_mem_get(uint32_t pid, void *value, uint8_t len);

#endif
