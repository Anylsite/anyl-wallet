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


TEST(TEST_TX, TEST_TX_SET)
{
    transaction_t tx;

    ASSERT_EQ(tx_set_to(&tx, "7e5f4552091a69125d5dfcb7b8c2659029395bdf"), 0);
    ASSERT_EQ(tx_set_to(&tx, "7e5f455X091a69125d5dfcb7b8c2659029395bdf"), -1);
    ASSERT_LT(tx_set_to(&tx, "01"), 0);
    ASSERT_LT(tx_set_to(&tx, "01xx"), 0);
    ASSERT_LT(tx_set_to(&tx, "0123"), 0);
}

TEST(TEST_TX, TEST_TX_ENCODE)
{
    transaction_t tx;
    const uint32_t ENCODED_TX_BUFSIZE = 100;
    uint8_t buf[ENCODED_TX_BUFSIZE];

    tx.nonce = 0;
    tx.gas_price = 0;
    tx.gas_limit = 0;
    tx_set_to(&tx, "7e5f4552091a69125d5dfcb7b8c2659029395bdf");
    tx_set_value_u64(&tx, 0);
    tx.data = NULL;

    tx_encode(&tx, buf, ENCODED_TX_BUFSIZE);

}
