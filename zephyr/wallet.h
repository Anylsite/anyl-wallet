#ifndef _WALLET_H_
#define _WALLET_H_
/* system includes */
#include <stdint.h>
/* local includes */
#include "eth/address.h"


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t k[32];
} privkey_t;

typedef uint32_t nonce_t;

typedef struct {
    privkey_t   privkey;
    address_t   address;
    nonce_t     nonce;
    uint32_t    gas_price;
    uint32_t    gas_limit;
} account_t;

void wallet_set_global_privkey(const privkey_t *pk);
account_t *wallet_get_account();

#ifdef __cplusplus
}
#endif

#endif /* _WALLET_H_ */

