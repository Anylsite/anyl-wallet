/**
* @brief 
* @file test_rlp.cpp
* @author J.H. 
* @date 2018-08-31
*/

/* module header */

/* system includes C */

/* system includes C++ */
#include <list>
#include "gtest/gtest.h"


/* local includes */
#include "helpers/rlp_encoder.h"

#define LONG_STRING_64 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
#define LONG_STRING_256 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

static bool test_encode(
    const uint8_t *input,
    uint32_t input_len,
    const uint8_t *output,
    uint32_t output_len
) 
{
#define OUT_BUF_LEN 1024
    uint8_t output_buf[OUT_BUF_LEN] = {0};
    uint32_t encoded_len = 0;
    
    assert(output_len <= OUT_BUF_LEN);

    encoded_len = rlp_encode_item(output_buf, input, input_len);
    assert(encoded_len > 0);
    assert(encoded_len == output_len);
    return memcmp(output_buf, output, output_len) == 0;
}

typedef struct _testpair {
    const char *input;
    const char *output;
    size_t in_len;
    size_t out_len;
    _testpair(
        const char *i, const char *o,
        size_t in_len=0, size_t out_len=0
    ): input(i), output(o), in_len(in_len), out_len(out_len) {}
} testpair_t;

static const testpair_t test_data_ok[] = {
    {
        LONG_STRING_256, "\xb9\x01\x00" LONG_STRING_256,
        strlen(LONG_STRING_256), strlen(LONG_STRING_256)+3
    },                                      // long string
    {
        LONG_STRING_64, "\xb8\x40" LONG_STRING_64,
        strlen(LONG_STRING_64), strlen(LONG_STRING_64)+2
    },                                      // really long string
    {"dog", "\x83""dog"},                   // a string
    {"hello world", "\x8b""hello world"},   // another string
    {"", "\x80", 0, 1},                     // empty string
    {"\x00", "\x80", 1, 1},                 // integer 0
    {"\x04\x00", "\x82\x04\x00", 2, 3},     // integer 0x4000
    {"\xf", "\xf"},                         // integer 15
    {"\x7f", "\x7f"},                       // integer 0x7f
    {"\x80", "\x81\x80"},                   // integer 0x80
    {"\xff", "\x81\xff"},                   // integer 0xff
};

TEST(TEST_RLP, TEST_RLP_ITEMS) 
{
    for (auto &pair: test_data_ok) {
        size_t out_len = pair.out_len > 0 ? pair.out_len : strlen(pair.output);
        size_t in_len = pair.in_len > 0 ? pair.in_len : strlen(pair.input);
        ASSERT_TRUE(
            test_encode(
                (uint8_t*)pair.input,
                in_len,
                (uint8_t*)pair.output,
                out_len
            )
        );
    }
}

typedef struct {
    size_t input_len;
    const char *output;
    size_t output_len;
} header_testpair_t;

static const header_testpair_t test_data_headers[] = {
    {0, "\xc0", 1},
    {54, "\xf6", 1},
    {64, "\xf8\x40", 2},
    {1024, "\xf9\x04\x00", 3}
};

TEST(TEST_RLP, TEST_RLP_LISTS) 
{
    for (auto &pair: test_data_headers) {
        uint8_t header_output[HEADER_LEN];
        uint8_t output_len = rlp_encode_array_header(header_output, pair.input_len);
        ASSERT_EQ(output_len, pair.output_len);
        ASSERT_TRUE(memcmp(header_output, pair.output, pair.output_len) == 0);
    }
}





TEST(TEST_RLP, TEST_RLP_INTEGER) 
{
    const std::list<std::pair<uint64_t, const char *>> _test_data = {
        {0, "\x80"},
        {100, "\x64"},
        {128, "\x81\x80"},
        {0xff, "\x81\xff"},
        {0xffff, "\x82\xff\xff"},
        {UINT64_MAX-1, "\x88\xff\xff\xff\xff\xff\xff\xff\xff"}
    };
    uint8_t output_buf[OUT_BUF_LEN] = {0};
    uint32_t output_len;
    for(auto pair: _test_data) {
        output_len = rlp_encode_integer(output_buf, pair.first);
        ASSERT_EQ(output_len, strlen(pair.second));
    }
}
