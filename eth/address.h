#ifndef _ADDRESS_H_
#define _ADDRESS_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

typedef uint8_t address_t[20];
int privkey_to_ethereum_address(const uint8_t *privkey, address_t *addr_out);

#ifdef __cplusplus
}
#endif

#endif /* _ADDRESS_H_ */

