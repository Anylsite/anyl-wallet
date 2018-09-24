/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
* Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
*/

#include "helpers/crypto_helper.h"
#include "crypto/keccak256.h"

#include <stdint.h>
#include <string.h>

static char get_hex_nibble(uint8_t value) {
    value &= 0x0f;

    if (value <= 9) {
      return '0' + value;
     }

    return 'a' + (value - 10);
}

void generate_checksum_address(const uint8_t *public_address, char *checksum_address) {
    uint8_t end = CRYPTO_HELPER_CHECKSUM_ADDRESS_LENGTH - 1;

    checksum_address[end--] = 0;

    for (int8_t i = 20 - 1; i >= 0; i--) {
        checksum_address[end--] = get_hex_nibble(public_address[i]);
        checksum_address[end--] = get_hex_nibble(public_address[i] >> 4);
    }

    checksum_address[end--] = 'x';
    checksum_address[end--] = '0';

    uint8_t hashed[KECCAK256_HASH_LENGTH];
    SHA3_CTX keccakContext;

    keccak_init(&keccakContext);
    keccak_update(&keccakContext, (const unsigned char*)&checksum_address[2], 40);
    keccak_final(&keccakContext, (unsigned char*)hashed);

    memset((char*)&keccakContext, 0, sizeof(SHA3_CTX));

    for (uint8_t i = 0; i < 40; i += 2) {
        if (checksum_address[2 + i] >= 'a' && (hashed[i >> 1] >> 4) >= 8) {
            checksum_address[2 + i] -= 0x20;
        }
        if (checksum_address[2 + i + 1] >= 'a' && (hashed[i >> 1] & 0x0f) >= 8) {
            checksum_address[2 + i + 1] -= 0x20;
        }
    }
}

void keccak256(const uint8_t *data, uint8_t *hash, uint16_t length) {
    SHA3_CTX context;

    keccak_init(&context);
    keccak_update(&context, (const unsigned char*)data, (size_t)length);
    keccak_final(&context, (unsigned char*)hash);

    // Clear out the contents of what we hashed (in case it was secret)
    memset((char*)&context, 0, sizeof(SHA3_CTX));
}

