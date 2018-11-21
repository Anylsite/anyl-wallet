/**
* @brief 
* @file encoders.c
* @author J.H. 
* @date 2018-11-21
*/

/* system includes */
#include <pb_encode.h>

/* local includes */
#include "encoders.h"
#include "st_transaction.h"
#include "helpers/hextobin.h"

// encode binary array of length N as an hexstring
#define PB_WRITE_BYTES(x)\
static bool write_bytes_ ##x (pb_ostream_t *stream, const pb_field_t *field, void * const *arg)\
{\
    if(!pb_encode_tag_for_field(stream, field))      { return false; }\
    if (!pb_encode_varint(stream, (uint64_t)(x)*2))   { return false; }\
    char out_buf[2];\
    for(size_t i = 0; i < (x); i++) {\
        bintohex_nonull(&(((const uint8_t *)(*arg))[i]), 1, out_buf, sizeof(out_buf));\
        if(!pb_write(stream, (unsigned char*)out_buf, sizeof(out_buf))) { return false; }\
    }\
    return true;\
}

PB_WRITE_BYTES(65)
PB_WRITE_BYTES(64)
/*PB_WRITE_BYTES(65)*/
PB_WRITE_BYTES(35)
PB_WRITE_BYTES(33)
PB_WRITE_BYTES(3)

bool write_pubkey(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    return write_bytes_65(stream, field, arg);
}


// write a simple string
bool write_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    if(*arg == NULL) { return true; }
    return pb_encode_tag_for_field(stream, field) &&
           pb_encode_string(stream, *arg, strlen(*arg));
}

// encode a list of addresses
static bool write_address_list(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    const st_address_lst_t *lst = (const st_address_lst_t *)*arg;
    for(size_t i = 0; i < lst->size; i++) {
/*        if(!pb_encode_tag_for_field(stream, field)) { return false; }*/
        void * arg = &(lst->lst[i].a);
        if(!write_bytes_35(stream, field, &arg)) { return false; }
    }

    return true;
}

static bool write_input_list(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    const st_address_lst_t *lst = (const st_address_lst_t *)*arg;
    for(size_t i = 0; i < lst->size; i++) {
/*        if(!pb_encode_tag_for_field(stream, field)) { return false; }*/
        void * arg = &(lst->lst[i].a);
        if(!write_bytes_3(stream, field, &arg)) { return false; }
    }

    return true;
}

bool write_bytes(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    const st_payload_t *pl = (const st_payload_t *)*arg;
    if(!pb_encode_tag_for_field(stream, field)) { return false; }
    if(!pb_encode_varint(stream, pl->size)) { return false; }
    if(!pb_write(stream, pl->buf, pl->size)) { return false; }

    return true;
}

static bool write_payload(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    return write_bytes(stream, field, arg);
}

bool write_varint(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    return pb_encode_tag_for_field(stream, field) &&
           pb_encode_varint(stream, (long)*arg);
}

static bool write_tx_header(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    TransactionHeader hdr = TransactionHeader_init_zero;
    st_tx_hdr_t *header = (st_tx_hdr_t*) *arg;
    TransactionHeader_init_encoder(&hdr);

    hdr.batcher_public_key.arg =    (void*)&header->batcher_pubkey.k;
    hdr.family_name.arg =           (void*)header->family_name;
    hdr.family_version.arg =        (void*)header->family_version;
    hdr.nonce.arg =                 (void*)header->nonce;
    hdr.payload_sha512.arg =        (void*)&header->payload_sha512;
    hdr.signer_public_key.arg =     (void*)&header->signer_pubkey.k;
    hdr.inputs.arg =                (void*)&header->inputs;
    hdr.outputs.arg =               (void*)&header->outputs;
    hdr.dependencies.arg =          (void*)&header->dependencies;
    return pb_encode_tag_for_field(stream, field) &&
           pb_encode_submessage(stream, TransactionHeader_fields, &hdr);
}

static bool write_batch_header(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    BatchHeader hdr = BatchHeader_init_default;
    BatchHeader_init_encoder(&hdr);
    st_batch_hdr_data_t *hdr_data = (st_batch_hdr_data_t*) *arg;
    hdr.transaction_ids.arg   =    (void*)hdr_data->tx_lst;
    hdr.signer_public_key.arg =    (void*)hdr_data->signer_pubkey;
    return pb_encode_tag_for_field(stream, field) &&
           pb_encode_submessage(stream, BatchHeader_fields, &hdr);
}

static bool write_transaction_ids(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    st_tx_lst_t *lst = (st_tx_lst_t*)*arg;
    for(size_t i = 0; i < lst->size; i++) {
        void * arg = (void *)&(lst->lst[i].header_signature.s);
        if(!write_bytes_64(stream, field, &arg)) { return false; }
    }
    return true;
}

static bool write_transaction(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    Transaction tx_proto = Transaction_init_zero;
    Transaction_init_encoder(&tx_proto);
    st_tx_t *tx = (st_tx_t *)*arg;

    tx_proto.header_signature.arg = &tx->header_signature;
    // set the payload
    tx_proto.payload.arg = &tx->payload;
    // set the header
    tx_proto.header.arg = &tx->header;
    return pb_encode_tag_for_field(stream, field) &&
           pb_encode_submessage(stream, Transaction_fields, &tx_proto);
}

static bool write_transactions(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    const st_tx_lst_t *lst = (const st_tx_lst_t*)*arg;
    for(size_t i = 0; i < lst->size; i++) {
        void * arg = (void *)&(lst->lst[i]);
        if(!write_transaction(stream, field, &arg)) { return false; }
    }
    return true;
}

void TransactionHeader_init_encoder(TransactionHeader *th)
{
    th->nonce.funcs.encode              = &write_string;
    th->family_name.funcs.encode        = &write_string;
    th->family_version.funcs.encode     = &write_string;
    th->payload_sha512.funcs.encode     = &write_bytes_64;
    th->signer_public_key.funcs.encode  = &write_pubkey;
    th->batcher_public_key.funcs.encode = &write_bytes_33;
    th->inputs.funcs.encode             = &write_input_list;
    th->outputs.funcs.encode            = &write_input_list;
    th->dependencies.funcs.encode       = &write_address_list;
}

void Transaction_init_encoder(Transaction *tx)
{
    tx->header_signature.funcs.encode   = &write_bytes_64;
    tx->payload.funcs.encode            = &write_payload;
    tx->header.funcs.encode             = &write_tx_header;
}

void BatchHeader_init_encoder(BatchHeader *bh)
{
    bh->signer_public_key.funcs.encode  = &write_pubkey;
    bh->transaction_ids.funcs.encode    = &write_transaction_ids;
}

void Batch_init_encoder(Batch *batch)
{
    batch->header.funcs.encode              = &write_batch_header;
    batch->header_signature.funcs.encode    = &write_bytes_64;
    batch->transactions.funcs.encode        = &write_transactions;
}

void TransactionList_init_encoder(TransactionList *tx_lst)
{
    tx_lst->transactions.funcs.encode       = &write_transactions;
}
