#ifndef _PAYLOAD_H_
#define _PAYLOAD_H_
/* system includes */
/* local includes */
#include <stdint.h>
#include <stddef.h>


#ifdef  __cplusplus
extern "C" {
#endif
int encode_create_agent(const char *agent_name, uint8_t *buf, size_t buf_size);
int encode_properties(uint8_t *buf, size_t buf_size);
int encode_updateProperties(uint8_t *buf, size_t buf_size);
int sign_and_encode(const uint8_t *payload, size_t payload_size, uint8_t *buf, size_t buf_size);

#ifdef __cplusplus
}
#endif

#endif /* _PAYLOAD_H_ */

