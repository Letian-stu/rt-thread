/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtconfig.h>
#include <board.h>

#define ONCHIP_FLASH_DEV_NAME          "onchip_flash"
#define NOR_FLASH_DEV_NAME             "w25q64"

#define FAL_ONCHIP_START_ADDR_BOOT          0
#define FAL_ONCHIP_MAX_LEN_BOOT             (256*1024)
#define FAL_ONCHIP_START_ADDR_APP           FAL_ONCHIP_START_ADDR_BOOT + FAL_ONCHIP_MAX_LEN_BOOT
#define FAL_ONCHIP_MAX_LEN_APP              (512*1024)
#define FAL_ONCHIP_START_ADDR_KVDB          FAL_ONCHIP_START_ADDR_APP + FAL_ONCHIP_MAX_LEN_APP
#define FAL_ONCHIP_MAX_LEN_KVDB             (128*1024)
#define FAL_ONCHIP_START_ADDR_TSDB          FAL_ONCHIP_START_ADDR_KVDB + FAL_ONCHIP_MAX_LEN_KVDB
#define FAL_ONCHIP_MAX_LEN_TSDB             (128*1024)

#define FAL_NORFLASH_START_ADDR_KVDB        0
#define FAL_NORFLASH_MAX_LEN_KVDB           (1024*1024)
#define FAL_NORFLASH_START_ADDR_TSDB        FAL_NORFLASH_START_ADDR_KVDB + FAL_NORFLASH_MAX_LEN_KVDB
#define FAL_NORFLASH_MAX_LEN_TSDB           (1024*1024)
#define FAL_NORFLASH_START_ADDR_OTA         FAL_NORFLASH_START_ADDR_TSDB + FAL_NORFLASH_MAX_LEN_TSDB
#define FAL_NORFLASH_MAX_LEN_OTA            (512*1024)
#define FAL_NORFLASH_START_ADDR_FACTORY     FAL_NORFLASH_START_ADDR_OTA + FAL_NORFLASH_MAX_LEN_OTA
#define FAL_NORFLASH_MAX_LEN_FACTORY        (512*1024)
#define FAL_NORFLASH_START_ADDR_FS          FAL_NORFLASH_START_ADDR_FACTORY + FAL_NORFLASH_MAX_LEN_FACTORY
#define FAL_NORFLASH_MAX_LEN_FS             (5*1024*1024)

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev at32_onchip_flash;
extern struct fal_flash_dev nor_flash0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &at32_onchip_flash,                                              \
    &nor_flash0,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                               \
{      																				 \
    {FAL_PART_MAGIC_WROD,        "bl",      ONCHIP_FLASH_DEV_NAME,         FAL_ONCHIP_START_ADDR_BOOT,         FAL_ONCHIP_MAX_LEN_BOOT, 0}, \
    {FAL_PART_MAGIC_WROD,       "app",      ONCHIP_FLASH_DEV_NAME,          FAL_ONCHIP_START_ADDR_APP,          FAL_ONCHIP_MAX_LEN_APP, 0}, \
    {FAL_PART_MAGIC_WROD, "fdb_kvdb1",      ONCHIP_FLASH_DEV_NAME,         FAL_ONCHIP_START_ADDR_KVDB,         FAL_ONCHIP_MAX_LEN_KVDB, 0}, \
    {FAL_PART_MAGIC_WROD, "fdb_tsdb1",      ONCHIP_FLASH_DEV_NAME,         FAL_ONCHIP_START_ADDR_TSDB,         FAL_ONCHIP_MAX_LEN_TSDB, 0}, \
    {FAL_PART_MAGIC_WORD, "fdb_kvdb2",         NOR_FLASH_DEV_NAME,       FAL_NORFLASH_START_ADDR_KVDB,       FAL_NORFLASH_MAX_LEN_KVDB, 0}, \
    {FAL_PART_MAGIC_WORD, "fdb_kvdb2",         NOR_FLASH_DEV_NAME,       FAL_NORFLASH_START_ADDR_TSDB,       FAL_NORFLASH_MAX_LEN_TSDB, 0}, \
    {FAL_PART_MAGIC_WORD,       "ota",         NOR_FLASH_DEV_NAME,        FAL_NORFLASH_START_ADDR_OTA,        FAL_NORFLASH_MAX_LEN_OTA, 0}, \
    {FAL_PART_MAGIC_WORD,   "factory",         NOR_FLASH_DEV_NAME,    FAL_NORFLASH_START_ADDR_FACTORY,    FAL_NORFLASH_MAX_LEN_FACTORY, 0}, \
    {FAL_PART_MAGIC_WORD,        "fs",         NOR_FLASH_DEV_NAME,         FAL_NORFLASH_START_ADDR_FS,         FAL_NORFLASH_MAX_LEN_FS, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
