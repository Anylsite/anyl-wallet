/**
* @brief 
* @file compat.c
* @author J.H. 
* @date 2018-10-12
*/

/* system includes */
#include <zephyr.h>

/* local includes */
#include <misc/byteorder.h>

void srand(unsigned int seed)
{
    ARG_UNUSED(seed);
}

int rand(void)
{
    return sys_rand32_get();
}


uint32_t time(void)
{
    return 0;
}

uint64_t htobe64(uint64_t little_endian_64bits)
{
    return sys_cpu_to_be64(little_endian_64bits);
}
