/**
* @brief 
* @file test_crpyto.cpp
* @author Mohit Agnihotri 
* @date 2018-10-27
*/

/* module header */

/* system includes C */
#include <stdint.h>

/* system includes C++ */
#include <gtest/gtest.h>

/* local includes */
#include <helpers/hextobin.h>
#include <eth/address.h>
#include <eth/sign.h>

TEST(TEST_ADDRESS, TEST_ADDRESS_RECOVERY)
{
    address_t _expected_addr;
    address_t _addr;
    uint8_t signature[65];
    const char *_data_to_sign = "A";
    signature_t sign = {{0u}, {0u}, 0u};

    hextobin("eac4a6600cb8b05876932ab90d4db031341f30649ac812021ead6fc90c604ce17c8dd6bb39a5545f75f3593b7db8f843d74ceac0bfa3696bbffeb11dda80db7b1c", signature, sizeof(signature));
    hextobin("7e5f4552091a69125d5dfcb7b8c2659029395bdf", _expected_addr, sizeof(_expected_addr));


    memcpy(&sign.r, signature, sizeof(uint256_t));
    memcpy(&sign.s, signature+sizeof(uint256_t), sizeof(uint256_t));
    sign.v = signature[64];

    int status = address_from_signature((uint8_t*)_data_to_sign, strlen(_data_to_sign), &sign, &_addr);

    ASSERT_TRUE(status == 0u);
    ASSERT_EQ(memcmp(_expected_addr, _addr, 20), 0);
}


