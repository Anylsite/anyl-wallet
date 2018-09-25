#ifndef _HEXTOBIN_H_
#define _HEXTOBIN_H_
/* system includes */
/* local includes */
#include <stddef.h>
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif
void hextobin(const char * str, uint8_t * bytes, size_t blen);

#ifdef __cplusplus
}
#endif

#endif /* _HEXTOBIN_H_ */

