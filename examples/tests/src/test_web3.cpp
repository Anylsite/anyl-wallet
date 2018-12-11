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
#include "eth/web3_jsonp.h"
#include "helpers/hextobin.h"


const static size_t WEB3_BUFSIZE = 2048;
static uint8_t buf[WEB3_BUFSIZE];

TEST(TEST_WEB3, eth_getTransactionCount)
{
    web3_ctx_t web3;
    web3_init(&web3, buf, WEB3_BUFSIZE);
    address_t addr;
    const char *_expected = "{\"jsonrpc\":\"2.0\",\"id\":0,\"method\":\"eth_getTransactionCount\",\"params\":[\"0x7e5f4552091a69125d5dfcb7b8c2659029395bdf\",\"latest\"]}";
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

TEST(TEST_WEB3, eth_call)
{
    web3_ctx_t web3;
    web3_init(&web3, buf, WEB3_BUFSIZE);
    char buf[] = "{\"jsonrpc\":\"2.0\",\"id\":0,\"method\":\"eth_call\",\"params\":[{\"to\":\"0x7e5f4552091a69125d5dfcb7b8c2659029395bdf\",\"from\":\"0x2e83b5ae698e1f1ab5b6f4bb0732d72f0c74d049\",\"gas\":\"0x5208\",\"gasPrice\":\"0xf4240\",\"value\":\"0x499602d2\",\"data\":\"0x000102030405\"},\"latest\"]}";

    transaction_t tx;
    memset(&tx, 0, sizeof(tx));

    tx.gas_price = 1000000;
    tx.gas_limit = 21000;
    
    set256_uint64(&tx.value, 1234567890);
    uint8_t data[] = { 0, 1, 2, 3, 4, 5 };
    tx.data = data;
    tx.data_len = sizeof(data);
    address_t from;
    hextobin("7e5f4552091a69125d5dfcb7b8c2659029395bdf", tx.to.a, sizeof(tx.to.a));
    hextobin("2e83b5Ae698E1f1ab5b6F4bb0732D72F0c74D049", from.a, sizeof(from.a));

    eth_call(&web3, &from, &tx, 0);
    ASSERT_EQ(strcmp(buf, (char*)web3.buf), 0);
}

TEST(TEST_WEB3, test_decode_hexencoded)
{
    jsonrpc_result_t res;
    uint256_t out;
    char buf[] = "{\"jsonrpc\":\"2.0\",\"result\":\"0xd004a304575e176b227d40caa7aca0cfd8a60daeea88da14cb767bb87483f8ef\",\"id\":42,\"error\":{\"code\":-32015,\"message\":\"Transaction execution error.\",\"data\":\"Internal(\"Requires higher than upper limit of 47000000\")\"}}";
    uint256_t expected_res;
    fromstring256("d004a304575e176b227d40caa7aca0cfd8a60daeea88da14cb767bb87483f8ef", &expected_res);
    memset(&res, 0, sizeof(res));

    ASSERT_EQ(jsonrpc_decode_hexencoded((uint8_t*)buf, strlen(buf), &res, &out), 0);
    ASSERT_EQ(res.id, 42);
    ASSERT_EQ(equal256(&out, &expected_res), true);
    ASSERT_EQ(res.error.code, -32015);
    ASSERT_EQ(strcmp(res.error.message, "Transaction execution error."), 0);
}

TEST(TEST_WEB3, test_decode_txreceipt)
{
    jsonrpc_result_t res;
    tx_receipt_t receipt;
    char buf[] = "{\"jsonrpc\":\"2.0\",\"result\":{\"blockHash\":\"0x69164698dd4a04b07a217604abfc97022dec8f02825e839d1146744cad3abe29\",\"blockNumber\":\"0x8d1447\",\"contractAddress\":null,\"cumulativeGasUsed\":\"0x5958\",\"from\":null,\"gasUsed\":null,\"logs\":[],\"logsBloom\":\"0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"root\":null,\"status\":\"0x1\",\"to\":null,\"transactionHash\":\"0xd004a304575e176b227d40caa7aca0cfd8a60daeea88da14cb767bb87483f8ef\",\"transactionIndex\":\"0x0\"},\"id\":42}";

    ASSERT_EQ(jsonrpc_decode_txreceipt((uint8_t*)buf, strlen(buf), &res, &receipt), 0);
    ASSERT_EQ(res.id, 42);
    ASSERT_EQ(receipt.status, 1);
    ASSERT_EQ(receipt.blockNumber, 0x8d1447);
}

void helper_eth_convert(uint64_t value, const char *compare, enum ETH_UNIT from, enum ETH_UNIT to)
{
    int rc;
    uint256_t amount;
    const size_t buf_len = 50;
    char buf[buf_len] = {0};
    set256_uint64(&amount, value);
    rc = eth_convert(&amount, from, to, buf, buf_len);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(strcmp(buf, compare), 0);
    ASSERT_EQ(strlen(buf), strlen(compare));
}

TEST(TEST_ETH_CONVERT, test_eth_convert)
{
    int rc;
    uint256_t amount;
    const size_t buf_len = 50;
    char buf[buf_len] = {0};

    set256_uint64(&amount, 1234);

    rc = eth_convert(&amount, ETH_UNIT_ETHER, ETH_UNIT_FINNEY, buf, 3);
    ASSERT_EQ(rc, -1);

    rc = eth_convert(&amount, ETH_UNIT_ETHER, ETH_UNIT_ETHER, buf, 4);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(strcmp(buf, "1234"), 0);

    rc = eth_convert(&amount, ETH_UNIT_ETHER, ETH_UNIT_FINNEY, buf, 6);
    ASSERT_EQ(rc, -1);

    rc = eth_convert(&amount, ETH_UNIT_ETHER, ETH_UNIT_FINNEY, buf, 7);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(strcmp(buf, "1234000"), 0);

    rc = eth_convert(&amount, ETH_UNIT_ETHER, ETH_UNIT_KETHER, buf, 4);
    ASSERT_EQ(rc, -1);

    rc = eth_convert(&amount, ETH_UNIT_ETHER, ETH_UNIT_KETHER, buf, 5);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(strcmp(buf, "1.234"), 0);

    rc = eth_convert(&amount, ETH_UNIT_ETHER, ETH_UNIT_METHER, buf, 7);
    ASSERT_EQ(rc, -1);

    rc = eth_convert(&amount, ETH_UNIT_ETHER, ETH_UNIT_METHER, buf, 8);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(strcmp(buf, "0.001234"), 0);

    helper_eth_convert(1,"1000000000000000000", ETH_UNIT_ETHER, ETH_UNIT_WEI);
    helper_eth_convert(1,"1000000000000000", ETH_UNIT_ETHER, ETH_UNIT_KWEI);
    helper_eth_convert(1,"1000000000000", ETH_UNIT_ETHER, ETH_UNIT_MWEI);
    helper_eth_convert(1,"1000000000", ETH_UNIT_ETHER, ETH_UNIT_GWEI);
    helper_eth_convert(1,"1000000", ETH_UNIT_ETHER, ETH_UNIT_SZABO);
    helper_eth_convert(1,"1000", ETH_UNIT_ETHER, ETH_UNIT_FINNEY);
    helper_eth_convert(1,"1", ETH_UNIT_ETHER, ETH_UNIT_ETHER);
    helper_eth_convert(1,"0.001", ETH_UNIT_ETHER, ETH_UNIT_KETHER);
    helper_eth_convert(1,"0.000001", ETH_UNIT_ETHER, ETH_UNIT_METHER);
    helper_eth_convert(1,"0.000000001", ETH_UNIT_ETHER, ETH_UNIT_GETHER);
    helper_eth_convert(1,"0.000000000001", ETH_UNIT_ETHER, ETH_UNIT_TETHER);

    set256_uint64(&amount, 0);
    rc = eth_convert(&amount, ETH_UNIT_ETHER, ETH_UNIT_WEI, buf, buf_len);
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(strcmp(buf, "0"), 0);

    helper_eth_convert(123,"0.123", ETH_UNIT_ETHER, ETH_UNIT_KETHER);
    helper_eth_convert(1234,"1.234", ETH_UNIT_ETHER, ETH_UNIT_KETHER);
    helper_eth_convert(12345,"12.345", ETH_UNIT_ETHER, ETH_UNIT_KETHER);
    helper_eth_convert(123456,"123.456", ETH_UNIT_ETHER, ETH_UNIT_KETHER);
}
