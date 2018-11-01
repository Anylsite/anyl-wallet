#ifndef _UTILS_H_
#define _UTILS_H_
/* system includes */
/* local includes */
#include "helpers/uint256.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <shell/shell.h>
#pragma GCC diagnostic pop


#ifdef  __cplusplus
extern "C" {
#endif

#define FMT_EMPTY           0
#define FMT_FIXED           1 << 0   // use fixed integer length (0 is used as a prefix)
#define FMT_NO_TRAIL_ZERO   1 << 1   // remove trailing zeros

void printk_hex(const struct shell *shell, const uint8_t *data, size_t data_len);
#define printk_hex_nl(shell, data, len) printk_hex(shell, data, len); shell_fprintf(shell, SHELL_NORMAL, "\n");
void printk_uint256(const struct shell *shell, const uint256_t *v);
void printk_uint256_int(const struct shell *shell, const uint256_t *v, uint8_t decimals, uint8_t fmt);
#ifdef __cplusplus
}
#endif

#endif /* _UTILS_H_ */

