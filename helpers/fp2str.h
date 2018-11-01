#ifndef _FP2STR_H_
#define _FP2STR_H_
/* system includes */
#include <stdint.h>
#include <stddef.h>
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

int fp2str(uint32_t val, uint8_t digits, char *buf, size_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* _FP2STR_H_ */

