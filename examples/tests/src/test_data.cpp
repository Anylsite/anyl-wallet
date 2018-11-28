/**
* @brief 
* @file test_data.cpp
* @author J.H. 
* @date 2018-09-19
*/

/* module header */

/* system includes C */
#include <gtest/gtest.h>

/* system includes C++ */


/* local includes */
#include "eth/data.h"



TEST(TEST_DATA, TEST_DATA_ITEMS)
{
#define NUM_ITEMS   64
#define DATA_LEN ((DATA_ITEM_LEN * NUM_ITEMS) + SELECTOR_LEN)
    uint8_t data[DATA_LEN];
    data_block_t block;
    block.data_len = DATA_LEN;
    block.data = data;

    ASSERT_EQ(data_init(&block), 0);
    for(int i = 0 ; i < NUM_ITEMS; i++) {
        encode_uint(&block, i, i);
    }
    for(int i = 0 ; i < NUM_ITEMS; i++) {
       ASSERT_EQ(decode_uint(&block, i), i);
    }

    ASSERT_EQ(data_init(&block), 0);
    char test_str[] = "0: Hello world!";
    for(int i = 0 ; i < NUM_ITEMS; i++) {
        test_str[0] = '0' + i;
        encode_data(&block, i, (uint8_t*)&test_str, strlen(test_str));
    }
    char test_buf[DATA_ITEM_LEN+1];
    for(int i = 0 ; i < NUM_ITEMS; i++) {
        test_str[0] = '0' + i;
        decode_data(&block, i, (uint8_t*)&test_buf, DATA_ITEM_LEN);
        ASSERT_EQ(strncmp(test_buf, test_str, DATA_ITEM_LEN), 0);
    }

}
