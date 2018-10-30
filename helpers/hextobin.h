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
// permissive hextobin - this doesn't require strlen to be multiple of 2
int hextobin_2(const char * str, uint8_t * bytes, size_t blen);
int bintohex(const uint8_t * bytes, size_t blen, char *out, size_t out_len);

// convert a byte array to a string. This won't terminate the string with a NUL char
int bintohex_nonull(const uint8_t * bytes, size_t blen, char *out, size_t out_len);

#ifdef __cplusplus
}
#endif

#endif /* _HEXTOBIN_H_ */

