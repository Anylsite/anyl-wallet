#ifndef _COMPAT_H_
#define _COMPAT_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

// to provide rand() function for the trezor-library
// note: this function is actually not used anywhere at the moment
int rand();

#ifdef __cplusplus
}
#endif

#endif /* _COMPAT_H_ */

