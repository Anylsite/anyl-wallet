#ifndef _WEB3_RPC_H_
#define _WEB3_RPC_H_
/* system includes */
#include <stddef.h>
/* local includes */
#include "helpers/uint256.h"
#include "eth/address.h"
#include "eth/transaction.h"


#ifdef  __cplusplus
extern "C" {
#endif

int web3_eth_blockNumber(uint64_t *out);
int web3_eth_getBalance(const address_t *address, uint256_t *out);
int web3_eth_getTransactionCount(const address_t *address, uint256_t *out);
int web3_eth_sendRawTransaction(const uint8_t *data, size_t data_len, uint256_t *out);
int web3_eth_estimateGas(const address_t *from, const transaction_t *tx, uint256_t *out);
int web3_eth_call(const address_t *from, const transaction_t *tx, uint256_t *out, uint8_t tx_flags);
int web3_eth_getTransactionReceipt(const tx_hash_t *tx_hash, tx_receipt_t *out);
#ifdef __cplusplus
}
#endif

#endif /* _WEB3_RPC_H_ */

