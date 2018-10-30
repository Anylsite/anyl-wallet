/**
* @brief 
* @file test_web3.cpp
* @author J.H. 
* @date 2018-10-19
*/

/* module header */

/* system includes C */

/* system includes C++ */
#include <gtest/gtest.h>


/* local includes */
#include "eth/web3.h"
#include "helpers/hextobin.h"


const static size_t WEB3_BUFSIZE = 1024;
static uint8_t buf[WEB3_BUFSIZE];

TEST(TEST_WEB3, eth_getTransactionCount)
{
    web3_ctx_t web3;
    web3_init(&web3, buf, WEB3_BUFSIZE);
    address_t addr;
    const char *_expected = "{\"jsonrpc\":\"2.0\",\"id\":0,\"method\":\"eth_getTransactionCount\",\"params\":[\"0x7E5F4552091A69125D5DFCB7B8C2659029395BDF\",\"latest\"]}";
    hextobin("7e5f4552091a69125d5dfcb7b8c2659029395bdf", addr.a, sizeof(addr.a));

    ASSERT_EQ(eth_getTransactionCount(&web3, &addr), 0);
    ASSERT_EQ(strcmp(_expected, (const char*)web3.buf), 0);

    // test insufficient bufsize
    web3.buf_size = 20;
    ASSERT_EQ(eth_getTransactionCount(&web3, &addr), -1);


}


TEST(TEST_WEB3, eth_sendRawTransaction)
{
    web3_ctx_t web3;
    web3_init(&web3, buf, WEB3_BUFSIZE);
    uint8_t data[] = { 0, 1, 2, 3, 4, 5 };
    const char *_expected = "{\"jsonrpc\":\"2.0\",\"id\":0,\"method\":\"eth_sendRawTransaction\",\"params\":[\"0x000102030405\"]}";

    ASSERT_EQ(eth_sendRawTransaction(&web3, data, sizeof(data)), 0);
    ASSERT_EQ(strcmp(_expected, (const char*)web3.buf), 0);

}
