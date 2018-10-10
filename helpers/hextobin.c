/**
* @brief 
* @file hextobin.c
* @author J.H. 
* @date 2018-09-22
*/

/* system includes */
#include <assert.h>
#include <string.h>

/* local includes */
#include "hextobin.h"

static int16_t map_hex(char c)
{
    if((c >= '0') && (c <= '9')) { return c - '0'; }
    if((c >= 'A') && (c <= 'F')) { return (c - 'A') + 10; }
    if((c >= 'a') && (c <= 'f')) { return (c - 'a') + 10; }
    return -1;
}

int hextobin(const char * str, uint8_t * bytes, size_t blen)
{
   uint8_t  pos;
   int16_t  b0;
   int16_t  b1;
   assert(strlen(str) <= (blen * 2));
   if(strlen(str) == 0) { return E_EMPTY_STRING; }
   if((strlen(str) % 2) != 0) { return E_INVALID_STRING_LEN; }

   memset(bytes, 0, blen);
   for (pos = 0; ((pos < (blen*2)) && (pos < strlen(str))); pos += 2)
   {
      b0 = map_hex(str[pos]);
      assert((b0 >= -1) && (b0 <= 0xf));
      if(b0 < 0)  { return E_INVALID_CHAR; }
      b1 = map_hex(str[pos+1]);
      assert((b1 >= -1) && (b1 <= 0xf));
      if(b1 < 0)  { return E_INVALID_CHAR; }
      bytes[pos/2] = (uint8_t)(b0 << 4) | b1;
   };
   return strlen(str) / 2;
}
