#ifndef _ENCODERS_H_
#define _ENCODERS_H_
/* system includes */
#include <stdbool.h>
/* local includes */
#include "transaction.pb.h"
#include "batch.pb.h"


#ifdef  __cplusplus
extern "C" {
#endif

// helper functions for encoding an item
// encode a byte array. This expects st_payload_t as an argument
bool write_bytes(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
// write a string. This expects a null-terminated string as an argument
bool write_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
bool write_pubkey(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
bool write_varint(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);

// initialize default encoders for the message
void BatchHeader_init_encoder(BatchHeader *bh);
void Batch_init_encoder(Batch *bh);
void Transaction_init_encoder(Transaction *tx);
void TransactionHeader_init_encoder(TransactionHeader *th);
void TransactionList_init_encoder(TransactionList *tx_lst);

#ifdef __cplusplus
}
#endif

#endif /* _ENCODERS_H_ */

