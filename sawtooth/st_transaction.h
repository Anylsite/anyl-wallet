#ifndef _ST_TRANSACTION_H_
#define _ST_TRANSACTION_H_
/* system includes */
#include <stdint.h>
#include <stddef.h>
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

#define ST_PUBKEY_SIZE      65
#define FAMILY_NAME_MAX     16
#define FAMILY_VERSION_MAX  4
#define NONCE_MAX           4
#define SHA_512_SIZE        64
#define ST_ADDRESS_SIZE     32+3
#define ST_SIGNATURE_SIZE   64+1    // last byte is not used

typedef struct {
    uint8_t k[ST_PUBKEY_SIZE];
} st_pubkey_t;

typedef struct {
    uint8_t a[ST_ADDRESS_SIZE];
} st_address_t;

typedef struct {
    uint8_t s[ST_SIGNATURE_SIZE];
} st_signature_t;

typedef struct {
    st_address_t *lst;
    size_t       size;
} st_address_lst_t;

typedef struct {
    const uint8_t *buf;
    size_t size;
} st_payload_t;

// sawtooth uses 256bit ECDSA key
typedef struct {
    uint8_t k[32];
} st_privkey_t;

typedef struct {
    st_pubkey_t         batcher_pubkey;
    // dependencies R
    st_address_lst_t    dependencies;
    // family name
    uint8_t             family_name[FAMILY_NAME_MAX+1];
    // family version
    uint8_t             family_version[FAMILY_VERSION_MAX+1];
    // inputs R
    st_address_lst_t    inputs;
    // nonce
    uint8_t             nonce[NONCE_MAX+1];
    // outputs R
    st_address_lst_t    outputs;
    // payload_sha512
    uint8_t             payload_sha512[SHA_512_SIZE];
    // signer_pubkey
    st_pubkey_t         signer_pubkey;
} st_tx_hdr_t;

typedef struct {
    st_tx_hdr_t         header;
    st_signature_t      header_signature;
    st_payload_t        payload;
} st_tx_t;

typedef struct {
    const st_tx_t *lst;
    size_t size;
} st_tx_lst_t;

typedef struct {
    const st_tx_lst_t     *tx_lst;
    const st_pubkey_t     *signer_pubkey;
} st_batch_hdr_data_t;


int st_encode_header(const st_tx_hdr_t *header, uint8_t *buf, size_t buf_size);

int st_sign_tx(st_tx_t *tx, const st_privkey_t *privkey, uint8_t *buf, size_t buf_size);
int st_encode_tx(const st_tx_t *tx,  uint8_t *buf, size_t buf_size);

// encode a list of transactions. Make sure the all transactions are signed!
int st_encode_batch(
        const st_tx_t *transactions,
        size_t transactions_size,
        const st_privkey_t *batcher_privkey,
        const st_pubkey_t *signer_pubkey,
        uint8_t *buf, size_t buf_size);

int st_encode_tx_list(
        const st_tx_t *transactions,
        size_t transactions_size,
        uint8_t *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* _ST_TRANSACTION_H_ */

