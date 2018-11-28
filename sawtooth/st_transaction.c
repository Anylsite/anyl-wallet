/**
* @brief 
* @file st_transaction.c
* @author J.H. 
* @date 2018-11-09
*/

/* system includes */
#include <pb_encode.h>
#include <assert.h>
#include <trezor-crypto/sha2.h>

/* local includes */
#include "st_transaction.h"
#include "encoders.h"
#include "helpers/macros.h"
#include "eth/sign.h"
#include <trezor-crypto/secp256k1.h>
#include <trezor-crypto/ecdsa.h>
#include "transaction.pb.h"

static int st_sign_data_rawsig(
        const st_privkey_t *privkey,
        const uint8_t *data,
        size_t data_size,
        uint8_t *signature)
{
    uint8_t digest[32];
    uint8_t pby;
    static const ecdsa_curve *curve = &secp256k1;
    sha256_Raw(data, data_size, digest);
    ecdsa_sign_digest(curve, privkey->k, digest, signature, &pby, NULL);
    signature[64] = pby + 27;
    return 0;
}

int st_sign_tx(st_tx_t *tx, const st_privkey_t *privkey, uint8_t *buf, size_t buf_size)
{
    // hash the payload
    (void)sha512_Raw(tx->payload.buf, tx->payload.size, tx->header.payload_sha512);
    // encode the header
    int ret = st_encode_header(&tx->header, buf, buf_size);
    if(ret <= 0) { return -1; }
    // sign the header
    if(st_sign_data_rawsig(privkey, buf, ret, tx->header_signature.s) != 0) { return -1; }

    return 0;
}

//
// encode and sign the transaction
//
int st_encode_tx(const st_tx_t *tx,  uint8_t *buf, size_t buf_size)
{
    assert(tx != NULL);
    Transaction tx_proto = Transaction_init_zero;
    Transaction_init_encoder(&tx_proto);
    pb_ostream_t stream = pb_ostream_from_buffer(buf, buf_size);

    // set the signature
    tx_proto.header_signature.arg = (void*)&tx->header_signature;
    // set the payload
    tx_proto.payload.arg = (void*)&tx->payload;
    // set the header
    tx_proto.header.arg = (void*)&tx->header;
    // encode the transaction

    if (pb_encode(&stream, Transaction_fields, &tx_proto) == false) {
        return -1;
    }
    return stream.bytes_written;
}

int st_encode_header(const st_tx_hdr_t *header, uint8_t *buf, size_t buf_size)
{
    TransactionHeader hdr = TransactionHeader_init_zero;
    TransactionHeader_init_encoder(&hdr);

    pb_ostream_t stream = pb_ostream_from_buffer(buf, buf_size);

    hdr.batcher_public_key.arg =    (void*)&header->batcher_pubkey.k;
    hdr.family_name.arg =           (void*)header->family_name;
    hdr.family_version.arg =        (void*)header->family_version;
    hdr.nonce.arg =                 (void*)header->nonce;
    hdr.payload_sha512.arg =        (void*)&header->payload_sha512;
    hdr.signer_public_key.arg =     (void*)&header->signer_pubkey.k;
    hdr.inputs.arg =                (void*)&header->inputs;
    hdr.outputs.arg =               (void*)&header->outputs;
    hdr.dependencies.arg =          (void*)&header->dependencies;

    if (pb_encode(&stream, TransactionHeader_fields, &hdr) == false) {
        return -1;
    }

    return stream.bytes_written;
}

int st_encode_batch_header(const st_tx_t *transactions, size_t transactions_size, const st_pubkey_t *signer_pubkey, uint8_t *buf, size_t buf_size)
{
    BatchHeader batch_hdr = BatchHeader_init_default;
    BatchHeader_init_encoder(&batch_hdr);

    pb_ostream_t stream = pb_ostream_from_buffer(buf, buf_size);

    st_tx_lst_t tx_lst = { transactions, transactions_size };
    batch_hdr.transaction_ids.arg   =    (void*)&tx_lst;
    batch_hdr.signer_public_key.arg =    (void*)&signer_pubkey->k;

    if (pb_encode(&stream, BatchHeader_fields, &batch_hdr) == false) {
        return -1;
    }

    return stream.bytes_written;
}

int st_encode_batch(
        const st_tx_t *transactions,
        size_t transactions_size,
        const st_privkey_t *batcher_privkey,
        const st_pubkey_t *signer_pubkey,
        uint8_t *buf, size_t buf_size)
{
    Batch batch = Batch_init_default;
    Batch_init_encoder(&batch);
    // create batch header
    st_pubkey_t pubk = {0};
    size_t header_size = st_encode_batch_header(transactions, transactions_size, &pubk, buf, buf_size);
    if(header_size <= 0) { return -1; }
    if(header_size >= buf_size) { return -1; }
    // sign the header
    uint8_t header_signature[65];
    if(eth_sign_data_rawsig(batcher_privkey->k, buf, header_size, header_signature) != 0) { return -1; }
    // encode the batch
    //
    st_tx_lst_t tx_lst = { transactions, transactions_size };
    st_batch_hdr_data_t bh_data = {
        .tx_lst = &tx_lst,
        .signer_pubkey =  signer_pubkey
    };
    batch.header.arg            = (void*)&bh_data;
    batch.header_signature.arg  = (void*)header_signature;
    batch.transactions.arg      = (void*)&tx_lst;

    pb_ostream_t stream = pb_ostream_from_buffer(buf, buf_size);
    if (pb_encode(&stream, Batch_fields, &batch) == false) {
        return -1;
    }
    return stream.bytes_written;
}


int st_encode_tx_list(
        const st_tx_t *transactions,
        size_t transactions_size,
        uint8_t *buf, size_t buf_size)
{
    TransactionList tx_lst_proto = TransactionList_init_zero;
    TransactionList_init_encoder(&tx_lst_proto);

    st_tx_lst_t tx_lst = { transactions, transactions_size };
    tx_lst_proto.transactions.arg = (void*)&tx_lst;

    pb_ostream_t stream = pb_ostream_from_buffer(buf, buf_size);
    if (pb_encode(&stream, TransactionList_fields, &tx_lst_proto) == false) {
        return -1;
    }
    return stream.bytes_written;
}
