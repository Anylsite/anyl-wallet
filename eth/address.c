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
#include <assert.h>

/* local includes */
#include "address.h"
#include "sign.h"

#define ERROR (-1)
#define SUCCESS 0

static const ecdsa_curve *curve = &secp256k1;
int publickey_to_address(const uint8_t *pubkey, address_t *addr_out)
{
    uint8_t hash[32];
    assert(pubkey != NULL);
    assert(addr_out != NULL);

    keccak_256(pubkey+1, 64, hash);
    memcpy(addr_out, hash+12, 20);
    return SUCCESS;
}

int privkey_to_ethereum_address(const uint8_t *privkey, address_t *addr_out)
{
    uint8_t pubkey[65];
    ecdsa_get_public_key65(curve, privkey, pubkey);
    return publickey_to_address(pubkey, addr_out);
}

int address_from_signature(const uint8_t *data, size_t data_len, const signature_t *signature, address_t *addr_out)
{
    uint8_t pubkey[65];
    uint8_t digest[32];
    uint8_t sign[65] = {0u};

    assert(data != NULL);
    assert (signature != NULL);
    assert(addr_out != NULL);

    //Get the message digest
    if ( eth_digest_message(data, data_len, digest) != SUCCESS ) { return ERROR; }
    // COnvert the signature into flat buffer
    memcpy(sign, &signature->r, sizeof(uint256_t));
    memcpy(sign+sizeof(uint256_t), &signature->s, sizeof(uint256_t));

    if ( ecdsa_recover_pub_from_sig (curve, pubkey, sign, digest, signature->v-27u) ) { return ERROR; }
    if ( publickey_to_address(pubkey, addr_out) ) { return ERROR; }

    return SUCCESS;
}
