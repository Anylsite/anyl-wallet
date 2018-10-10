#ifndef _HEXTOBIN_H_
#define _HEXTOBIN_H_
/* system includes */
/* local includes */
#include <stddef.h>
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define E_EMPTY_STRING          (-1)
#define E_INVALID_STRING_LEN    (-2)
#define E_INVALID_CHAR          (-3)

int hextobin(const char * str, uint8_t * bytes, size_t blen);

#ifdef __cplusplus
}
#endif

#endif /* _HEXTOBIN_H_ */

