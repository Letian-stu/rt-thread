/**    
* @author		Letian
* @date		    2024/07/27 17:17
* @version	    V1.0.0
*/
#ifndef _ATTR_PID_TABLE_H_
#define _ATTR_PID_TABLE_H_

typedef enum 
{ 
    ATTR_ResU8_1 = 0x00000000, //uint8 /-/ / 0 预留 
    ATTR_ResU8_2 = 0x00000001, //uint8 /-/ / 1 预留 
    ATTR_ResU8_3 = 0x00000002, //uint8 /-/ / 2 预留 
    ATTR_Res8_1 = 0x00010000, //int8 /-/ / 0 预留 
    ATTR_Res8_2 = 0x00010001, //int8 /-/ / 1 预留 
    ATTR_Res8_3 = 0x00010002, //int8 /-/ / 2 预留 
    ATTR_ResU16_1 = 0x00020000, //uint16 /-/ / 0 预留 
    ATTR_ResU16_2 = 0x00020002, //uint16 /-/ / 2 预留 
    ATTR_ResU16_3 = 0x00020004, //uint16 /-/ / 4 预留 
    ATTR_Res16_1 = 0x00030000, //int16 /-/ / 0 预留 
    ATTR_Res16_2 = 0x00030002, //int16 /-/ / 2 预留 
    ATTR_Res16_3 = 0x00030004, //int16 /-/ / 4 预留 
    ATTR_ResU32_1 = 0x00040000, //uint32 /-/ / 0 预留 
    ATTR_ResU32_2 = 0x00040004, //uint32 /-/ / 4 预留 
    ATTR_ResU32_3 = 0x00040008, //uint32 /-/ / 8 预留 
    ATTR_Res32_1 = 0x00050000, //int32 /-/ / 0 预留 
    ATTR_Res32_2 = 0x00050004, //int32 /-/ / 4 预留 
    ATTR_Res32_3 = 0x00050008, //int32 /-/ / 8 预留 
    ATTR_ResFloat_1 = 0x00060000, //float /-/ / 0 预留 
    ATTR_ResFloat_2 = 0x00060004, //float /-/ / 4 预留 
    ATTR_ResFloat_3 = 0x00060008, //float /-/ / 8 预留 
    ATTR_ResMem_1 = 0x00070000, //mem /-/ / 0 预留 
    ATTR_ResMem_2 = 0x00070040, //mem /-/ / 64 预留 
    ATTR_ResMem_3 = 0x00070080, //mem /-/ / 128 预留 
    ATTR_SysTick = 0x0004000c, //uint32 /-/ / 12 预留 
    ATTR_SysTime = 0x00040010, //uint32 /-/ / 16 预留 
    ATTR_SysFreq = 0x00040014, //uint32 /-/ / 20 预留 
    ATTR_SysSoftMsg = 0x000700c0, //mem /-/ / 192 预留
}ATTR_LIST_E;

#endif