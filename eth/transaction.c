/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
*/

#include "transaction.h"

#include <assert.h>

#include "helpers/byte_converter.h"
#include "helpers/crypto_helper.h"
#include "helpers/rlp_encoder.h"

#include <string.h>

void encode_transaction(Transaction transaction, Signature signature, uint8_t *result, uint32_t *result_size) {
    assert(result != NULL);
    assert(result_size != NULL);
    assert(*result_size > 100); // minimal realistic tx size is 100 bytes

    // Getting raw bytes from transaction properties

    // Nonce
    uint8_t nonce_bytes_raw[sizeof(transaction.nonce)] = {0};
    number_to_bytes(nonce_bytes_raw, transaction.nonce);

    // Gas price
    uint8_t gas_price_bytes_raw[sizeof(transaction.gas_price)] = {0};
    number_to_bytes(gas_price_bytes_raw, transaction.gas_price);

    // Gas limit
    uint8_t gas_limit_bytes_raw[sizeof(transaction.gas_limit)] = {0};
    number_to_bytes(gas_limit_bytes_raw, transaction.gas_limit);

    // To address
    uint8_t to_bytes_raw[sizeof(transaction.to)];
    memset(to_bytes_raw, 0, sizeof(transaction.to));
    char_to_bytes(to_bytes_raw, transaction.to);

    // Value
    uint8_t value_bytes_raw[sizeof(transaction.value)];
    memset(value_bytes_raw, 0, sizeof(transaction.value));
    char_to_bytes(value_bytes_raw, transaction.value);

    // Data
    uint8_t data_bytes_raw[sizeof(transaction.data)];
    memset(data_bytes_raw, 0, sizeof(transaction.data));
    char_to_bytes(data_bytes_raw, transaction.data);

    // Getting RLP encoding for transaction propoerties from raw bytes

    // Nonce
    uint8_t nonce_rlp_output[5] = {0};
    uint8_t encoded_nonce_length = encode_item(nonce_rlp_output, nonce_bytes_raw, sizeof(nonce_bytes_raw));

    // Gas price
    uint8_t gas_price_rlp_output[5] = {0};
    uint8_t encoded_gas_price_length = encode_item(gas_price_rlp_output, gas_price_bytes_raw, sizeof(gas_price_bytes_raw));

    // Gas limit
    uint8_t gas_limit_rlp_output[5] = {0};
    uint8_t encoded_gas_limit_length = encode_item(gas_limit_rlp_output, gas_limit_bytes_raw, sizeof(gas_limit_bytes_raw));

    // To address
    // 4 bytes header + 20 bytes address content
    uint8_t to_rlp_output[24] = {0};
    uint8_t encoded_to_length = encode_item(to_rlp_output, to_bytes_raw, sizeof(to_bytes_raw));

    // Value
    uint8_t value_rlp_output[5] = {0};
    uint8_t encoded_value_length = encode_item(value_rlp_output, value_bytes_raw, sizeof(value_bytes_raw));

    // Data
    uint8_t data_rlp_output[1024] = {0};
    uint8_t encoded_data_length = encode_item(data_rlp_output, data_bytes_raw, sizeof(data_bytes_raw));

    // R part of the signature, 1 bytes header + 32 bytes for content
    uint8_t r_rlp_output[33] = {0};
    uint8_t encoded_r_length = 0;

    if (signature.r) {
        encoded_r_length = encode_item(r_rlp_output, signature.r, SIGNATURE_LENGTH / 2);
    }

    // S part of the signature, 1 bytes header + 32 bytes for content
    uint8_t s_rlp_output[33] = {0};
    uint8_t encoded_s_length = 0;

    if (signature.s) {
        encoded_s_length = encode_item(s_rlp_output, signature.s, SIGNATURE_LENGTH / 2);
    }

    // V part of the signature
    uint8_t v_rlp_output[5] = {0};
    uint8_t encoded_v_length = 0;

    if (signature.v) {
        encoded_v_length = encode_item(v_rlp_output, &signature.v, sizeof(signature.v));
    }

    // Getting RLP encoding for header
    uint32_t encoded_properties_length = encoded_nonce_length +
                                         encoded_gas_price_length +
                                         encoded_gas_limit_length +
                                         encoded_to_length +
                                         encoded_value_length +
                                         encoded_data_length;

    if (signature.r && signature.s && signature.v) {
        encoded_properties_length += encoded_r_length + encoded_s_length + encoded_v_length;
    }

    uint8_t header_rlp_output[5];
    uint8_t encoded_header_length = encode_whole_header(header_rlp_output, encoded_properties_length);

    // Putting everything together in one big array of encoded raw bytes
    *result_size = encoded_header_length + encoded_properties_length;

    uint16_t offset = 0;

    memcpy(result, header_rlp_output, encoded_header_length);

    offset += encoded_header_length;
    memcpy(result + offset, nonce_rlp_output, encoded_nonce_length);

    offset += encoded_nonce_length;
    memcpy(result + offset, gas_price_rlp_output, encoded_gas_price_length);

    offset += encoded_gas_price_length;
    memcpy(result + offset, gas_limit_rlp_output, encoded_gas_limit_length);

    offset += encoded_gas_limit_length;
    memcpy(result + offset, to_rlp_output, encoded_to_length);

    offset += encoded_to_length;
    memcpy(result + offset, value_rlp_output, encoded_value_length);

    offset += encoded_value_length;
    memcpy(result + offset, data_rlp_output, encoded_data_length);

    if (signature.r && signature.s && signature.v) {
        offset += encoded_data_length;
        memcpy(result + offset, r_rlp_output, encoded_r_length);

        offset += encoded_r_length;
        memcpy(result + offset, s_rlp_output, encoded_s_length);

        offset += encoded_s_length;
        memcpy(result + encoded_s_length, v_rlp_output, encoded_v_length);
    }
    assert(offset > 0);
}
