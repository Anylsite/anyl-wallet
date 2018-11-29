/**
* @brief 
* @file payload.c
* @author J.H. 
* @date 2018-11-26
*/

/* system includes */
#include <assert.h>
#include "pb_encode.h"
#include <posix/time.h>

/* local includes */
#include "fish_encoders.h"
#include "sawtooth/st_transaction.h"
#include "helpers/hextobin.h"
#include "eth/sign.h"
#include "payload.h"

static void init_payload(SCPayload *payload, SCPayload_Action action)
{
    payload->action = action;
    SCPayload_init_encoder(payload);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    payload->timestamp = tv.tv_sec;
}

int encode_create_agent(const char *agent_name, uint8_t *buf, size_t buf_size)
{
    SCPayload payload = SCPayload_init_default;
    init_payload(&payload, SCPayload_Action_CREATE_AGENT);
    payload.create_agent.name.arg = (void*)agent_name;

    pb_ostream_t stream = pb_ostream_from_buffer(buf, buf_size);
    if (pb_encode(&stream, SCPayload_fields, &payload) == false) {
        return -1;
    }
    return stream.bytes_written;
}

static void fish_record_set_properties(const fish_record_t *rec, PropertyValue *properties, size_t properties_len)
{
    assert(properties_len >= 4);
    PropertyValue_init_encoder(&properties[0], PropertySchema_DataType_NUMBER);
    PropertyValue_init_encoder(&properties[1], PropertySchema_DataType_NUMBER);
    PropertyValue_init_encoder(&properties[2], PropertySchema_DataType_STRING);
    PropertyValue_init_encoder(&properties[3], PropertySchema_DataType_LOCATION);

    properties[0].name.arg = (void*)&"weight";
    properties[0].number_value = rec->weight;
    properties[1].name.arg = (void*)&"length";
    properties[1].number_value = rec->length;
    properties[2].name.arg = (void*)&"species";
    properties[2].string_value.arg = (void*)rec->species;
    properties[3].name.arg = (void*)&"location";
    properties[3].location_value.latitude = rec->latitude;
    properties[3].location_value.longitude = rec->longitude;
}

int encode_createRecord(const fish_record_t *rec, uint8_t *buf, size_t buf_size)
{
    assert(rec != NULL);
    SCPayload payload = SCPayload_init_default;
    init_payload(&payload, SCPayload_Action_CREATE_RECORD);

    PropertyValue properties[4] = { 0 };
    PropertyValue_lst_t property_lst = { properties, 4 };
    fish_record_set_properties(rec, properties, 4);

    payload.create_record.properties.arg        = &property_lst;
    payload.create_record.record_id.arg         = (void*)rec->id;
    payload.create_record.record_type.arg       = (void*)&"fish";

    pb_ostream_t stream = pb_ostream_from_buffer(buf, buf_size);
    if (pb_encode(&stream, SCPayload_fields, &payload) == false) {
        return -1;
    }
    return stream.bytes_written;
}

int encode_updateProperties(const fish_record_t *rec, uint8_t *buf, size_t buf_size)
{
    SCPayload payload = SCPayload_init_default;
    init_payload(&payload, SCPayload_Action_UPDATE_PROPERTIES);

    PropertyValue properties[4] = { 0 };
    PropertyValue_lst_t property_lst = { properties, 4 };
    fish_record_set_properties(rec, properties, 4);

    payload.update_properties.properties.arg        = &property_lst;
    payload.update_properties.record_id.arg             = (void*)rec->id;

    pb_ostream_t stream = pb_ostream_from_buffer(buf, buf_size);
    if (pb_encode(&stream, SCPayload_fields, &payload) == false) {
        return -1;
    }
    return stream.bytes_written;
}

int sign_and_encode(const st_privkey_t *privkey, const uint8_t *payload, size_t payload_size, uint8_t *buf, size_t buf_size)
{
    st_tx_t tx;
    st_pubkey_t pubkey;
    st_address_t input, output;
    privkey_to_pubkey(privkey->k, pubkey.k);

    memset(&tx, 0, sizeof(tx));
    const char *family_name = "supply_chain";
    strncpy((char*)&tx.header.family_name, family_name, strlen(family_name));
    strncpy((char*)&tx.header.family_version, "1.1", sizeof(tx.header.family_version));
    strncpy((char*)&tx.header.nonce, "42", sizeof(tx.header.nonce));
    tx.header.signer_pubkey = pubkey;
    hextobin(
            "034f355bdcb7cc0af728ef3cceb9615d90684bb5b2ca5f859ab0f0b704075871aa",
            tx.header.batcher_pubkey.k, sizeof(tx.header.batcher_pubkey.k)
           );
    hextobin(
            "3400de",
            input.a, sizeof(input.a)
            );
    tx.header.inputs.lst = &input;
    tx.header.inputs.size = 1;
    hextobin(
            "3400de",
            output.a, sizeof(output.a)
            );
    tx.header.outputs.lst = &output;
    tx.header.outputs.size = 1;

    tx.payload.buf = (const uint8_t *)payload;
    tx.payload.size = payload_size;
    st_sign_tx(&tx, privkey, buf, buf_size);
    return st_encode_tx_list(
            &tx, 1,
            buf, buf_size);
}

