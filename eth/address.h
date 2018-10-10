#ifndef _ADDRESS_H_
#define _ADDRESS_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

int privkey_to_ethereum_address(const uint8_t *privkey, uint8_t *addr);

#ifdef __cplusplus
}
#endif

#endif /* _ADDRESS_H_ */

