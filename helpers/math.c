/**
* @brief 
* @file math.c
* @author J.H. 
* @date 2018-11-07
*/

/* system includes */

/* local includes */
#include "math.h"


uint64_t ipow(uint64_t base, uint8_t exp)
{
    uint64_t result = 1;
    for (;;)
    {
        if (exp & 1) { result *= base; }
        exp >>= 1;
        if (!exp) { break; }
        base *= base;
    }

    return result;
}
