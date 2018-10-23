#ifndef _WEB3_H_
#define _WEB3_H_
/* system includes */
#include <stdint.h>
/* local includes */
#include "eth/address.h"


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t *buf;
    size_t buf_size;
    size_t buf_used;
    uint32_t req_id;
} web3_ctx_t;

void web3_init(web3_ctx_t *web3, uint8_t *buf, size_t buf_size);

int eth_getTransactionCount(web3_ctx_t *web3, const address_t *addr);
int eth_blockNumber(web3_ctx_t *web3);
int eth_sendRawTransaction(web3_ctx_t *web3, uint8_t *data, size_t data_len);
int eth_getBalance(web3_ctx_t *web3, const address_t *addr);

#ifdef __cplusplus
}
#endif

#endif /* _WEB3_H_ */

