/**
* @brief 
* @file test_tx.cpp
* @author J.H. 
* @date 2018-10-11
*/

/* module header */

/* system includes C */

/* system includes C++ */
#include <gtest/gtest.h>


/* local includes */
#include "eth/transaction.h"
#include "eth/sign.h"
#include "helpers/hextobin.h"

TEST(TEST_TX, TEST_TX_SET)
{
    transaction_t tx;

    ASSERT_EQ(tx_set_to(&tx, "7e5f4552091a69125d5dfcb7b8c2659029395bdf"), 0);
    ASSERT_EQ(tx_set_to(&tx, "7e5f455X091a69125d5dfcb7b8c2659029395bdf"), -1);
    ASSERT_LT(tx_set_to(&tx, "01"), 0);
    ASSERT_LT(tx_set_to(&tx, "01xx"), 0);
    ASSERT_LT(tx_set_to(&tx, "0123"), 0);
}

#if 0
TEST(TEST_TX, TEST_TX_ENCODE)
{
    transaction_t tx;
    const uint32_t ENCODED_TX_BUFSIZE = 100;
    uint8_t buf[ENCODED_TX_BUFSIZE] = {0};

    tx.nonce = 1;
    tx.gas_price = 1;
    tx.gas_limit = 1;
    tx_set_to(&tx, "7e5f4552091a69125d5dfcb7b8c2659029395bdf");
    tx_set_value_u64(&tx, 1);
    tx.data = NULL;
    tx.data_len = 0;

    uint32_t data_len = tx_encode(&tx, NULL, buf, ENCODED_TX_BUFSIZE);

    for (int i = 0; i < data_len; i++) {
        printf("%02x", buf[i]);
    }
    printf("\n");

}
TEST(TEST_TX, TEST_TX_ENCODE_AND_SIGN)
{
    transaction_t tx;
    const uint32_t ENCODED_TX_BUFSIZE = 100;
    uint8_t buf[ENCODED_TX_BUFSIZE] = {0};
	uint8_t priv[32];
    hextobin("0000000000000000000000000000000000000000000000000000000000000001", priv, sizeof(priv));

    tx.nonce = 1998;
    tx.gas_price = 1 * 1000000000;
    tx.gas_limit = 21612;
//    tx_set_to(&tx, "7e5f4552091a69125d5dfcb7b8c2659029395bdf");
    tx_set_to(&tx, "2e83b5ae698e1f1ab5b6f4bb0732d72f0c74d049");
    tx_set_value_u64(&tx, 0);
    tx.data = (uint8_t*)"anyledger";
    tx.data_len = strlen((const char*)tx.data);

    uint32_t data_len = tx_encode(&tx, NULL, buf, ENCODED_TX_BUFSIZE);

    for (size_t i = 0; i < data_len; i++) {
        printf("%02x", buf[i]);
    }
    printf("\n");

    signature_t tx_sig;

    eth_sign_data(priv, (uint8_t*)buf, data_len, &tx_sig);
    for (size_t i = 0; i < (sizeof(uint256_t)*2); i++) {
        printf("%02x", ((uint8_t*)&tx_sig)[i]);
    }
    printf("%02x", tx_sig.v);
    printf("\n");

    data_len = tx_encode(&tx, &tx_sig, buf, ENCODED_TX_BUFSIZE);
    for (size_t i = 0; i < data_len; i++) {
        printf("%02x", buf[i]);
    }
    printf("\n");

}
#endif
