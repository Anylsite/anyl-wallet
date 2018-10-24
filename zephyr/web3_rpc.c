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
#include "web3_json.h"
#include "eth/web3.h"
#include "http_service.h"
#include "helpers/hextobin.h"
#include "zephyr/utils.h"

#define WEB3_BUF_SIZE 512
static uint8_t WEB3_BUF[WEB3_BUF_SIZE];
static web3_ctx_t web3_ctx = {
    .buf = WEB3_BUF,
    .buf_size = WEB3_BUF_SIZE
};

int web3_eth_sendRawTransaction(const uint8_t *data, size_t data_len)
{
    if(eth_sendRawTransaction(&web3_ctx, data, data_len) < 0) {
        printk("error encoding eth_getTransactionCount() JSON\n");
        return 0;
    }
    uint8_t *body = NULL;
    size_t content_len = 0;
    if(http_send_data(NULL, web3_ctx.buf, web3_ctx.buf_used, &body, &content_len) < 0) {
        printk("error: HTTP send\n");
        return 0;
    }
    jsonrpc_result_t res;
    int ret = jsonrpc_decode_hexencoded(body, content_len, &res);
    if(ret < 0) {
        printk("error: decode result\n");
        return 0;
    }
    if(res.error.code < 0) {
        printk("error: JSONRPC error: %s\n", res.error.message);
        return 0;
    }

/*    printk("%s\n", res.result);*/
    //////////////////////////

    return 0;
}

int web3_eth_blockNumber(uint64_t *out)
{
    if(eth_blockNumber(&web3_ctx) < 0) {
        printk("error encoding eth_blockNumber() JSON\n");
        return -1;
    }
    uint8_t *body = NULL;
    size_t content_len = 0;
    if(http_send_data(NULL, web3_ctx.buf, web3_ctx.buf_used, &body, &content_len) < 0) {
        printk("error: HTTP send\n");
        return -1;
    }
    jsonrpc_result_t res;
    if(jsonrpc_decode_hexencoded(body, content_len, &res) < 0) {
        printk("error: decode result\n");
        return -1;
    }
    *out = strtol(res.result, NULL, 16);
    if(errno != 0) { return -1; }
    return 0;
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
        return 0;
    }
    uint8_t *body = NULL;
    size_t content_len = 0;
    if(http_send_data(NULL, web3_ctx.buf, web3_ctx.buf_used, &body, &content_len) < 0) {
        printk("error: HTTP send\n");
        return -1;
    }
    jsonrpc_result_t res;
    if(jsonrpc_decode_hexencoded(body, content_len, &res) < 0) {
        printk("error: decode result\n");
        return -1;
    }
    assert((strlen(res.result)) > 2 && (strlen(result) < 64 + 2));

    clear256(out);
    if(strlen(res.result) < (16 + 2)) {
        uint64_t x = strtol(res.result, NULL, 16);
        if(errno != 0) { return -1; }
        readu64LE((uint8_t*)&x, &LOWER_P(out));
    } else {
        // convert ASCII to bin
        uint8_t buf[32] = {0};
        uint8_t *buf_p = buf + sizeof(buf) - ((strlen(res.result) - 2) / 2);
        hextobin(res.result, buf_p, sizeof(buf) - (buf_p - buf));
        // convert bin to uint256_t
        readu256BE(buf, out);
    }

    return 0;
}

int web3_eth_getBalance(const address_t *address, uint256_t *out)
{
    return web3_eth_address_block_hex("eth_getBalance", address, out);
}

int web3_eth_getTransactionCount(const address_t *address, uint256_t *out)
{
    return web3_eth_address_block_hex("eth_getTransactionCount", address, out);
}
