/**
* @brief 
* @file utils.c
* @author J.H. 
* @date 2018-10-12
*/

/* system includes */
#include <zephyr.h>
#include <assert.h>
#include <string.h>

/* local includes */
#include "utils.h"

void printk_hex(const struct shell *shell, const uint8_t *data, size_t data_len)
{
    for (size_t i = 0; i < data_len; i++) {
        shell_fprintf(shell, SHELL_NORMAL, "%02x", data[i]);
    }
}

void printk_uint256(const struct shell *shell, const uint256_t *v)
{
    assert(v != NULL);
    char buf[64+1] = {0};
    tostring256(v, 16, buf, sizeof(buf));
    shell_fprintf(shell, SHELL_NORMAL,  "0x%s\n", buf);
}

void printk_uint256_int(const struct shell *shell, const uint256_t *v, uint8_t decimals, uint8_t fmt)
{
    assert(v != NULL);
    char buf[64+1] = {0};
    tostring256(v, 10, buf, sizeof(buf));
    size_t buf_len = strlen(buf);
    if(decimals > (sizeof(buf)-1)) {
        shell_fprintf(shell, SHELL_NORMAL, "<%s() conversion error>", __FUNCTION__);
        return;
    }
    if((fmt & FMT_FIXED) && (buf_len < decimals)) {
        memmove(buf + decimals - buf_len, buf, buf_len);
        for(unsigned int i = 0; i < (decimals-buf_len); i++) {
            buf[i] = '0';
        }
    }
    if(fmt & FMT_NO_TRAIL_ZERO) {
        for(unsigned int i = strlen(buf)-1; i > 0; i--) {
            if(buf[i] != '0') { break; }
            buf[i] = '\0';
        }
    }
    shell_fprintf(shell, SHELL_NORMAL, "%s", buf);
}
