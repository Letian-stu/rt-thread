/**    
* @author		Letian
* @date		    2025/02/08 20:02
* @version	    V1.0.0
*/
#ifndef _ATTR_DEFCONFIG_H_
#define _ATTR_DEFCONFIG_H_

typedef enum{
    ATTR_SUCCESS = 0,
    ATTR_FAIL,
    ATTR_TYPE_ERR,
}ATTR_ERR_E;

//各数据枚举值
typedef enum{
    ATTR_UINT8 = 0,
    ATTR_INT8 = 1,    
    ATTR_UINT16 = 2,
    ATTR_INT16 = 3,    
    ATTR_UINT32 = 4,
    ATTR_INT32 = 5,    
    ATTR_FLOAT = 6,
    ATTR_MEM = 7,
    ATTR_MAX,
}ATTR_TYPE_E;

#define ATTR_GET_TYPE(PID) ((PID >> 16) & 0x07)
#define ATTR_GET_ADDR(PID) (PID & 0x0000FFFF)

//单个数据使用字节数
#define ATTR_BYTE_LEN           1
#define ATTR_WORD_LEN           2
#define ATTR_DWORD_LEN          4
#define ATTR_FLOAT_LEN          ATTR_DWORD_LEN
#define ATTR_MEM_LEN            64

//各数据使用数量
#define ATTR_UINT8_DATA_NUM     3
#define ATTR_INT8_DATA_NUM      3
#define ATTR_UINT16_DATA_NUM    3
#define ATTR_INT16_DATA_NUM     3
#define ATTR_UINT32_DATA_NUM    6
#define ATTR_INT32_DATA_NUM     3
#define ATTR_FLOAT_DATA_NUM     3
#define ATTR_MEM_DATA_NUM       4

//各数据占用大小
#define ATTR_UINT8_DATA_SIZE    ATTR_UINT8_DATA_NUM  * ATTR_BYTE_LEN
#define ATTR_INT8_DATA_SIZE     ATTR_INT8_DATA_NUM   * ATTR_BYTE_LEN
#define ATTR_UINT16_DATA_SIZE   ATTR_UINT16_DATA_NUM * ATTR_WORD_LEN
#define ATTR_INT16_DATA_SIZE    ATTR_INT16_DATA_NUM  * ATTR_WORD_LEN
#define ATTR_UINT32_DATA_SIZE   ATTR_UINT32_DATA_NUM * ATTR_DWORD_LEN
#define ATTR_INT32_DATA_SIZE    ATTR_INT32_DATA_NUM  * ATTR_DWORD_LEN
#define ATTR_FLOAT_DATA_SIZE    ATTR_FLOAT_DATA_NUM  * ATTR_FLOAT_LEN
#define ATTR_MEM_DATA_SIZE      ATTR_MEM_DATA_NUM    * ATTR_MEM_LEN

//属于使用空间
#define ATTR_SUM_DATA_SIZE      322  
//属性使用个数
#define ATTR_SUM_DATA_NUM       28   

//各数据起始地址
#define ATTR_UINT8_START_ADDR   0x0000
#define ATTR_INT8_START_ADDR    0x0003
#define ATTR_UINT16_START_ADDR  0x0006
#define ATTR_INT16_START_ADDR   0x000C
#define ATTR_UINT32_START_ADDR  0x0012
#define ATTR_INT32_START_ADDR   0x002A
#define ATTR_FLOAT_START_ADDR   0x0036
#define ATTR_MEM_START_ADDR     0x0042

//属性表最大空间
#define ATTR_MAX_SIZE           8192

#endif