/*
 * @Date: 2024-07-21 21:31:50
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-10-26 22:32:20
 * @FilePath: \attr\src\attr.c
 */
#include "attr.h"

attr_t g_attr = {0};

int8_t attr_create(void)
{
    ATTR_ERR_E res = ATTR_SUCCESS;

    attr_t *self = (attr_t *)&g_attr;

    if(self == NULL){
        return ATTR_FAIL;
    }

    self->attr_mem = (attr_mem_t *)&g_attr_mem;

    if(!self->is_open)
    {
        self->attr_sum = ATTR_SUM_DATA_NUM;
        self->attr_len = ATTR_SUM_DATA_SIZE;

        attr_mem_create(self->attr_mem);
    }
    return res;
}

int8_t attr_rmeove(void)
{
    ATTR_ERR_E res = ATTR_SUCCESS;

    attr_t *self = (attr_t *)&g_attr;

    if(self == NULL){
        return ATTR_FAIL;
    }
    
    if(self->is_open)
    {
        attr_mem_remove(self->attr_mem);
        self->attr_sum = ATTR_SUM_DATA_NUM;
        self->attr_len = ATTR_SUM_DATA_SIZE;
    }
    return res;
}

uint8_t attr_set_i8(uint32_t pid, int8_t value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_INT8)
        return attr_mem_set(pid, (char *)&value, sizeof(int8_t)); 
    else
        return ATTR_TYPE_ERR;
}

uint8_t attr_set_u8(uint32_t pid, uint8_t value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_UINT8)
        return attr_mem_set(pid, (char *)&value, sizeof(uint8_t)); 
    else
        return ATTR_TYPE_ERR;
}

uint8_t attr_set_i16(uint32_t pid, int16_t value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_INT16)
        return attr_mem_set(pid, (char *)&value, sizeof(int16_t)); 
    else
        return ATTR_TYPE_ERR;
}

uint8_t attr_set_u16(uint32_t pid, uint16_t value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_UINT16)
        return attr_mem_set(pid, (char *)&value, sizeof(uint16_t)); 
    else
        return ATTR_TYPE_ERR;
}

uint8_t attr_set_i32(uint32_t pid, int32_t value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_INT32)
        return attr_mem_set(pid, (char *)&value, sizeof(int32_t)); 
    else
        return ATTR_TYPE_ERR;
}

uint8_t attr_set_u32(uint32_t pid, uint32_t value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_UINT32)
        return attr_mem_set(pid, (char *)&value, sizeof(uint32_t)); 
    else
        return ATTR_TYPE_ERR;
}

uint8_t attr_set_float(uint32_t pid, float value)
{    
    if(ATTR_GET_TYPE(pid) == ATTR_FLOAT)
        return attr_mem_set(pid, (char *)&value, sizeof(float)); 
    else
        return ATTR_TYPE_ERR;
}

uint8_t attr_set_mem(uint32_t pid, char *value, uint8_t len)
{
    if(ATTR_GET_TYPE(pid) == ATTR_MEM)
        return attr_mem_set(pid, (char *)value, len); 
    else
        return ATTR_TYPE_ERR;    
}

uint8_t attr_get_i8(uint32_t pid, int8_t *value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_INT8)
        return attr_mem_get(pid, (char *)value, sizeof(int8_t)); 
    else
        return ATTR_TYPE_ERR;      
}

uint8_t attr_get_u8(uint32_t pid, uint8_t *value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_UINT8)
        return attr_mem_get(pid, (char *)value, sizeof(uint8_t)); 
    else
        return ATTR_TYPE_ERR;  
}

uint8_t attr_get_i16(uint32_t pid, int16_t *value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_INT16)
        return attr_mem_get(pid, (char *)value, sizeof(int16_t)); 
    else
        return ATTR_TYPE_ERR;  
}

uint8_t attr_get_u16(uint32_t pid, uint16_t *value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_UINT16)
        return attr_mem_get(pid, (char *)value, sizeof(uint16_t)); 
    else
        return ATTR_TYPE_ERR;  
}

uint8_t attr_get_i32(uint32_t pid, int32_t *value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_INT32)
        return attr_mem_get(pid, (char *)value, sizeof(int32_t)); 
    else
        return ATTR_TYPE_ERR;  
}

uint8_t attr_get_u32(uint32_t pid, uint32_t *value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_UINT32)
        return attr_mem_get(pid, (char *)value, sizeof(uint32_t)); 
    else
        return ATTR_TYPE_ERR;  
}

uint8_t attr_get_float(uint32_t pid, float *value)
{
    if(ATTR_GET_TYPE(pid) == ATTR_FLOAT)
        return attr_mem_get(pid, (char *)value, sizeof(float)); 
    else
        return ATTR_TYPE_ERR;  
}

uint8_t attr_get_mem(uint32_t pid, char *value, uint8_t len)
{
    if(ATTR_GET_TYPE(pid) == ATTR_MEM)
        return attr_mem_get(pid, (char *)value, len); 
    else
        return ATTR_TYPE_ERR;   
}
