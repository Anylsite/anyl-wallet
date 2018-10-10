/**
* @brief 
* @file address.c
* @author J.H. 
* @date 2018-10-10
*/

/* system includes */
#include <trezor-crypto/ecdsa.h>
#include <trezor-crypto/hasher.h>
#include <trezor-crypto/secp256k1.h>
#include <string.h>

/* local includes */
#include "address.h"

static const ecdsa_curve *curve = &secp256k1;

int privkey_to_ethereum_address(const uint8_t *privkey, uint8_t *addr)
{
    uint8_t pubkey[65];
    uint8_t hash[32];
    ecdsa_get_public_key65(curve, privkey, pubkey);
    keccak_256(pubkey+1, 64, hash);
    memcpy(addr, hash+12, 20);
    return 0;
}

