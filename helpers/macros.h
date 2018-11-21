#ifndef _MACROS_H_
#define _MACROS_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define UNUSED_PARAM(x) ((void)(x));

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

#ifdef __cplusplus
}
#endif

#endif /* _MACROS_H_ */

