/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
* Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
*/

#ifndef RLP_ENCODER_H__
#define RLP_ENCODER_H__

#include <stdint.h>
#include <stddef.h>
#include <helpers/uint256.h>

#ifdef __cplusplus
extern "C" {
#endif

// the max size of header data in the RLP format is 8 bytes = uint64_max
// since we use uint32_t as an input len, we don't need to allocate more than 4+1 bytes for the header
#define HEADER_LEN  (4+1)  // max size of the header
#define HEADER_DATA_MAXLEN 1<<(((HEADER_LEN-1)*8)-1) // max size of the data contained in an array header
    //
// return header size for the data of @input_len
uint8_t rlp_header_len(size_t input_len);

uint32_t rlp_encode_integer(uint8_t* output, uint64_t val);
uint32_t rlp_encode_uint256(uint8_t* output, uint256_t val);
uint32_t rlp_encode_item(uint8_t* output, const uint8_t* input, uint32_t input_len);
uint32_t rlp_encode_array_header(uint8_t *header_output, uint32_t total_len);

#ifdef __cplusplus
}
#endif

#endif  /* RLP_ENCODER_H__ */

