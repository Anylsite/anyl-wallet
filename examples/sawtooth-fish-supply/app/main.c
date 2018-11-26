/**
* @brief 
* @file main.c
* @author J.H. 
* @date 2018-11-26
*/

/* system includes */
#include <stdint.h>

/* local includes */
#include "payload/fish_encoders.h"

#define PAYLOAD_BUF_SIZE    2048
void main(void)
{
    uint8_t buf[PAYLOAD_BUF_SIZE];
    size_t payload_written = encode_create_agent("POKUS", buf, sizeof(buf));
}
