/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
* Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
*/

#ifndef CRYPTO_HELPER_H__
#define CRYPTO_HELPER_H__

#include <stdint.h>

// "0x" + (40 bytes address) + "\0"
#define CRYPTO_HELPER_CHECKSUM_ADDRESS_LENGTH (2 + 40 + 1)
#define KECCAK256_HASH_LENGTH 32

void generate_checksum_address(const uint8_t *public_address, char *checksum_address);
void keccak256(const uint8_t *data, uint8_t *hash, uint16_t length);

#endif  /* CRYPTO_HELPER_H__ */

