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
   if(strlen(str) == 0) { return E_EMPTY_STRING; }
   if((strlen(str) % 2) != 0) { return E_INVALID_STRING_LEN; }
   if((strlen(str) > 2) && (str[0] == '0') && (str[1] == 'x')) {
       str += 2;
    }
   assert(strlen(str) <= (blen * 2));

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

int hextobin_2(const char * str, uint8_t * bytes, size_t blen)
{
    uint8_t  pos = 0;
    int16_t  b1 = -1;
    if(strlen(str) == 0) { return E_EMPTY_STRING; }
    if((strlen(str) > 2) && (str[0] == '0') && (str[1] == 'x')) {
        str += 2;
    }
    assert(strlen(str) <= (blen * 2));
    memset(bytes, 0, blen);
    // sanitize odd-length string
    if((strlen(str) % 2) != 0) {
       b1 = map_hex(str[pos]);
       assert((b1 >= -1) && (b1 <= 0xf));
       if(b1 < 0)  { return E_INVALID_CHAR; }
       bytes[0] = (uint8_t)b1;
       str += 1;
       bytes += 1;
       blen -= 1;
    }
    if(strlen(str) == 0) { return 0; }
    return hextobin(str, bytes, blen);
}

int bintohex_nonull(const uint8_t * bytes, size_t blen, char *out, size_t out_len)
{
    if (bytes == NULL || blen == 0) { return E_EMPTY_STRING; }
    if(out_len < (2*blen)) { return E_INVALID_STRING_LEN; }

    for(size_t i=0; i<blen; i++) {
        out[i*2]   = "0123456789abcdef"[bytes[i] >> 4];
        out[i*2+1] = "0123456789abcdef"[bytes[i] & 0x0F];
    }

    return (2*blen);
}

int bintohex(const uint8_t * bytes, size_t blen, char *out, size_t out_len)
{
    if (bytes == NULL || blen == 0) { return E_EMPTY_STRING; }
    if(out_len < ((2*blen)+1)) { return E_INVALID_STRING_LEN; }
    int ret = bintohex_nonull(bytes, blen, out, out_len);
    if(ret < 0) { return ret; }
    out[blen*2] = '\0';
    return ret+1;
}
