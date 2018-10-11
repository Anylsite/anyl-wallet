/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
*/

#ifndef TRANSACTION_H__
#define TRANSACTION_H__

#include <stdint.h>
#include <stddef.h>
#include "helpers/uint256.h"

#ifdef  __cplusplus
extern "C" {
#endif
#define SIGNATURE_LENGTH 64

typedef uint8_t address_t[20];

typedef struct {
    uint32_t nonce;
    uint32_t gas_price;
    uint32_t gas_limit;
    address_t to;
    uint256_t value;
    uint8_t *data;
} transaction_t;

typedef struct {
    uint256_t r;
    uint256_t s;
    uint8_t v;
} signature_t;

void encode_transaction(const transaction_t *transaction, uint8_t *result, uint32_t *result_size);

int tx_set_to(transaction_t *tx, const char *to_hex);
void tx_set_value_u64(transaction_t *tx, uint64_t val);

int tx_encode(const transaction_t *tx, uint8_t *buf, size_t buf_size);


#ifdef __cplusplus
}
#endif
#endif  /* TRANSACTION_H__ */

