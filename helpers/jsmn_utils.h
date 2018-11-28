#ifndef _JSMN_UTILS_H_
#define _JSMN_UTILS_H_
/* system includes */
/* local includes */
#include "jsmn.h"

#ifdef  __cplusplus
extern "C" {
#endif

int jsoneq(const char *json, const jsmntok_t *tok, const char *s);

#ifdef __cplusplus
}
#endif

#endif /* _JSMN_UTILS_H_ */

