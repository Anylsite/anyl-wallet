/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
* Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
*/

#ifndef BYTE_CONVERTER_H__
#define BYTE_CONVERTER_H__

#include <stdint.h>

uint8_t number_to_bytes(uint8_t *raw_bytes, uint32_t number);
uint8_t char_to_bytes(uint8_t *raw_bytes, const char *text);

#endif  /* BYTE_CONVERTER_H__ */