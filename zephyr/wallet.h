#ifndef _WALLET_H_
#define _WALLET_H_
/* system includes */
#include <stdint.h>
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t k[32];
} privkey_t;

#ifdef __cplusplus
}
#endif

#endif /* _WALLET_H_ */

