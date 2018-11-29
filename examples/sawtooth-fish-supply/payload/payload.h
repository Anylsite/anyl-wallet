#ifndef _PAYLOAD_H_
#define _PAYLOAD_H_
/* system includes */
/* local includes */
#include <stdint.h>
#include <stddef.h>


#ifdef  __cplusplus
extern "C" {
#endif

#define RECORD_SPECIES_LEN    4
#define RECORD_ID_LEN         16

typedef struct {
    uint64_t    weight;
    uint64_t    length;
    char        species[RECORD_SPECIES_LEN];
    char        id[RECORD_ID_LEN];
    int64_t     latitude;
    int64_t     longitude;
} fish_record_t;


int encode_create_agent(const char *agent_name, uint8_t *buf, size_t buf_size);
int encode_createRecord(const fish_record_t *rec, uint8_t *buf, size_t buf_size);
int encode_updateProperties(const fish_record_t *rec, uint8_t *buf, size_t buf_size);
int sign_and_encode(const st_privkey_t *privkey, const uint8_t *payload, size_t payload_size, uint8_t *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* _PAYLOAD_H_ */

