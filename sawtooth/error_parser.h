#ifndef _ERROR_PARSER_H_
#define _ERROR_PARSER_H_
/* system includes */
#include <stddef.h>
/* local includes */


#ifdef  __cplusplus
extern "C" {
#endif

// parse a JSON error reply returned by the node
// 
int st_parse_error_reply(const char *buf, size_t buf_size, const char **err_msg, size_t *err_msg_len);

#ifdef __cplusplus
}
#endif

#endif /* _ERROR_PARSER_H_ */

