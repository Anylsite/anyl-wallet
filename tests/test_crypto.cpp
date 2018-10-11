/**
* @brief 
* @file test_crpyto.cpp
* @author J.H. 
* @date 2018-10-10
*/

/* module header */

/* system includes C */
#include <stdint.h>

/* system includes C++ */
#include <gtest/gtest.h>


/* local includes */
#include <helpers/hextobin.h>
#include <eth/address.h>


TEST(TEST_CRYPTO, TEST_PRIVKEY)
{
	uint8_t priv[32];
    uint8_t addr[20];
    uint8_t _expected_addr[20];
    hextobin("7e5f4552091a69125d5dfcb7b8c2659029395bdf", _expected_addr, sizeof(_expected_addr));
    hextobin("0000000000000000000000000000000000000000000000000000000000000001", priv, sizeof(priv));

    privkey_to_ethereum_address(priv, addr);
    ASSERT_EQ(memcmp(_expected_addr, addr, 20), 0);
}
