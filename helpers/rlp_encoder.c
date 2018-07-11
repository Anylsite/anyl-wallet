/*
* AnyLedger - Created by Bogdan Djukic on 11-07-2018. 
* Inspired by Okada Takahiro, https://github.com/kopanitsa/web3-arduino.
*/

#include "rlp_encoder.h"

uint32_t encode_item(uint8_t* output, const uint8_t* input, uint32_t input_len) {
    if (input_len == 1 && input[0] == 0x00) {
        uint8_t c[1] = {0x80};
        memcpy(output, c, 1);
        
        return 1;
    } else if (input_len==1 && input[0] < 128) {
        memcpy(output, input, 1);
        
        return 1;
    } else if (input_len <= 55) {
        uint8_t _ = (uint8_t)0x80 + (uint8_t)input_len;
        uint8_t header[] = {_};
        
        memcpy(output, header, 1);
        memcpy(output + 1, input, input_len);
        
        return input_len+1;
    } else {
        uint8_t tmp_header[8];
        memset(tmp_header, 0, 8);
        uint32_t hexdigit = 1;
        uint32_t tmp = input_len;
        while ((uint32_t)(tmp / 256) > 0) {
            tmp_header[hexdigit] = (uint8_t)(tmp % 256);
            tmp = (uint32_t)(tmp / 256);
            hexdigit++;
        }
        
        tmp_header[hexdigit] = (uint8_t)(tmp);
        tmp_header[0] = (uint8_t)0xb7 + (uint8_t)hexdigit;

        uint8_t header[8];
        memset(header, 0, 8);
        header[0] = tmp_header[0];
        
        for (int i = 0; i < hexdigit; i++) {
            header[i + 1] = tmp_header[hexdigit - i];
        }

        memcpy(output, header, hexdigit + 1);
        memcpy(output + hexdigit + 1, input, input_len);

        return input_len + hexdigit + 1;
    }
}

uint32_t encode_whole_header(uint8_t *header_output, uint32_t total_len) {
    if (total_len < 55) {
        header_output[0] = (uint8_t)0xc0 + (uint8_t)total_len;
        return 1;
    } else {
        uint8_t tmp_header[8];
        memset(tmp_header, 0, 8);
        uint32_t hexdigit = 1;
        uint32_t tmp = total_len;
        
        while ((uint32_t)(tmp / 256) > 0) {
            tmp_header[hexdigit] = (uint8_t)(tmp % 256);
            tmp = (uint32_t)(tmp / 256);
            hexdigit++;
        }
        
        tmp_header[hexdigit] = (uint8_t)(tmp);
        tmp_header[0] = (uint8_t)0xf7 + (uint8_t)hexdigit;

        uint8_t header[8];
        memset(header, 0, 8);
        header[0] = tmp_header[0];
        
        for (int i = 0; i < hexdigit; i++) {
            header[i + 1] = tmp_header[hexdigit - i];
        }

        memcpy(header_output, header, hexdigit + 1);

        return hexdigit + 1;
    }
}