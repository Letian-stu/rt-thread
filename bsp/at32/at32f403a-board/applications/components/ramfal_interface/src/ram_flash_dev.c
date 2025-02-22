#include <rtthread.h>
#include "drv_common.h"

#ifdef BSP_USING_RAM_FLASH
#include <string.h>

#if defined(RT_USING_FAL)
#include "fal.h"
#endif

//#define DRV_DEBUG
#define LOG_TAG                "drv.ramflash"
#include <drv_log.h>

// 定义模拟Flash的RAM空间
#define RAM_FLASH_SIZE         (8 * 1024)    // 8KB
#define RAM_FLASH_PAGE_SIZE    (256)         // 256 bytes per page
static uint8_t ram_flash_space[RAM_FLASH_SIZE];

/**
 * @brief read data from ram flash.
 * @note this operation's units is byte.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
static int ram_flash_read(rt_uint32_t addr, rt_uint8_t *buf, rt_uint32_t size)
{
    if ((addr + size) > RAM_FLASH_SIZE)
    {
        LOG_E("read outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -RT_EINVAL;
    }

    memcpy(buf, ram_flash_space + addr, size);
    return size;
}

/**
 * @brief write data to ram flash.
 * @note this operation's units is byte.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
static int ram_flash_write(rt_uint32_t addr, const rt_uint8_t *buf, rt_uint32_t size)
{
    if ((addr + size) > RAM_FLASH_SIZE)
    {
        LOG_E("write outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -RT_EINVAL;
    }

    memcpy(ram_flash_space + addr, buf, size);
    return size;
}

/**
 * @brief erase data on ram flash.
 * @note this operation's units is page.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
static int ram_flash_erase(rt_uint32_t addr, rt_uint32_t size)
{
    if ((addr + size) > RAM_FLASH_SIZE)
    {
        LOG_E("erase outrange flash size! addr is (0x%p)", (void *)(addr + size));
        return -RT_EINVAL;
    }

    // 擦除操作将内存填充为0xFF
    memset(ram_flash_space + addr, 0xFF, size);
    return size;
}

#if defined(RT_USING_FAL)

static int fal_ram_flash_read(long offset, rt_uint8_t *buf, rt_uint32_t size);
static int fal_ram_flash_write(long offset, const rt_uint8_t *buf, rt_uint32_t size);
static int fal_ram_flash_erase(long offset, rt_uint32_t size);

const struct fal_flash_dev ram_flash_dev = 
{
    "ram_flash",
    0,
    RAM_FLASH_SIZE,
    RAM_FLASH_PAGE_SIZE,
    {
        NULL,
        fal_ram_flash_read,
        fal_ram_flash_write,
        fal_ram_flash_erase
    }
};

static int fal_ram_flash_read(long offset, rt_uint8_t *buf, rt_uint32_t size)
{
    return ram_flash_read(offset, buf, size);
}

static int fal_ram_flash_write(long offset, const rt_uint8_t *buf, rt_uint32_t size)
{
    return ram_flash_write(offset, buf, size);
}

static int fal_ram_flash_erase(long offset, rt_uint32_t size)
{
    return ram_flash_erase(offset, size);
}

#endif
#endif /* BSP_USING_RAM_FLASH */
