/**
* @brief 
* @file data.c
* @author J.H. 
* @date 2018-09-19
*/

/* system includes */
#include <assert.h>
#include <string.h>

/* local includes */
#include "data.h"
#include "helpers/macros.h"


// returns index in bytes
static size_t compute_item_index(size_t index)
{
    return (index * DATA_ITEM_LEN) + SELECTOR_LEN;
}

// TODO: add check for system endianess
static uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}


// check the data block consistency
int data_init(data_block_t *blk)
{
    assert(blk != NULL);
    assert(blk->data != NULL);
    assert(((blk->data_len - SELECTOR_LEN) % DATA_ITEM_LEN) == 0);
    memset(blk->data, 0, blk->data_len);
    return 0;
}

int data_set_selector(data_block_t *blk, uint32_t selector)
{
    assert(blk != NULL);
    assert(blk->data != NULL);
    assert(blk->data_len >= 4);
    uint32_t *u_p = (uint32_t*)blk->data;
    *u_p =  swap_uint32(selector);
    return 0;
}

void item_clear(data_block_t *blk, size_t idx)
{
    size_t item_idx = compute_item_index(idx);
    memset(blk->data + item_idx, 0, DATA_ITEM_LEN);
}


int encode_uint(data_block_t *blk, size_t idx, uint64_t val)
{
    // store @val at @blk[@idx]. data are BE encoded
    item_clear(blk, idx);
    size_t item_idx = compute_item_index(idx);
    uint64_t *u_p = (uint64_t*)(&blk->data[item_idx + (3 * sizeof(uint64_t))]);
    *u_p = ENDIAN_SWAP_U64(val);
    return 0;
}

int encode_uint128(data_block_t *blk, size_t idx, uint128_t *val)
{
    assert(val != NULL);
    assert(blk != NULL);
    item_clear(blk, idx);
    size_t item_idx = compute_item_index(idx);
    uint128_t *u_p= (uint128_t*)(&blk->data[item_idx+sizeof(uint128_t)]);
    *u_p = *val;
    return 0;
}

int encode_uint256(data_block_t *blk, size_t idx, uint256_t *val)
{
    assert(val != NULL);
    assert(blk != NULL);
    item_clear(blk, idx);
    size_t item_idx = compute_item_index(idx);
    uint256_t *u_p= (uint256_t*)(&blk->data[item_idx]);
    *u_p = *val;
    return 0;
}

int encode_address(data_block_t *blk, size_t idx, address_t *val)
{
    assert(val != NULL);
    assert(blk != NULL);
    item_clear(blk, idx);
    size_t item_idx = compute_item_index(idx);
    address_t *a_p= (address_t*)(&blk->data[item_idx] + (sizeof(uint256_t) - sizeof(address_t)));
    *a_p = *val;
    return 0;
}

int encode_data(data_block_t *blk, size_t idx, const uint8_t *data, size_t data_len)
{
    assert(data_len <= 32);
    size_t item_idx = compute_item_index(idx);
    assert(item_idx < blk->data_len);
    item_clear(blk, idx);
    memcpy(&blk->data[item_idx], data, data_len);
    return 0;
}

uint64_t decode_uint(data_block_t *blk, size_t idx)
{
    size_t item_idx = compute_item_index(idx);
    assert(item_idx < blk->data_len);
    uint128_t *u_p = (uint128_t*)(&blk->data[item_idx+sizeof(uint128_t)]);
    return ENDIAN_SWAP_U64(LOWER_P(u_p));
}

int decode_data(data_block_t *blk, size_t idx, uint8_t *data_out, size_t data_out_len)
{
    size_t item_idx = compute_item_index(idx);
    assert(item_idx < blk->data_len);
    uint8_t *u_p = &blk->data[item_idx];
    memcpy(data_out, u_p, MIN(data_out_len, DATA_ITEM_LEN));
    return 0;
}
