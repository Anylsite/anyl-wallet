/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018.
* Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
*/

#include "rlp_encoder.h"
#include <assert.h>
#include <string.h>
#include <endian.h>


// return header size for the data of @input_len
uint8_t rlp_header_len(size_t input_len)
{
    if (input_len <= 55) {
        return 1;
    }
    uint8_t bytes = 0;
    while(input_len > 0) {
        input_len >>= 8;
        bytes++;
    }
    return bytes + 1;
}

static uint32_t val_size_bytes(uint32_t val)
{
    uint8_t shifts = 0;
    while(val > 0) {
        val >>= 8;
        shifts++;
    }
    return shifts;
}

static uint8_t encode_int_to_bytes(uint32_t val, uint8_t *output, const size_t output_max)
{
    const uint32_t size_bytes = val_size_bytes(val);
    uint8_t size_tmp = size_bytes;
    assert(output_max >= size_bytes);

    while (val > 0) {
        output[--size_tmp] = val & 0xff;
        val >>= 8;
    }
    return size_bytes;
}

uint32_t rlp_encode_item(uint8_t* output, const uint8_t* input, uint32_t input_len)
{
    assert(output != NULL);
    assert(input != NULL);

    if (input_len == 1 && input[0] == 0x00) {
        // empty string
        output[0] = 0x80;
        return 1;
    }
    if (input_len == 1 && input[0] < 128) {
        // single item
        memcpy(output, input, 1);
        return 1;
    }
    if (input_len <= 55) {
        // item of length 0 - 55
        uint8_t header = (uint8_t)0x80 + (uint8_t)input_len;

        memcpy(output, &header, 1);
        memcpy(output + 1, input, input_len);

        return input_len + 1;
    }
    // length > 55
    uint32_t input_len_bytes = encode_int_to_bytes(input_len, &output[1], 4);

    output[0] = (uint8_t)0xb7 + (uint8_t)input_len_bytes;
    memcpy(output + input_len_bytes + 1, input, input_len);

    return input_len + input_len_bytes + 1;
}


// return RLP encoded header for array of items of @total_len size
// should be renamed to encode_array_header
uint32_t rlp_encode_array_header(uint8_t *header_output, uint32_t total_len)
{
    assert(total_len <= (uint32_t)HEADER_DATA_MAXLEN);

    // simply encoded array
    if (total_len < 55) {
        header_output[0] = (uint8_t)0xc0 + (uint8_t)total_len;
        return 1;
    }
    const uint32_t input_len_bytes = encode_int_to_bytes(total_len, &header_output[1], 4);

    header_output[0] = (uint8_t)0xf7 + (uint8_t)input_len_bytes;

    return input_len_bytes + 1;
}

// minimize bytes used by the integer
static uint8_t _prepare_integer(uint64_t val, uint64_t *out)
{
    size_t i;
    // swap to BE
    *out = htobe64(val);
    // shift zero bytes until a val is found
    for(i = 0; i < sizeof(uint64_t); i++) {
        if(*out & 0xff) { break; }
        *out >>= 8;
    }
    return sizeof(uint64_t) - i;
}

uint32_t rlp_encode_integer(uint8_t* output, uint64_t val)
{
    uint64_t x;
    uint8_t size = _prepare_integer(val, &x);
    return rlp_encode_item(output, (uint8_t*)&x, size);
    assert(0); // should never happen
}

uint32_t rlp_encode_uint256(uint8_t* output, uint256_t val)
{
    if(zero128(&UPPER(val)) == true) {
        // upper 128 bits are empty
        if(UPPER(LOWER(val)) == 0) {
            // only last 64 bits are used
            return rlp_encode_integer(output, LOWER(LOWER(val)));
        } else {
            return rlp_encode_item(output, (uint8_t*)(&val + sizeof(128)), sizeof(val) - sizeof(128));
        }
    }
    return rlp_encode_item(output, (uint8_t*)&val, sizeof(uint256_t));
    assert(0);
}
