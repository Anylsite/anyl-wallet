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
    const char* _data_to_sign = "Hello world";
    hextobin("30755ed65396facf86c53e6217c52b4daebe72aa4941d89635409de4c9c7f9466d4e9aaec7977f05e923889b33c0d0dd27d7226b6e6f56ce737465c5cfd04be400", signature, sizeof(signature));
    hextobin("11f4d0A3c12e86B4b5F39B213F7E19D048276DAe", _expected_addr, sizeof(_expected_addr));

    address_from_signature((uint8_t*)_data_to_sign, strlen(_data_to_sign), (const signature_t *)signature, &_addr);
    ASSERT_EQ(memcmp(_expected_addr, _addr, 20), 0);
}


