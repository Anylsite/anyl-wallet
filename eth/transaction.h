/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
*/

#ifndef TRANSACTION_H__
#define TRANSACTION_H__

#include <stdint.h>
#include <stddef.h>
#include "helpers/uint256.h"
#include "eth/sign.h"
#include "eth/address.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define SIGNATURE_LENGTH 64

typedef struct {
    uint32_t nonce;
    uint32_t gas_price;
    uint32_t gas_limit;
    address_t to;
    uint256_t value;
    uint8_t *data;
    size_t data_len;
} transaction_t;

typedef struct {
    uint8_t h[32];
} tx_hash_t;

typedef struct {
    uint64_t blockNumber;
    uint8_t status;
} tx_receipt_t;

int tx_set_to(transaction_t *tx, const char *to_hex);
void tx_set_value_u64(transaction_t *tx, uint64_t val);

int tx_encode(const transaction_t *tx, const signature_t *sig, uint8_t *buf, size_t buf_size);
// create pre-EIP155 tx
int tx_encode_sign(const transaction_t *tx, const uint8_t *privkey, uint8_t *buf, size_t buf_size);


#ifdef __cplusplus
}
#endif
#endif  /* TRANSACTION_H__ */

