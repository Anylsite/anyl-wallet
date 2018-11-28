/**
* @brief 
* @file jsmn_utils.c
* @author J.H. 
* @date 2018-11-28
*/

/* system includes */
#include <string.h>

/* local includes */
#include "jsmn_utils.h"

int jsoneq(const char *json, const jsmntok_t *tok, const char *s)
{
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
