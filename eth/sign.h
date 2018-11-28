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

// signature is 32 bytes
int eth_sign(const uint8_t *privkey, const uint8_t *data, size_t data_len, uint8_t *signature);
int eth_digest_message(const uint8_t *data, size_t data_len, uint8_t *digest);
int eth_sign_data_rawsig(const uint8_t *privkey, const uint8_t *data, size_t data_len, uint8_t *signature);
int eth_sign_data(const uint8_t *privkey, const uint8_t *data, size_t data_len, signature_t *out);

int privkey_to_pubkey(const uint8_t *privkey, uint8_t *pubkey);

#ifdef __cplusplus
}
#endif

#endif /* _SIGN_H_ */

