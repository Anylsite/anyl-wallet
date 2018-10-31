#ifndef _DATA_H_
#define _DATA_H_
/* system includes */
#include <stdint.h>
#include <string.h>
/* local includes */
#include "helpers/uint256.h"
#include "eth/address.h"

#define DATA_ITEM_LEN   32
#define SELECTOR_LEN    4

#ifdef  __cplusplus
extern "C" {
#endif
typedef struct {
    size_t data_len;
    uint8_t *data; // length of data must be multiples of 32, plus 4 bytes for a selector
} data_block_t;

// check the data block consistency
int data_init(data_block_t *blk);

int data_set_selector(data_block_t *blk, uint32_t selector);

// zero item at @idx
void item_clear(data_block_t *blk, size_t idx);

// encode various ints. These are right-padded
int encode_uint(data_block_t *blk, size_t idx, uint64_t val);
int encode_int(data_block_t *blk, size_t idx, int64_t val);
int encode_uint256(data_block_t *blk, size_t idx, uint256_t *val);
int encode_uint128(data_block_t *blk, size_t idx, uint128_t *val);
int encode_address(data_block_t *blk, size_t idx, address_t *val);

// decode item at @idx as an uint64_t
uint64_t decode_uint(data_block_t *blk, size_t idx);
// encode generic data - left-padding is used
// maximal size of data is 32
int encode_data(data_block_t *blk, size_t idx, const uint8_t *data, size_t data_len);
int decode_data(data_block_t *blk, size_t idx, uint8_t *data_out, size_t data_len);

#ifdef __cplusplus
}
#endif

#endif /* _DATA_H_ */

