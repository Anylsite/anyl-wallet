#ifndef _FISH_ENCODERS_H_
#define _FISH_ENCODERS_H_
/* system includes */
#include "payload.pb.h"
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    PropertyValue   *lst;
    size_t          size;
} PropertyValue_lst_t;

// initialize payload encoder. this expects that the action field is set

void SCPayload_init_encoder(SCPayload *payload);
void PropertyValue_init_encoder(PropertyValue *value, PropertySchema_DataType type);

#ifdef __cplusplus
}
#endif

#endif /* _FISH_ENCODERS_H_ */

