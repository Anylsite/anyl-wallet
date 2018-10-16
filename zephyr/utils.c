/**
* @brief 
* @file utils.c
* @author J.H. 
* @date 2018-10-12
*/

/* system includes */
#include <zephyr.h>

/* local includes */
#include "utils.h"

void printk_hex(const uint8_t *data, size_t data_len)
{
    for (size_t i = 0; i < data_len; i++) {
        printk("%02x", data[i]);
    }
}
