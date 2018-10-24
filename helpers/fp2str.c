/**
* @brief 
* @file fp2str.c
* @author J.H. 
* @date 2018-10-24
*/

/* system includes */
#include <string.h>
#include <stdio.h>

/* local includes */
#include "fp2str.h"

int fp2str(uint32_t val, uint8_t digits, char *buf, size_t buf_len)
{
    char *buf_p = buf;
    int ret = snprintf(buf_p, buf_len, "%d", val >> digits);
    if(ret < 0) {
        return ret;
    }
    if(digits == 0) { return 0; }
    // extract last n bits
    val &= ((1L<<digits)-1);
    if(val == 0) { return 0; }
    // if there's frac part, at least three additional bytes are required
    if((size_t)(ret + 2 + 1) > buf_len) { return -1; }
    buf_p+=ret;
    *buf_p++ = '.';


    while(val > 0) {
        if(buf_p == (buf+buf_len-1)) { return -1; }
        val *= 10;
        *buf_p++ = '0' + (val >> digits);
        val &= ((1 << digits) - 1);
    }
    *buf_p = '\0';
    return 0;
}
