#ifndef _WEB3_H_
#define _WEB3_H_
/* system includes */
#include <stdint.h>
/* local includes */
#include "eth/address.h"
#include "eth/transaction.h"


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

// what to filter out when assembling tx JSON
#define TX_NO_FROM      1 << 0
#define TX_NO_GAS       1 << 1
#define TX_NO_GASPRICE  1 << 2
#define TX_NO_VALUE     1 << 3
#define TX_NO_DATA      1 << 4

int eth_getTransactionCount(web3_ctx_t *web3, const address_t *addr);
int eth_blockNumber(web3_ctx_t *web3);
int eth_sendRawTransaction(web3_ctx_t *web3, const uint8_t *data, size_t data_len);
int eth_getBalance(web3_ctx_t *web3, const address_t *addr);
int eth_call(web3_ctx_t *web3, const address_t *from, const transaction_t *tx, uint8_t tx_flags);
int eth_estimateGas(web3_ctx_t *web3, const address_t *from, const transaction_t *tx);
int eth_getTransactionReceipt(web3_ctx_t *web3, const tx_hash_t *tx_hash);

#ifdef __cplusplus
}
#endif

#endif /* _WEB3_H_ */

