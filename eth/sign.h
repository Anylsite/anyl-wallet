#ifndef _SIGN_H_
#define _SIGN_H_
/* system includes */
#include <stdint.h>
#include <stddef.h>
/* local includes */
#include "helpers/uint256.h"

#ifdef  __cplusplus
extern "C" {
#endif


typedef struct {
    uint256_t r;
    uint256_t s;
    uint8_t v;
} signature_t;

// sign a data using provided @privkey
// @param privkey: privkey to use
// @param data: data to sign
// @param data_len: length of the data
// @param signature: signature is stored here. The parameter must have size of at least 65 bytes
int eth_sign(const uint8_t *privkey, const uint8_t *data, size_t data_len, uint8_t *signature);
// create a keccak256 digest of a data
// @param data: data to digest
// @param data_len: length of the data
// @param digest: digest is stored here. The parameter must have size of at least 32 bytes.
int eth_digest_message(const uint8_t *data, size_t data_len, uint8_t *digest);

// sign keccak256(data). the signature parameter must have size of at least 65 bytes.
int eth_sign_data_rawsig(const uint8_t *privkey, const uint8_t *data, size_t data_len, uint8_t *signature);
// sign keccak256(data) and store the signature as r, v, s values
int eth_sign_data(const uint8_t *privkey, const uint8_t *data, size_t data_len, signature_t *out);

int privkey_to_pubkey(const uint8_t *privkey, uint8_t *pubkey);

#ifdef __cplusplus
}
#endif

#endif /* _SIGN_H_ */

