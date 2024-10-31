#include "../inc/attr_mem.h"

char g_attr_mem[ATTR_SUM_DATA_SIZE] = {0};

static uint16_t ATTR_REAL_ADDR[ATTR_MAX] = {ATTR_UINT8_START_ADDR,ATTR_INT8_START_ADDR,ATTR_UINT16_START_ADDR,ATTR_INT16_START_ADDR,
ATTR_UINT32_START_ADDR,ATTR_INT32_START_ADDR,ATTR_FLOAT_START_ADDR,ATTR_MEM_START_ADDR };

attr_mem_t attr_mem = {0};

char *attr_mem_data(void)
{
    return (char *)g_attr_mem;
}

attr_mem_t *attr_mem_self(void)
{
    return (attr_mem_t *)&attr_mem;
}

static int8_t attr_lock_create(void)
{

}

static int8_t attr_lock_remove(void)
{

}

static void attr_lock(void)
{

}

static void attr_unlock(void)
{

}

int8_t attr_mem_create(attr_mem_t *self)
{
    ATTR_ERR_E res = ATTR_SUCCESS;
    if(self == NULL)
        return ATTR_FAIL;

    if(self->is_open == 0)
    {
        self->is_open = 1;
        self->data_size = ATTR_SUM_DATA_SIZE;
        self->data = (char *)g_attr_mem;

        attr_lock_create();
        self->lock = attr_lock;
        self->unlock = attr_unlock;

        memset(self->data, 0x00, ATTR_SUM_DATA_SIZE );
    }
    return res;
}

int8_t attr_mem_remove(attr_mem_t *self)
{
    ATTR_ERR_E res = ATTR_SUCCESS;
    if(self == NULL)
        return ATTR_FAIL;

    if(self->is_open)
    {
        attr_lock_remove();
        self->is_open = 0;
        self->data_size = 0;
        self->data = NULL;
        memset(self->data, 0x00, ATTR_SUM_DATA_SIZE );
    }
    return res;
}

uint8_t attr_mem_set(uint32_t pid, void *value, uint8_t len)
{
    ATTR_TYPE_E pid_type = ATTR_GET_TYPE(pid);
    uint16_t pid_addr = ATTR_GET_ADDR(pid);
    uint16_t real_addr = ATTR_REAL_ADDR[pid_type] + pid_addr;
    attr_mem_t *attr_mem = attr_mem_self(); 
    attr_mem->lock();
    memcpy((char *)&attr_mem->data[real_addr], value, len);
    attr_mem->unlock();
}

uint8_t attr_mem_get(uint32_t pid, void *value, uint8_t len)
{
    ATTR_TYPE_E pid_type = ATTR_GET_TYPE(pid);
    uint16_t pid_addr = ATTR_GET_ADDR(pid);
    uint16_t real_addr = ATTR_REAL_ADDR[pid_type] + pid_addr;
    attr_mem_t *attr_mem = attr_mem_self(); 

    attr_mem->lock();
    memcpy(value, (char *)&attr_mem->data[real_addr], len);
    attr_mem->unlock();
}
