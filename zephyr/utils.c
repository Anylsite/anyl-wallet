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
#include <stdlib.h>

/* local includes */
#include "utils.h"
#include "helpers/math.h"

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

int shell_get_uint64(const char *argval, uint64_t *out)
{
    char *endptr = NULL;
    errno = 0;
    *out = strtoul(argval, &endptr, 10);
    if((errno != 0) || ((endptr != NULL) && (*endptr != 0))) {
        return -1;
    }
    return 0;
}

void shell_print_decimal_u256(const struct shell *shell, const uint256_t *val, uint8_t decimals)
{
    uint256_t decimals_u256, div_u256, mod_u256;
    clear256(&decimals_u256);
    clear256(&div_u256);
    clear256(&mod_u256);
    LOWER(LOWER(decimals_u256)) = ipow(10, decimals);
    divmod256(val, &decimals_u256, &div_u256, &mod_u256);

    printk_uint256_int(shell, &div_u256, 0, FMT_EMPTY);
    printk(".");
    printk_uint256_int(shell, &mod_u256, decimals, FMT_FIXED | FMT_NO_TRAIL_ZERO);
    printk("\n");
}
