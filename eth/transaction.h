/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
*/

#ifndef TRANSACTION_H__
#define TRANSACTION_H__

#include <stdint.h>
#include "helpers/uint256.h"

#define SIGNATURE_LENGTH 64

typedef struct {
    uint32_t nonce;
    uint32_t gas_price;
    uint32_t gas_limit;
    char *to;
    char *value;
    char *data;
} Transaction;

typedef struct {
    uint8_t *r;
    uint8_t *s;
    uint8_t v;
} Signature;

void encode_transaction(Transaction transaction, Signature signature, uint8_t *result, uint32_t *result_size);


#endif  /* TRANSACTION_H__ */

