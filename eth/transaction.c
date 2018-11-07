/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
*/

#include "transaction.h"

#include <assert.h>

#include "helpers/rlp_encoder.h"
#include "helpers/hextobin.h"
#include "helpers/macros.h"

#include <string.h>

int tx_encode(const transaction_t *tx, const signature_t *sig, uint8_t *buf, size_t buf_size)
{
    UNUSED_PARAM(buf_size);
    uint8_t *data_p = buf; // reserve
    uint16_t total_data_len = 0;
    uint16_t data_len = 0;
    // 0) nonce
    data_len = rlp_encode_integer(data_p, tx->nonce);
    data_p += data_len;
    total_data_len += data_len;
    // 1) gas_price
    data_len = rlp_encode_integer(data_p, tx->gas_price);
    data_p += data_len;
    total_data_len += data_len;
    // 2) gas_limit
    data_len = rlp_encode_integer(data_p, tx->gas_limit);
    data_p += data_len;
    total_data_len += data_len;
    // 3) to
    data_len = rlp_encode_item(data_p, tx->to.a, sizeof(tx->to.a));
    data_p += data_len;
    total_data_len += data_len;
    // 4) value
    data_len = rlp_encode_uint256(data_p, tx->value);
    data_p += data_len;
    total_data_len += data_len;
    // 5) data
    if((tx->data == NULL) || (tx->data_len == 0)) {
        data_len = rlp_encode_item(data_p, (uint8_t*)&tx->data, 1);
    } else {
        data_len = rlp_encode_item(data_p, tx->data, tx->data_len);
    }
    data_p += data_len;
    total_data_len += data_len;

    if(sig != NULL) {
        data_len = rlp_encode_integer(data_p, sig->v);
        data_p += data_len;
        total_data_len += data_len;
        data_len = rlp_encode_uint256(data_p, sig->r);
        data_p += data_len;
        total_data_len += data_len;
        data_len = rlp_encode_uint256(data_p, sig->s);
        data_p += data_len;
        total_data_len += data_len;
    }

    // finally prepend the header
    uint8_t header_len = rlp_header_len(total_data_len);
    memmove(buf + header_len, buf, total_data_len);
    rlp_encode_array_header(buf, total_data_len);
    total_data_len += header_len;
    return total_data_len;
}

int tx_set_to(transaction_t *tx, const char *to_hex)
{
    assert(tx != NULL);
    assert(to_hex != NULL);
    int ret = hextobin(to_hex, tx->to.a, sizeof(tx->to.a));
    if(ret < 0)     { return -1; }
    if(ret != 20)   { return -1; }
    return 0;
}

void tx_set_value_u64(transaction_t *tx, uint64_t val)
{
    set256_uint64(&tx->value, val);
}

int tx_encode_sign(const transaction_t *tx, const uint8_t *privkey, uint8_t *buf, size_t buf_size)
{
    signature_t signature;
    int unsigned_tx_len = tx_encode(tx, NULL, buf, buf_size);
    // create unsigned transaction, RLP encoded
    if(unsigned_tx_len < 0) { return -1; }
    // (nonce, gas_price, gas_limit, to, value, data)
    // keccak256(rlp_encoded_tx)
    eth_sign_data(privkey, buf, unsigned_tx_len, &signature);
    // sign(tx_hash)
    // create transaction again, this time add v, r, s of the signature
    return tx_encode(tx, &signature, buf, buf_size);
}
