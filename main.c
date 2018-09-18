/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
*/

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "sdk_common.h"
#include "nrf_assert.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_crypto.h"
#include "nrf_crypto_ecc.h"
#include "nrf_crypto_error.h"
#include "nrf_crypto_ecdsa.h"
#include "mem_manager.h"
#include "helpers/crypto_helper.h"
#include "transaction.h"

static nrf_crypto_ecc_secp256k1_private_key_t private_key;
static nrf_crypto_ecc_secp256k1_public_key_t public_key;

static nrf_crypto_ecdsa_secp256r1_signature_t signature;
static size_t signature_size;

static void print_hex_array(uint8_t const * p_string, size_t size)
{
    #if NRF_LOG_ENABLED
    
    size_t i;
    NRF_LOG_RAW_INFO("    ");
    
    for(i = 0; i < size; i++)
    {
        NRF_LOG_RAW_INFO("%02x", p_string[i]);
    }
    
    #endif
}

static void print_hex(char const * p_msg, uint8_t const * p_data, size_t size)
{
    NRF_LOG_INFO(p_msg);
    
    print_hex_array(p_data, size);
    
    NRF_LOG_RAW_INFO("\r\n");
}


#define DEMO_ERROR_CHECK(error)     \
do                                  \
{                                   \
    if (error != NRF_SUCCESS)       \
    {                               \
        NRF_LOG_ERROR("Error 0x%04X: %s", error, nrf_crypto_error_string_get(error));\
        APP_ERROR_CHECK(error);     \
    }                               \
} while(0)

static void sign_transaction()
{
    ret_code_t err_code = NRF_SUCCESS;

    NRF_LOG_INFO("Signature generation");
    
    err_code = nrf_crypto_ecc_key_pair_generate(
                                                NULL, 
                                                &g_nrf_crypto_ecc_secp256k1_curve_info,
                                                &private_key, 
                                                &public_key);
    DEMO_ERROR_CHECK(err_code);

    nrf_crypto_ecc_secp256k1_raw_private_key_t private_key_raw;
    nrf_crypto_ecc_secp256k1_raw_public_key_t public_key_raw;

    err_code = nrf_crypto_ecc_public_key_to_raw(&public_key, public_key_raw, sizeof(public_key));
    DEMO_ERROR_CHECK(err_code);

    err_code = nrf_crypto_ecc_private_key_to_raw(&private_key, private_key_raw, sizeof(private_key));
    DEMO_ERROR_CHECK(err_code);

    print_hex("Private key: ", private_key_raw, sizeof(private_key_raw));
    print_hex("Public key: ", public_key_raw, sizeof(public_key_raw));
    
    char public_checksum_key_row[CRYPTO_HELPER_CHECKSUM_ADDRESS_LENGTH];
    generate_checksum_address(&public_key_raw, &public_checksum_key_row);
    NRF_LOG_HEXDUMP_INFO(public_checksum_key_row, sizeof(public_checksum_key_row));

    // RLP encode
    uint8_t *encoded_result = malloc(2048);
    uint32_t encoded_result_size;    

    Transaction transaction;
    transaction.nonce = 10;
    transaction.gas_price = 100;
    transaction.gas_limit = 200;
    transaction.to = "0x12";
    transaction.value = "0x6060";
    transaction.data = "0x112233";

    Signature transaction_signature;
    
    encode_transaction(transaction, transaction_signature, encoded_result, &encoded_result_size);
    NRF_LOG_DEBUG("RLP encoded transaction:");
    NRF_LOG_HEXDUMP_DEBUG(encoded_result, encoded_result_size);
    printf("RLP size: %d", encoded_result_size);

    // Hash
    uint8_t hash[KECCAK256_HASH_LENGTH];
    keccak256(encoded_result, hash, encoded_result_size);
    NRF_LOG_DEBUG("Keccak256 hash value:");
    NRF_LOG_HEXDUMP_DEBUG(hash, KECCAK256_HASH_LENGTH);

    // Sign
    signature_size = sizeof(signature);
    
    err_code = nrf_crypto_ecdsa_sign(
                                     NULL,
                                     &private_key,
                                     hash,
                                     KECCAK256_HASH_LENGTH,
                                     signature,
                                     &signature_size);
    DEMO_ERROR_CHECK(err_code);

    print_hex("Signature: ", signature, signature_size);

    uint8_t *r = malloc(SIGNATURE_LENGTH / 2);
    uint8_t *s = malloc(SIGNATURE_LENGTH / 2);
    uint8_t v = signature[SIGNATURE_LENGTH] + 27;

    memcpy(r, signature, SIGNATURE_LENGTH / 2);
    memcpy(s, signature + SIGNATURE_LENGTH / 2, SIGNATURE_LENGTH / 2);

    transaction_signature.r = r;
    transaction_signature.s = s;
    transaction_signature.v = v;

    print_hex("r: ", transaction_signature.r, SIGNATURE_LENGTH / 2);
    print_hex("s: ", transaction_signature.s, SIGNATURE_LENGTH / 2);
    printf("v: %02x", transaction_signature.v);

    uint32_t encoded_final_size;
    uint8_t *encoded_final = malloc(2048);

    encode_transaction(transaction, transaction_signature, encoded_final, &encoded_final_size);
    print_hex("RLP encoded transaction:", encoded_final, encoded_final_size);
}

static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

int main(void)
{
    ret_code_t err_code = NRF_SUCCESS;

    log_init();

    err_code = nrf_mem_init();
    DEMO_ERROR_CHECK(err_code);

    err_code = nrf_crypto_init();
    DEMO_ERROR_CHECK(err_code);

    sign_transaction();

    for (;;)
    {
    }
}

/** @}
 */
