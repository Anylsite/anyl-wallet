/**
* @brief 
* @file utils.c
* @author J.H. 
* @date 2018-10-12
*/

/* system includes */
#include <zephyr.h>
#include <assert.h>

/* local includes */
#include "utils.h"

void printk_hex(const uint8_t *data, size_t data_len)
{
    for (size_t i = 0; i < data_len; i++) {
        printk("%02x", data[i]);
    }
}

void printk_uint256(const uint256_t *v)
{
    assert(v != NULL);
    char buf[64+1] = {0};
    tostring256(v, 16, buf, sizeof(buf));
    printk("0x%s\n", buf);
}
