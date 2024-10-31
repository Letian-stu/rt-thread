/*
 * @Date: 2024-07-26 22:35:02
 * @LastEditors: StuTian 1656733975@qq.com
 * @LastEditTime: 2024-07-27 00:10:23
 * @FilePath: \attr\main.c
 */
/*
#include <stdio.h>
#include <stdlib.h>

#include "inc/attr.h"
#include "inc/attr_pid_table.h"

void hexdump(const void *pdata, int len)
{
    int i, j, k, l;
    const char *data = (const char *)pdata;
    char buf[256], str[64], t[] = "0123456789ABCDEF";
    for (i = j = k = 0; i < len; i++)
    {
        if (0 == i % 16)
            j += sprintf(buf + j, "%04xh: ", i);
        buf[j++] = t[0x0f & (data[i] >> 4)];
        buf[j++] = t[0x0f & data[i]];
        buf[j++] = ' ';
        str[k++] = isprint(data[i]) ? data[i] : '.';
        if (0 == (i + 1) % 16)
        {
            str[k] = 0;
            j += sprintf(buf + j, "; %s\n", str);
            printf("%s", buf);
            j = k = buf[0] = str[0] = 0;
        }
    }
    str[k] = 0;
    if (k)
    {
        for (l = 0; l < 3 * (16 - k); l++)
            buf[j++] = ' ';
        j += sprintf(buf + j, "; %s\n", str);
    }
    if (buf[0])
        printf("%s", buf);
}

int main(int argc, char *argv[]) 
{
	attr_create(&attr);
	
	int8_t setval8 = 12;
	attr_set_i8(ATTR_Res8_1, setval8++);
	int8_t getval8 = 0;
	attr_get_i8(ATTR_Res8_1, &getval8);
	printf("tt get %x \n", getval8);
	
	int16_t setval16 = 0x1234;
	attr_set_i16(ATTR_Res16_1, setval16++);
	int16_t getval16 = 0;
	attr_get_i16(ATTR_Res16_1, &getval16);
	printf("tt get %x \n", getval16);


	int32_t setval32 = 0x12345678;
	attr_set_i32(ATTR_Res32_1, setval32++);
	int32_t getval32 = 0;
	attr_get_i32(ATTR_Res32_1, &getval32);
	printf("tt get %x \n", getval32);


	float setvalf = 11.2;
	attr_set_float(ATTR_ResFloat_1, setvalf);
	float getvalf = 0;
	attr_get_float(ATTR_ResFloat_1, &getvalf);
	printf("tt get %f \n", getvalf);

	char setval[] = "hello world";
	attr_set_mem(ATTR_ResMem_1, setval, sizeof(setval));
	char getval[64] = {0};
	attr_get_mem(ATTR_ResMem_1, getval, sizeof(setval));
	printf("tt get %s \n", getval );


	hexdump((const void *)attr_mem_data(), ATTR_SUM_DATA_SIZE);
	printf("%d", ATTR_SUM_DATA_SIZE);
	while(1);
	return 0;
}
*/