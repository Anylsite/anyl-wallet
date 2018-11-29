/**
* @brief 
* @file web3_rpc.c
* @author J.H. 
* @date 2018-10-23
*/

/* system includes */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#include <zephyr.h>
#pragma GCC diagnostic pop
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

/* local includes */
#include "web3_rpc.h"
#include "eth/web3.h"
#include "http_service.h"
#include "helpers/hextobin.h"
#include "eth/web3_jsonp.h"
#include "zephyr-wallet/utils.h"
#include "config.h"

#define WEB3_BUF_SIZE 512
static uint8_t WEB3_BUF[WEB3_BUF_SIZE];
static web3_ctx_t web3_ctx = {
    .buf = WEB3_BUF,
    .buf_size = WEB3_BUF_SIZE
};

static http_nfo_t http_client_nfo = {
    .host =         SERVER_ADDR,
    .port =         8545,
    .url =          "/",
    .content_type = "application/json",
    .headers =      NULL,
    .payload =      WEB3_BUF,
    .payload_size = WEB3_BUF_SIZE
};


int web3_eth_sendRawTransaction(const uint8_t *data, size_t data_len, uint256_t *out)
{
    if(eth_sendRawTransaction(&web3_ctx, data, data_len) < 0) {
        printk("error encoding eth_getTransactionCount() JSON\n");
        return -1;
    }
    if(http_send_data(&http_client_nfo) < 0) {
        printk("error: HTTP send\n");
        return -1;
    }
    jsonrpc_result_t res;
    int ret = jsonrpc_decode_hexencoded(
            http_client_nfo.content, http_client_nfo.content_size,
            &res, out);
    if(ret < 0) {
        printk("error: decoding result\n");
    } else if(res.error.code < 0) {
        printk("error: JSONRPC error: %s\n", res.error.message);
        ret = -1;
    }

    http_nfo_free(&http_client_nfo);
    return ret;
}

int web3_eth_blockNumber(uint64_t *out)
{
    if(eth_blockNumber(&web3_ctx) < 0) {
        printk("error encoding eth_blockNumber() JSON\n");
        return -1;
    }
    if(http_send_data(&http_client_nfo) < 0) {
        printk("error: HTTP send\n");
        return -1;
    }
    jsonrpc_result_t res;
    uint256_t out_u256;
    int ret = jsonrpc_decode_hexencoded(
                http_client_nfo.content, http_client_nfo.content_size,
                &res, &out_u256);
    if (ret < 0) {
        printk("error: decode result\n");
    } else {
        *out = LOWER(LOWER(out_u256));
    }
    http_nfo_free(&http_client_nfo);
    return ret;
}


static int web3_eth_address_block_hex(const char *method, const address_t *address, uint256_t *out)
{
    typedef int (*rpc_call_fp)(web3_ctx_t*, const address_t*);;
    struct {
        const char *name;
        rpc_call_fp call;
    } call_map[] = {
        {"eth_getBalance", eth_getBalance},
        {"eth_getTransactionCount", eth_getTransactionCount},
        {NULL, NULL}
    };
    rpc_call_fp rpc_call = NULL;

    for(size_t i = 0; i < sizeof(call_map); i++) {
        assert(call_map[i]->call != NULL);
        if(strcmp(call_map[i].name, method) == 0) {
            rpc_call = call_map[i].call;
            break;
        }
    }

    if(rpc_call(&web3_ctx, address) < 0) {
        printk("error encoding %s() JSON\n", method);
        return -1;
    }
    jsonrpc_result_t res;
    if(http_send_data(&http_client_nfo) < 0) {
        printk("error: HTTP send\n");
        return -1;
    }
    int ret = jsonrpc_decode_hexencoded(http_client_nfo.content, http_client_nfo.content_size, &res, out);
    if(ret < 0) {
        printk("error: decode result\n");
    }
    http_nfo_free(&http_client_nfo);
    return ret;
}

int web3_eth_getBalance(const address_t *address, uint256_t *out)
{
    return web3_eth_address_block_hex("eth_getBalance", address, out);
}

int web3_eth_getTransactionCount(const address_t *address, uint256_t *out)
{
    return web3_eth_address_block_hex("eth_getTransactionCount", address, out);
}

int web3_eth_estimateGas(const address_t *from, const transaction_t *tx, uint256_t *out)
{
    if(eth_estimateGas(&web3_ctx, from, tx) < 0) {
        printk("error encoding eth_estimateGas() JSON\n");
        return -1;
    }
    if(http_send_data(&http_client_nfo) < 0) {
        printk("error: HTTP send\n");
        return -1;
    }
    jsonrpc_result_t res;
    int ret = jsonrpc_decode_hexencoded(
                http_client_nfo.content, http_client_nfo.content_size,
                &res, out);
    if(ret < 0) {
        printk("error: decode result\n");
    } else if(res.error.code < 0) {
        printk("error: JSONRPC error: %s\n", res.error.message);
        ret = -1;
    }

    http_nfo_free(&http_client_nfo);
    return ret;
}

int web3_eth_call(const address_t *from, const transaction_t *tx, uint256_t *out, uint8_t tx_flags)
{
    if(eth_call(&web3_ctx, from, tx, tx_flags) < 0) {
        printk("error encoding eth_call() JSON\n");
        return -1;
    }
    if(http_send_data(&http_client_nfo) < 0) {
        printk("error: HTTP send\n");
        return -1;
    }
    jsonrpc_result_t res;
    int ret = jsonrpc_decode_hexencoded(
            http_client_nfo.content, http_client_nfo.content_size,
            &res, out);
    if(ret < 0) {
        printk("error: decode result\n");
        return -1;
    }
    if(res.error.code < 0) {
        printk("error: JSONRPC error: %s\n", res.error.message);
        return -1;
    }

    http_nfo_free(&http_client_nfo);
    return ret;
}

int web3_eth_getTransactionReceipt(const tx_hash_t *tx_hash, tx_receipt_t *out)
{
    if(eth_getTransactionReceipt(&web3_ctx, tx_hash) < 0) {
        printk("error encoding eth_getTransactionCount() JSON\n");
        return 0;
    }
    uint8_t *body = NULL;
    size_t content_len = 0;
    if(http_send_data(&http_client_nfo) < 0) {
        printk("error: HTTP send\n");
        return -1;
    }
    jsonrpc_result_t res;

    int ret = jsonrpc_decode_txreceipt(body, content_len, &res, out);
    if(ret < 0) {
        printk("error: decode result\n");
    } else if(res.error.code < 0) {
        printk("error: JSONRPC error: %s\n", res.error.message);
        ret = -1;
    }

    http_nfo_free(&http_client_nfo);
    return ret;
}
