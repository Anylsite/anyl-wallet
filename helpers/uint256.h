/*******************************************************************************
*   Ledger Blue
*   (c) 2016 Ledger
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
********************************************************************************/

// Adapted from https://github.com/calccrypto/uint256_t

#ifndef UINT256_H_
#define UINT256_H_
#include <stdbool.h>
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define U64 uint64_t
#define ENDIAN_SWAP_U64(val) ((U64) ( \
    (((U64) (val) & (U64) 0x00000000000000ff) << 56) | \
    (((U64) (val) & (U64) 0x000000000000ff00) << 40) | \
    (((U64) (val) & (U64) 0x0000000000ff0000) << 24) | \
    (((U64) (val) & (U64) 0x00000000ff000000) <<  8) | \
    (((U64) (val) & (U64) 0x000000ff00000000) >>  8) | \
    (((U64) (val) & (U64) 0x0000ff0000000000) >> 24) | \
    (((U64) (val) & (U64) 0x00ff000000000000) >> 40) | \
    (((U64) (val) & (U64) 0xff00000000000000) >> 56)))

typedef struct uint128_t { uint64_t elements[2]; } uint128_t;

typedef struct uint256_t { uint128_t elements[2]; } uint256_t;

#define UPPER_P(x) x->elements[0]
#define LOWER_P(x) x->elements[1]
#define UPPER(x) x.elements[0]
#define LOWER(x) x.elements[1]

void readu64BE(const uint8_t *buffer, uint128_t *target);
void readu64LE(const uint8_t *buffer, uint128_t *target);
void readu128BE(const uint8_t *buffer, uint128_t *target);
void readu256BE(const uint8_t *buffer, uint256_t *target);

// write 256 or 128 bits uint to a buffer, encoded as Big endian
void writeu256BE(const uint256_t *number, uint8_t *buffer);
void writeu128BE(const uint128_t *number, uint8_t *buffer);

void set256_uint64(uint256_t *target, uint64_t val);
void set256_uint64BE(uint256_t *target, uint64_t val);

bool zero128(const uint128_t *number);
bool zero256(const uint256_t *number);
void copy128(uint128_t *target, const uint128_t *number);
void copy256(uint256_t *target, const uint256_t *number);
void clear128(uint128_t *target);
void clear256(uint256_t *target);
void shiftl128(const uint128_t *number, uint32_t value, uint128_t *target);
void shiftr128(const uint128_t *number, uint32_t value, uint128_t *target);
void shiftl256(const uint256_t *number, uint32_t value, uint256_t *target);
void shiftr256(const uint256_t *number, uint32_t value, uint256_t *target);
uint32_t bits128(const uint128_t *number);
uint32_t bits256(const uint256_t *number);
bool equal128(const uint128_t *number1, const uint128_t *number2);
bool equal256(const uint256_t *number1, const uint256_t *number2);
bool gt128(const uint128_t *number1, const uint128_t *number2);
bool gt256(const uint256_t *number1, const uint256_t *number2);
bool gte128(const uint128_t *number1, const uint128_t *number2);
bool gte256(const uint256_t *number1, const uint256_t *number2);
void add128(const uint128_t *number1, const uint128_t *number2, uint128_t *target);
void add256(const uint256_t *number1, const uint256_t *number2, uint256_t *target);
void minus128(const uint128_t *number1, const uint128_t *number2, uint128_t *target);
void minus256(const uint256_t *number1, const uint256_t *number2, uint256_t *target);
void or128(const uint128_t *number1, const uint128_t *number2, uint128_t *target);
void or256(const uint256_t *number1, const uint256_t *number2, uint256_t *target);
void mul128(const uint128_t *number1, const uint128_t *number2, uint128_t *target);
void mul256(const uint256_t *number1, const uint256_t *number2, uint256_t *target);
void divmod128(const uint128_t *l, const uint128_t *r, uint128_t *retDiv, uint128_t *retMod);
void divmod256(const uint256_t *l, const uint256_t *r, uint256_t *retDiv, uint256_t *retMod);
bool tostring128(const uint128_t *number, uint32_t baseParam, char *out,
                 uint32_t outLength);
bool tostring256(const uint256_t *number, uint32_t baseParam, char *out,
                 uint32_t outLength);
int fromstring256(const char *hexencoded, uint256_t *out);
#ifdef __cplusplus
}
#endif
#endif
