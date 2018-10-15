#ifndef _ENDIAN_H_
#define _ENDIAN_H_
/* system includes */
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif


#ifndef __ZEPHYR__
	#warning "This header file (endian.h) is Zephyr specific.\n"
#endif	/* __APPLE__ */

#include <misc/byteorder.h>

#define htobe16(x) sys_cpu_to_be16(x)
#define htole16(x) sys_cpu_to_le16(x)
#define be16toh(x) sys_be16_to_cpu(x)
#define le16toh(x) sys_le16_to_cpu(x)

#define htobe32(x) sys_cpu_to_be32(x)
#define htole32(x) sys_cpu_to_le32(x)
#define be32toh(x) sys_be32_to_cpu(x)
#define le32toh(x) sys_be32_to_cpu(x)

#define htobe64(x) sys_cpu_to_be64(x)
#define htole64(x) sys_cpu_to_le64(x)
#define be64toh(x) sys_be64_to_cpu(x)
#define le64toh(x) sys_le64_to_cpu(x)

#ifdef __cplusplus
}
#endif

#endif /* _ENDIAN_H_ */

