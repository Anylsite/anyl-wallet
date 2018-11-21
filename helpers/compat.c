/**
* @brief random32() implementation for non-zephyr builds
* @file compat.c
* @author J.H. 
* @date 2018-11-07
*/

#ifndef __ZEPHYR__

/* system includes */
#include <stdint.h>
#include <stdlib.h>

/* local includes */

uint32_t random32()
{
    return rand();
}

#endif
