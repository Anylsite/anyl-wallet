/**
* @brief 
* @file web3_jsonp.c
* @author J.H. 
* @date 2018-11-01
*/

/* system includes */
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

/* local includes */
#include "web3_jsonp.h"
#include "helpers/jsmn.h"
#include "helpers/jsmn_utils.h"
#include "helpers/hextobin.h"


static int __json_uint10(const uint8_t *buf, const jsmntok_t *tok, unsigned long *out)
{
    char *endptr = NULL;
    assert(buf != NULL);
    assert(tok != NULL);
    assert(out != NULL);
    errno = 0;
    *out = strtoul((const char*)buf + tok->start, &endptr, 10);
    if((errno != 0) || (endptr != (char*)(buf + tok->end))) {
        return -1;
    }
    return 0;
}

static int __json_int10(const uint8_t *buf, const jsmntok_t *tok, long *out)
{
    char *endptr = NULL;
    assert(buf != NULL);
    assert(tok != NULL);
    assert(out != NULL);
    errno = 0;
    *out = strtol((const char*)buf + tok->start, &endptr, 10);
    if((errno != 0) || (endptr != (char*)(buf + tok->end))) {
        return -1;
    }
    return 0;
}

static int __json_uint64_hex(const uint8_t *buf, const jsmntok_t *tok, uint64_t *out)
{
    char *endptr = NULL;
    assert(buf != NULL);
    assert(tok != NULL);
    assert(out != NULL);
    errno = 0;
    *out = strtol((const char*)buf + tok->start, &endptr, 16);
    if((errno != 0) || (endptr != (char*)(buf + tok->end))) {
        return -1;
    }
    return 0;
}

static int __json_uint256_hex(const uint8_t *buf, const jsmntok_t *tok, uint256_t *out)
{
    assert(buf != NULL);
    assert(tok != NULL);
    assert(out != NULL);
    // yes this is ugly. hextobin should support limit on buf
    char tmp[68] = {0};
    memcpy(tmp, buf + tok->start, tok->end - tok->start);
    return fromstring256(tmp, out);
}

static int __jsonrpc_error_obj(uint8_t *buf, const jsmntok_t *toks, jsonrpc_error_t *out)
{
    assert(buf != NULL);
    assert(toks != NULL);
    assert(out != NULL);
	if (toks[0].type != JSMN_OBJECT) {
		return -1;
	}
    // we expect no objects in the error message - the size is thus  tok->size * 2
	for (int i = 1; i < ((toks[0].size * 2)+1); i++) {
        if(toks[i].type != JSMN_STRING) { return -1; }
		if (jsoneq((const char*)buf, &toks[i], "code") == 0) {
            if(__json_int10(buf, &toks[i+1], &out->code) != 0) { return -1; }
            i++;
        } else if (jsoneq((const char*)buf, &toks[i], "message") == 0) {
            out->message = (const char *) buf + toks[i+1].start;
            buf[toks[i+1].end] = '\0';
            i++;
        }
    }

    return 0;
}

static int __json_txreceipt(uint8_t *buf, const jsmntok_t *toks, tx_receipt_t *out)
{
    assert(buf != NULL);
    assert(toks != NULL);
    assert(out != NULL);
	if (toks[0].type != JSMN_OBJECT) {
		return -1;
	}
    // we expect no objects in the error message - the size is thus  tok->size * 2
	for (int i = 1; i < toks[0].size; i++) {
        if(toks[i*2 + 1].type != JSMN_STRING) { return -1; }
		if (jsoneq((const char*)buf, &toks[i*2 + 1], "status") == 0) {
            uint64_t tmp;
            if(__json_uint64_hex(buf, &toks[i*2 + 2], &tmp) != 0) { return -1; }
            out->status = (tmp > 0);
        } else if (jsoneq((const char*)buf, &toks[i*2 + 1], "blockNumber") == 0) {
            if(__json_uint64_hex(buf, &toks[i*2 + 2], &out->blockNumber) != 0) { return -1; }
        }
    }

    return 0;
}

static int __jsonrpc_result(uint8_t *buf, const jsmntok_t *toks, size_t ntoks, jsonrpc_result_t *out)
{
    assert(buf != NULL);
    assert(toks != NULL);
    assert(ntoks > 0);
    assert(out != 0);
    int ret = 0;
    // we assume first token is always an object
	if (ntoks < 1 || toks[0].type != JSMN_OBJECT) {
		return -1;
	}
	for (size_t i = 1; i < ntoks; i++) {
        // jsonrpc item is intentionaly ignored
		if (jsoneq((const char*)buf, &toks[i], "error") == 0) {
            if((ret = __jsonrpc_error_obj(buf, &toks[i+1], &out->error)) < 0) { break; }
            i++;
        }
        else if (jsoneq((const char*)buf, &toks[i], "id") == 0) {
            if((ret = __json_uint10(buf, &toks[i+1], &out->id)) < 0) { break; }
            i++;
        }
    }
    return ret;
}

int jsonrpc_decode_hexencoded(uint8_t *buf, size_t buf_size, jsonrpc_result_t *result, uint256_t *out)
{
	jsmntok_t t[64]; /* We expect no more than 128 tokens */
    jsmn_parser p;
	jsmn_init(&p);
    int ret = 0;
	int r = jsmn_parse(&p, (const char*)buf, buf_size, t, sizeof(t)/sizeof(t[0]));
    if(r < 0) { return -1; }

    if(__jsonrpc_result(buf, t, r, result) < 0) { return -1; }

    // we assume first token is always an object
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		return -1;
	}
	for (int i = 1; i < r; i++) {
        // jsonrpc item is intentionaly ignored
        if (jsoneq((const char*)buf, &t[i], "result") == 0) {
            if((ret = __json_uint256_hex(buf, &t[i+1], out)) < 0) {
                break;
            }
            i++;
        }
    }
    return ret;
}

int jsonrpc_decode_txreceipt(uint8_t *buf, size_t buf_size, jsonrpc_result_t *result, tx_receipt_t *out)
{
	jsmntok_t t[64]; /* We expect no more than 128 tokens */
    jsmn_parser p;
	jsmn_init(&p);
    int ret = 0;
	int r = jsmn_parse(&p, (const char*)buf, buf_size, t, sizeof(t)/sizeof(t[0]));
    if(r < 0) { return -1; }

    if(__jsonrpc_result(buf, t, r, result) < 0) { return -1; }

    // we assume first token is always an object
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		return -1;
	}
	for (int i = 1; i < r; i++) {
        // jsonrpc item is intentionaly ignored
        if (jsoneq((const char*)buf, &t[i], "result") == 0) {
            if((ret = __json_txreceipt(buf, &t[i+1], out)) < 0) { break; }
            i++;
        }
    }
    return ret;
}
