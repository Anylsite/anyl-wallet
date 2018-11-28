/**
* @brief 
* @file error_parser.c
* @author J.H. 
* @date 2018-11-28
*/

/* system includes */

/* local includes */
#include "error_parser.h"
#include "helpers/jsmn.h"
#include "helpers/jsmn_utils.h"

int st_parse_error_reply(const char *buf, size_t buf_size, const char **err_msg, size_t *err_msg_len)
{
	jsmntok_t t[4];
    jsmn_parser p;
	jsmn_init(&p);
	int r = jsmn_parse(&p, buf, buf_size, t, sizeof(t)/sizeof(t[0]));
    if(r < 0) { return -1; }

    // we assume first token is always an object
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		return -1;
	}
    // second item should be "error" key
    if (jsoneq((const char*)buf, &t[1], "error") != 0) {
        return -1;
    }
    if(t[2].type != JSMN_STRING) {
        return -1;
    }
    *err_msg = buf + t[2].start;
    *err_msg_len = t[2].end - t[2].start;
    return 0;
}
