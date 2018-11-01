/**
* @brief 
* @file test_crpyto.cpp
* @author J.H. 
* @date 2018-10-10
*/

/* module header */

/* system includes C */
#include <stdint.h>
#include <string.h>

/* system includes C++ */
#include <gtest/gtest.h>


/* local includes */
#include <helpers/hextobin.h>
#include <eth/address.h>
#include <eth/sign.h>


TEST(TEST_CRYPTO, TEST_PRIVKEY)
{
	uint8_t priv[32];
    address_t addr;
    address_t _expected_addr;
    hextobin("7e5f4552091a69125d5dfcb7b8c2659029395bdf", _expected_addr.a, sizeof(_expected_addr.a));
    hextobin("0000000000000000000000000000000000000000000000000000000000000001", priv, sizeof(priv));

    privkey_to_ethereum_address(priv, &addr);
    ASSERT_EQ(memcmp(_expected_addr.a, addr.a, sizeof(addr.a)), 0);
}

TEST(TEST_CRYPTO, TEST_ETH_SIGN)
{
	uint8_t priv[32];
	uint8_t signature[65];
	uint8_t _expected_signature[65];
    hextobin("0000000000000000000000000000000000000000000000000000000000000001", priv, sizeof(priv));
    hextobin("eac4a6600cb8b05876932ab90d4db031341f30649ac812021ead6fc90c604ce17c8dd6bb39a5545f75f3593b7db8f843d74ceac0bfa3696bbffeb11dda80db7b1c", _expected_signature, sizeof(_expected_signature));
    const char *_data_to_sign = "A";

    eth_sign(priv, (uint8_t*)_data_to_sign, strlen(_data_to_sign), signature);
    ASSERT_EQ(memcmp(_expected_signature, signature, 65), 0);

}

TEST(TEST_CRYPTO, TEST_ETH_SIGN_DATA)
{
	uint8_t priv[32];
	uint8_t signature[65];
	uint8_t _expected_signature[65];
    hextobin("0000000000000000000000000000000000000000000000000000000000000001", priv, sizeof(priv));
    hextobin("d6ab5e5a10eb382ec62acf649d927e20ec68ba13bb8b307d794cd4803424231b318a520d1df986752f704ec1b1645cacd15574788f010e18693376ef3aa423351b", _expected_signature, sizeof(_expected_signature));
    const char *_data_to_sign = "A";

    eth_sign_data_rawsig(priv, (uint8_t*)_data_to_sign, strlen(_data_to_sign), signature);
    ASSERT_EQ(memcmp(_expected_signature, signature, 65), 0);

}

TEST(TEST_CRYPTO, TEST_DIGEST)
{
    uint8_t digest[32];
    const char *_msg = "A";
    uint8_t _expected_digest[32];
    hextobin("8fb4e9c5bb2495aea346395e692a7600f577f3ef591f95419dc8fee21e848cae", _expected_digest, sizeof(_expected_digest));
    eth_digest_message((uint8_t*)_msg, strlen(_msg), digest);
    ASSERT_EQ(memcmp(_expected_digest, digest, 20), 0);
}

TEST(TEST_ADDRESS, TEST_ADDRESS_RECOVERY)
{
    address_t _expected_addr;
    address_t _addr;
    uint8_t signature[65];
    const char *_data_to_sign = "A";
    signature_t sign;
    memset(&sign, 0, sizeof(sign));

    hextobin("eac4a6600cb8b05876932ab90d4db031341f30649ac812021ead6fc90c604ce17c8dd6bb39a5545f75f3593b7db8f843d74ceac0bfa3696bbffeb11dda80db7b1c", signature, sizeof(signature));
    hextobin("7e5f4552091a69125d5dfcb7b8c2659029395bdf", _expected_addr.a, sizeof(_expected_addr.a));

    memcpy(&sign.r, signature, sizeof(uint256_t));
    memcpy(&sign.s, signature+sizeof(uint256_t), sizeof(uint256_t));
    sign.v = signature[64];

    int status = address_from_signature((uint8_t*)_data_to_sign, strlen(_data_to_sign), &sign, &_addr);

    ASSERT_TRUE(status == 0u);
    ASSERT_EQ(memcmp(_expected_addr.a, _addr.a, 20), 0);
}
