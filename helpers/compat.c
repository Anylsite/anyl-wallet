/**
* @brief random32() implementation for non-zephyr builds
* @file compat.c
* @author J.H. 
* @date 2018-11-07
*/
#include <stdint.h>

#ifdef __ZEPHYR__
#include "random/rand32.h"
uint32_t random32(void)
{
    return sys_rand32_get();
}

#else
/* system includes */
#include <stdlib.h>

/* local includes */

uint32_t random32()
{
    return rand();
}

#endif
