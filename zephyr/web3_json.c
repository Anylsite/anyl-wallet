/**
* @brief 
* @file web3_json.c
* @author J.H. 
* @date 2018-10-22
*/

/* system includes */
#include <json.h>

/* local includes */
#include "web3_json.h"

static const struct json_obj_descr jsonrpc_error_descr[] = {
	JSON_OBJ_DESCR_PRIM(jsonrpc_error_t, code, JSON_TOK_NUMBER),
    JSON_OBJ_DESCR_PRIM(jsonrpc_error_t, message, JSON_TOK_STRING),
};
static const struct json_obj_descr jsonrpc_hexstring_descr[] = {
	JSON_OBJ_DESCR_PRIM(jsonrpc_result_t, jsonrpc, JSON_TOK_STRING),
    JSON_OBJ_DESCR_PRIM(jsonrpc_result_t, id, JSON_TOK_NUMBER),
    JSON_OBJ_DESCR_PRIM(jsonrpc_result_t, result, JSON_TOK_STRING),
    JSON_OBJ_DESCR_OBJECT(jsonrpc_result_t, error, jsonrpc_error_descr)
};

static const struct json_obj_descr tx_receipt_desc[] = {
    JSON_OBJ_DESCR_PRIM(tx_receipt_result_t, blockNumber, JSON_TOK_NUMBER),
    JSON_OBJ_DESCR_PRIM(tx_receipt_result_t, from, JSON_TOK_STRING),
    JSON_OBJ_DESCR_PRIM(tx_receipt_result_t, to, JSON_TOK_STRING),
    JSON_OBJ_DESCR_PRIM(tx_receipt_result_t, status, JSON_TOK_STRING),
};

static int __init_jsonrpc_result(jsonrpc_result_t *res)
{
    memset(res, 0, sizeof(jsonrpc_result_t));
}

int jsonrpc_decode_hexencoded(const uint8_t *buf, size_t buf_size, jsonrpc_result_t *result)
{
    return json_obj_parse(
            buf, buf_size,
            jsonrpc_hexstring_descr, ARRAY_SIZE(jsonrpc_hexstring_descr),
            result);
}
