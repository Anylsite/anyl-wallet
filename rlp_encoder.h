/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
* Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
*/

#ifndef RLP_ENCODER_H__
#define RLP_ENCODER_H__

#include <stdint.h>

uint32_t encode_item(uint8_t* output, const uint8_t* input, uint32_t input_len);
uint32_t encode_whole_header(uint8_t *header_output, uint32_t total_len);

#endif  /* RLP_ENCODER_H__ */