/*
 * @Date: 2024-07-21 21:45:01
 * @LastEditors: StuTian 1656733975@qq.com
 * @LastEditTime: 2024-07-27 00:13:44
 * @FilePath: \attr\inc\attr_mem.h
 */
#ifndef _ATTR_MEM_H_
#define _ATTR_MEM_H_

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "attr.h"
#include "attr_defconfig.h"

typedef struct _attr_mem
{
    int8_t is_open; //是否开启

    uint16_t data_size; //缓冲区总长度
    char *data; //缓冲区

    void (*lock)(void);
    void (*unlock)(void);
}attr_mem_t;

char *attr_mem_data(void);

uint8_t attr_mem_set(uint32_t pid, void *value, uint8_t len);

uint8_t attr_mem_get(uint32_t pid, void *value, uint8_t len);

#endif