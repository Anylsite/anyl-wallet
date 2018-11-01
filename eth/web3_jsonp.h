#ifndef _WEB3_JSONP_H_
#define _WEB3_JSONP_H_
/* system includes */
#include <stddef.h>
#include <stdint.h>
/* local includes */
#include "helpers/uint256.h"
#include "eth/transaction.h"


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    long code;
    const char *message;
} jsonrpc_error_t;

typedef struct {
    unsigned long id;
    jsonrpc_error_t error;
} jsonrpc_result_t;

int jsonrpc_decode_hexencoded(uint8_t *buf, size_t buf_size, jsonrpc_result_t *result, uint256_t *out);
int jsonrpc_decode_txreceipt(uint8_t *buf, size_t buf_size, jsonrpc_result_t *result, tx_receipt_t *out);

#ifdef __cplusplus
}
#endif

#endif /* _WEB3_JSONP_H_ */

