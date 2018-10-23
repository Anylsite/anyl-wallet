#ifndef _WEB3_JSON_H_
#define _WEB3_JSON_H_
/* system includes */
#include <stddef.h>
#include <stdint.h>

/* local includes */
#include "helpers/uint256.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    int code;
    const char *message;
} jsonrpc_error_t;

typedef struct {
    const char *jsonrpc;
    int id;
    const char *result;
    jsonrpc_error_t error;
} jsonrpc_result_t;

typedef struct {
    int blockNumber;
    const char *from;
    const char *to;
    const char *status;
} tx_receipt_result_t;


int jsonrpc_decode_hexencoded(const uint8_t *buf, size_t buf_size, jsonrpc_result_t *result);

#ifdef __cplusplus
}
#endif

#endif /* _WEB3_JSON_H_ */

