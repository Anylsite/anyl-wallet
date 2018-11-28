/**
* @brief 
* @file sign.c
* @author J.H. 
* @date 2018-10-11
*/

/* system includes */
#include <trezor-crypto/ecdsa.h>
#include <trezor-crypto/sha3.h>
#include <trezor-crypto/secp256k1.h>
#include <string.h>
#include <stdio.h>

/* local includes */
#include "sign.h"

static const ecdsa_curve *curve = &secp256k1;

static const char *_msg_prefix = "\x19""Ethereum Signed Message:\n";

int eth_digest_message(const uint8_t *data, size_t data_len, uint8_t *digest)
{
    SHA3_CTX hasher_ctx;
    keccak_256_Init(&hasher_ctx);
    // first initialize hasher with the prefix
    keccak_Update(&hasher_ctx, (uint8_t*)_msg_prefix, strlen(_msg_prefix));
    // pass a data length (as an ASCII string) to the hasher
    char _data_len[8] = {0};
    snprintf(_data_len, sizeof(_data_len), "%zu", data_len);
    keccak_Update(&hasher_ctx, (uint8_t*)_data_len, strlen(_data_len));
    // lastly pass in the data
    keccak_Update(&hasher_ctx, data, data_len);
    // obtain the digest
    keccak_Final(&hasher_ctx, digest);

    return 0;
}

int eth_sign(const uint8_t *privkey, const uint8_t *data, size_t data_len, uint8_t *signature)
{
    uint8_t pby;
    uint8_t digest[32];

    // get the digest
    if(eth_digest_message(data, data_len, digest) < 0) { return -1; }
    // sign the digest
    ecdsa_sign_digest(curve, privkey, digest, signature, &pby, NULL);
    // add v
    signature[64] = pby + 27;
    return 0;
}

// just sign keccak(data)
int eth_sign_data_rawsig(const uint8_t *privkey, const uint8_t *data, size_t data_len, uint8_t *signature)
{
    uint8_t digest[32];
    uint8_t pby;
    keccak_256(data, data_len, digest);
    ecdsa_sign_digest(curve, privkey, digest, signature, &pby, NULL);
    signature[64] = pby + 27;
    return 0;
}

int eth_sign_data(const uint8_t *privkey, const uint8_t *data, size_t data_len, signature_t *out)
{
    uint8_t signature[65] = {0};
    if(eth_sign_data_rawsig(privkey, data, data_len, signature) != 0) { return -1; }
    out->v = signature[64];
    memcpy(&out->r, signature, sizeof(uint256_t));
    memcpy(&out->s, signature+sizeof(uint256_t), sizeof(uint256_t));
    return 0;
}

int privkey_to_pubkey(const uint8_t *privkey, uint8_t *pubkey)
{
    ecdsa_get_public_key65(curve, privkey, pubkey);
    return 0;
}
