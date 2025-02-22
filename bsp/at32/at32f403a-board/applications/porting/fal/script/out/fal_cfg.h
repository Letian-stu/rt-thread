#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtconfig.h>
#include <board.h>

#define ONCHIP_FLASH_DEV_NAME                       "OnChip"
#define NOR_FLASH_DEV_NAME                          "w25q64"

#define FAL_ONCHIP_START_ADDR_BOOT                  0
#define FAL_ONCHIP_MAX_LEN_BOOT                     (128*1024)
#define FAL_ONCHIP_START_ADDR_APP_A                 131072
#define FAL_ONCHIP_MAX_LEN_APP_A                    (256*1024)
#define FAL_ONCHIP_START_ADDR_APP_B                 393216
#define FAL_ONCHIP_MAX_LEN_APP_B                    (256*1024)
#define FAL_ONCHIP_START_ADDR_SYSCFG                655360
#define FAL_ONCHIP_MAX_LEN_SYSCFG                   (256*1024)
#define FAL_ONCHIP_START_ADDR_RESERVED              917504
#define FAL_ONCHIP_MAX_LEN_RESERVED                 (128*1024)

#define FAL_NORFLASH_START_ADDR_FACTORY             0
#define FAL_NORFLASH_MAX_LEN_FACTORY                (256*1024)
#define FAL_NORFLASH_START_ADDR_OTA                 262144
#define FAL_NORFLASH_MAX_LEN_OTA                    (256*1024)
#define FAL_NORFLASH_START_ADDR_KVDB                524288
#define FAL_NORFLASH_MAX_LEN_KVDB                   (512*1024)
#define FAL_NORFLASH_START_ADDR_TSDB                1048576
#define FAL_NORFLASH_MAX_LEN_TSDB                   (512*1024)
#define FAL_NORFLASH_START_ADDR_FS                  1572864
#define FAL_NORFLASH_MAX_LEN_FS                     (2048*1024)
#define FAL_NORFLASH_START_ADDR_USERDATA            3670016
#define FAL_NORFLASH_MAX_LEN_USERDATA               (512*1024)
#define FAL_NORFLASH_START_ADDR_LOG                 4194304
#define FAL_NORFLASH_MAX_LEN_LOG                    (1024*1024)
#define FAL_NORFLASH_START_ADDR_RESERVED            5242880
#define FAL_NORFLASH_MAX_LEN_RESERVED               (1024*1024)

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
#define FAL_PART_TABLE                                               \
{                                                                    \
    {FAL_PART_MAGIC_WORD,   "BOOT"      , "OnChip"    ,        0,   131072, 0}, \
    {FAL_PART_MAGIC_WORD,   "APP_A"     , "OnChip"    ,   131072,   262144, 0}, \
    {FAL_PART_MAGIC_WORD,   "APP_B"     , "OnChip"    ,   393216,   262144, 0}, \
    {FAL_PART_MAGIC_WORD,   "SYSCFG"    , "OnChip"    ,   655360,   262144, 0}, \
    {FAL_PART_MAGIC_WORD,   "RESERVED"  , "OnChip"    ,   917504,   131072, 0}, \
    {FAL_PART_MAGIC_WORD,   "FACTORY"   , "w25q64"    ,        0,   262144, 0}, \
    {FAL_PART_MAGIC_WORD,   "OTA"       , "w25q64"    ,   262144,   262144, 0}, \
    {FAL_PART_MAGIC_WORD,   "KVDB"      , "w25q64"    ,   524288,   524288, 0}, \
    {FAL_PART_MAGIC_WORD,   "TSDB"      , "w25q64"    ,  1048576,   524288, 0}, \
    {FAL_PART_MAGIC_WORD,   "FS"        , "w25q64"    ,  1572864,  2097152, 0}, \
    {FAL_PART_MAGIC_WORD,   "USERDATA"  , "w25q64"    ,  3670016,   524288, 0}, \
    {FAL_PART_MAGIC_WORD,   "LOG"       , "w25q64"    ,  4194304,  1048576, 0}, \
    {FAL_PART_MAGIC_WORD,   "RESERVED"  , "w25q64"    ,  5242880,  1048576, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
 