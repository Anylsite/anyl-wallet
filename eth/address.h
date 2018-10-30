#ifndef _ADDRESS_H_
#define _ADDRESS_H_
/* system includes */
/* local includes */
#include "sign.h"


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t a[20];
} address_t;
int privkey_to_ethereum_address(const uint8_t *privkey, address_t *addr_out);
int address_from_signature(const uint8_t *data, size_t data_len, const signature_t *signature, address_t *addr_out);

#ifdef __cplusplus
}
#endif

#endif /* _ADDRESS_H_ */

