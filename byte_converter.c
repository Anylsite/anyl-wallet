/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
* Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
*/

#include "byte_converter.h"

#include <string.h>
#include <stdint.h>

uint8_t number_to_bytes(uint8_t *raw_bytes, uint32_t number) {
    uint32_t ret = 0;
    uint8_t tmp[8];

    memset(tmp, 0, 8);
    
    if ((uint32_t)(number / 256) >= 0) {
        while ((uint32_t)(number / 256) > 0) {
            tmp[ret] = (uint8_t)(number % 256);
            number = (uint32_t)(number / 256);
            ret++;
        }
        
        tmp[ret] = (uint8_t)(number % 256);
        for (int i=0; i<ret + 1; i++) {
            raw_bytes[i] = tmp[ret - i];
        }
    } else {
        raw_bytes[0] = (uint8_t)number;
    }

    return ret + 1;
}

uint8_t char_to_bytes(uint8_t *raw_bytes, const char *text) {
    uint32_t ret = 0;
    uint8_t tmp[256];
    
    strcpy((char *)tmp, text);

    // remove "0x"
    char * ptr = strtok((char*)tmp, "x");
    
    if (strlen(ptr)!=1) {
        ptr = (char *)tmp;
    } else {
        ptr = strtok(NULL, "x");
    }

    size_t lenstr = strlen(ptr);
    for (int i = 0; i < lenstr; i+=2) {
        char c[3];
        c[0] = *(ptr + i);
        c[1] = *(ptr + i + 1);
        c[2] = 0x00;
        
        uint8_t val = strtol(c, NULL, 16);
        raw_bytes[ret] = val;
        
        ret++;
    }
    
    return ret;
}