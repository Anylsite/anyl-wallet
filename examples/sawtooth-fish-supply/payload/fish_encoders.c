/**
* @brief 
* @file fish_encoders.c
* @author J.H. 
* @date 2018-11-22
*/

/* system includes */
#include "pb_encode.h"
#include "sawtooth/encoders.h"
#include "agent.pb.h"

/* local includes */
#include "fish_encoders.h"


// this expects an initialized PropertyValue_lst_t as an argument
static bool write_properties(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    PropertyValue_lst_t *lst = (PropertyValue_lst_t *)*arg;
    for(size_t i = 0; i < lst->size; i++) {
        if(!(pb_encode_tag_for_field(stream, field) &&
             pb_encode_submessage(stream, PropertyValue_fields, &lst->lst[i]))) {
            return false;
        }
    }
    return true;
}

static bool write_struct_values(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
    // TBD
    return true;
}

void PropertyValue_init_encoder(PropertyValue *value, PropertySchema_DataType type)
{
    value->name.funcs.encode        = &write_string;
    value->data_type                = type;
    switch(type) {
        default:
        case PropertySchema_DataType_STRING:
            value->string_value.funcs.encode    = &write_string;
            break;
        case PropertySchema_DataType_BYTES:
            value->bytes_value.funcs.encode     = &write_bytes;
            break;
        case PropertySchema_DataType_STRUCT:
            value->struct_values.funcs.encode   = &write_struct_values;
            break;
        case PropertySchema_DataType_ENUM:
            // not supported yet
            break;
        // number, boolean and location don't need encoder and are set directly
        case PropertySchema_DataType_LOCATION:
        case PropertySchema_DataType_BOOLEAN:
        case PropertySchema_DataType_NUMBER:
        case PropertySchema_DataType_TYPE_UNSET:
            break;
    }
}

void Agent_init_encoder(Agent *agent)
{
    agent->public_key.funcs.encode  = &write_pubkey;
    agent->name.funcs.encode        = &write_string;
}

void UpdatePropertiesAction_init_encoder(UpdatePropertiesAction *action)
{
    action->record_id.funcs.encode      = &write_string;
    action->properties.funcs.encode     = &write_properties;
}

void CreateRecordAction_init_encoder(CreateRecordAction *action)
{
    action->record_id.funcs.encode      = &write_string;
    action->record_type.funcs.encode    = &write_string;
    action->properties.funcs.encode     = &write_properties;
}

void CreateProposalAction_init_encoder(CreateProposalAction *action)
{
    action->record_id.funcs.encode          = &write_string;
    action->receiving_agent.funcs.encode    = &write_string;
/*    action->properties.funcs.encode    = &TODO;*/
}

void AnswerProposalAction_init_encoder(AnswerProposalAction *action)
{
    action->record_id.funcs.encode          = &write_string;
    action->receiving_agent.funcs.encode    = &write_string;
/*    action->role.funcs.encode    = &TODO;*/
/*    action->response.funcs.encode    = &TODO;*/
}

void RevokeReporterAction_init_encoder(RevokeReporterAction *action)
{
    action->record_id.funcs.encode          = &write_string;
    action->reporter_id.funcs.encode          = &write_string;
/*    action->properties.funcs.encode          = &TODO;*/
}

void SCPayload_init_encoder(SCPayload *payload)
{
    switch(payload->action) {
        case SCPayload_Action_CREATE_AGENT:
            payload->create_agent.name.funcs.encode             = &write_string;
            break;
        case SCPayload_Action_CREATE_RECORD:
            CreateRecordAction_init_encoder(&payload->create_record);
            break;
        case SCPayload_Action_UPDATE_PROPERTIES:
            UpdatePropertiesAction_init_encoder(&payload->update_properties);
            break;
        case SCPayload_Action_FINALIZE_RECORD:
            payload->finalize_record.record_id.funcs.encode     = &write_string;
        default:
            break;
    }
}
