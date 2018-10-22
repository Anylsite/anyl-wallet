/**
* @brief
* @file test_uint256.cpp
* @author C.M.
* @date 2018-10-21
*/

/* module header */

/* system includes C */

/* system includes C++ */
#include <gtest/gtest.h>


/* local includes */
#include "helpers/uint256.h"

// TODO: Check if the logic implemented in writeu128BE is correct
TEST(TEST_UINT256, WRITEU128BE)
{
	uint128_t number;
	uint8_t buffer;

	UPPER(number) = 0x0;
	LOWER(number) = 0x123456789abcde;
	writeu128BE(&number, &buffer);
	ASSERT_EQ(UPPER(number), 0xdebc9a78563412);
	ASSERT_EQ(LOWER(number), LOWER(number));
	ASSERT_EQ(buffer, 0);

	UPPER(number) = 0x0;
	LOWER(number) = 0x123456789abcdef;
	writeu128BE(&number, &buffer);
	ASSERT_EQ(UPPER(number), 0xefcdab89674523);
	ASSERT_EQ(LOWER(number), LOWER(number));
	ASSERT_EQ(buffer, 1);

	UPPER(number) = 0x123456789abcde;
	LOWER(number) = 0x0;
	writeu128BE(&number, &buffer);
	ASSERT_EQ(UPPER(number), 0x0);
	ASSERT_EQ(LOWER(number), LOWER(number));
	ASSERT_EQ(buffer, 0);
}

TEST(TEST_UINT256, SET256_UINT64)
{
	uint256_t n256;
	uint64_t n64 = 123456;
	UPPER(LOWER(n256)) = 1;
	UPPER(UPPER(n256)) = 2;
	LOWER(LOWER(n256)) = 3;
	LOWER(UPPER(n256)) = 4;
	set256_uint64(&n256, n64);
	ASSERT_EQ(UPPER(UPPER(n256)), 0);
	ASSERT_EQ(UPPER(LOWER(n256)), 0);
	ASSERT_EQ(LOWER(UPPER(n256)), 0);
	ASSERT_EQ(LOWER(LOWER(n256)), n64);
}

TEST(TEST_UINT256, ZERO128)
{
	uint128_t n1, n2;
	UPPER(n1) = 123;
	LOWER(n1) = 456;
	UPPER(n2) = 0;
	LOWER(n2) = 0;
	ASSERT_TRUE(!zero128(&n1));
	ASSERT_TRUE(zero128(&n2));
}

TEST(TEST_UINT256, ZERO256)
{
	uint256_t n1, n2;
	LOWER(UPPER(n1)) = 123;
	LOWER(LOWER(n1)) = 456;
	UPPER(UPPER(n2)) = 789;
	UPPER(LOWER(n2)) = 123;

	LOWER(UPPER(n2)) = 0;
	LOWER(LOWER(n2)) = 0;
	UPPER(UPPER(n2)) = 0;
	UPPER(LOWER(n2)) = 0;

	ASSERT_TRUE(!zero256(&n1));
	ASSERT_TRUE(zero256(&n2));
}

TEST(TEST_UINT256, COPY128)
{
	uint128_t number, result;
	UPPER(number) = 123;
	LOWER(number) = 456;
	copy128(&result, &number);
	ASSERT_EQ(UPPER(number), UPPER(result));
	ASSERT_EQ(LOWER(number), LOWER(result));
}

TEST(TEST_UINT256, COPY256)
{
	uint256_t number, result;
	UPPER(LOWER(number)) = 1;
	UPPER(UPPER(number)) = 2;
	LOWER(LOWER(number)) = 3;
	LOWER(UPPER(number)) = 4;
	copy256(&result, &number);
	ASSERT_EQ(UPPER(UPPER(number)), UPPER(UPPER(result)));
	ASSERT_EQ(LOWER(UPPER(number)), LOWER(UPPER(result)));
	ASSERT_EQ(UPPER(LOWER(number)), UPPER(LOWER(result)));
	ASSERT_EQ(LOWER(LOWER(number)), LOWER(LOWER(result)));
}

TEST(TEST_UINT256, CLEAR128)
{
	uint128_t number;
	UPPER(number) = 0;
	LOWER(number) = 1;
	clear128(&number);
	ASSERT_EQ(LOWER(number), 0);
}

TEST(TEST_UINT256, CLEAR256)
{
	uint256_t number;
	UPPER(LOWER(number)) = 0;
	LOWER(LOWER(number)) = 1;
	clear256(&number);
	ASSERT_EQ(LOWER(LOWER(number)), 0);
}

TEST(TEST_UINT256, SHIFTL128)
{
	uint128_t number, reply;
    UPPER(reply) = 0x0;
	LOWER(reply) = 0x0;
	UPPER(number) = 0x1;
	LOWER(number) = 0x8;
	shiftl128(&number, 0, &reply);
	ASSERT_EQ(UPPER(reply), UPPER(number));
	ASSERT_EQ(LOWER(reply), LOWER(number));
	shiftl128(&number, 1, &reply);
	ASSERT_EQ(UPPER(reply), 0x2);
	ASSERT_EQ(LOWER(reply), 0x10);
	shiftl128(&number, 64, &reply);
	ASSERT_EQ(UPPER(reply), LOWER(number));
	ASSERT_EQ(LOWER(reply), 0x0);
	shiftl128(&number, 65, &reply);
	ASSERT_EQ(UPPER(reply), 0x10);
	ASSERT_EQ(LOWER(reply), 0x0);
	shiftl128(&number, 128, &reply);
	ASSERT_EQ(UPPER(reply), 0x0);
	ASSERT_EQ(LOWER(reply), 0x0);
}

TEST(TEST_UINT256, SHIFTR128)
{
	uint128_t number, reply;
    UPPER(reply) = 0x0;
	LOWER(reply) = 0x0;
	UPPER(number) = 0x2;
	LOWER(number) = 0x10;
	shiftr128(&number, 0, &reply);
	ASSERT_EQ(UPPER(reply), UPPER(number));
	ASSERT_EQ(LOWER(reply), LOWER(number));
	shiftr128(&number, 1, &reply);
	ASSERT_EQ(UPPER(reply), 0x1);
	ASSERT_EQ(LOWER(reply), 0x8);
	shiftr128(&number, 64, &reply);
	ASSERT_EQ(UPPER(reply), 0x0);
	ASSERT_EQ(LOWER(reply), UPPER(number));
	shiftr128(&number, 65, &reply);
	ASSERT_EQ(UPPER(reply), 0x0);
	ASSERT_EQ(LOWER(reply), 0x1);
	shiftr128(&number, 128, &reply);
	ASSERT_EQ(UPPER(reply), 0x0);
	ASSERT_EQ(LOWER(reply), 0x0);
}

TEST(TEST_UINT256, SHIFTL256)
{
	uint256_t number, reply;
	UPPER(UPPER(reply)) = 0x0;
	LOWER(UPPER(reply)) = 0x0;
	UPPER(LOWER(reply)) = 0x0;
	LOWER(LOWER(reply)) = 0x0;
	UPPER(UPPER(number)) = 0x0;
	LOWER(UPPER(number)) = 0x1;
	UPPER(LOWER(number)) = 0x0;
	LOWER(LOWER(number)) = 0x8;
	shiftl256(&number, 0, &reply);
	ASSERT_EQ(UPPER(UPPER(reply)), UPPER(UPPER(number)));
	ASSERT_EQ(LOWER(UPPER(reply)), LOWER(UPPER(number)));
	ASSERT_EQ(UPPER(LOWER(reply)), UPPER(LOWER(number)));
	ASSERT_EQ(LOWER(LOWER(reply)), LOWER(LOWER(number)));
	shiftl256(&number, 1, &reply);
	ASSERT_EQ(UPPER(UPPER(reply)), UPPER(UPPER(number)));
	ASSERT_EQ(LOWER(UPPER(reply)), 0x2);
	ASSERT_EQ(UPPER(LOWER(reply)), UPPER(LOWER(number)));
	ASSERT_EQ(LOWER(LOWER(reply)), 0x10);
	shiftl256(&number, 64, &reply);
	ASSERT_EQ(UPPER(UPPER(reply)), LOWER(UPPER(number)));
	ASSERT_EQ(LOWER(UPPER(reply)), 0x0);
	ASSERT_EQ(UPPER(LOWER(reply)), LOWER(LOWER(number)));
	ASSERT_EQ(LOWER(LOWER(reply)), 0x0);
	shiftl256(&number, 65, &reply);
	ASSERT_EQ(UPPER(UPPER(reply)), 0x2);
	ASSERT_EQ(LOWER(UPPER(reply)), 0x0);
	ASSERT_EQ(UPPER(LOWER(reply)), 0x10);
	ASSERT_EQ(LOWER(LOWER(reply)), 0x0);
}

TEST(TEST_UINT256, SHIFTR256)
{
	uint256_t number, reply;
	UPPER(UPPER(reply)) = 0x0;
	LOWER(UPPER(reply)) = 0x0;
	UPPER(LOWER(reply)) = 0x0;
	LOWER(LOWER(reply)) = 0x0;
	UPPER(UPPER(number)) = 0x2;
	LOWER(UPPER(number)) = 0x0;
	UPPER(LOWER(number)) = 0x10;
	LOWER(LOWER(number)) = 0x0;
	shiftr256(&number, 0, &reply);
	ASSERT_EQ(UPPER(UPPER(reply)), UPPER(UPPER(number)));
	ASSERT_EQ(LOWER(UPPER(reply)), LOWER(UPPER(number)));
	ASSERT_EQ(UPPER(LOWER(reply)), UPPER(LOWER(number)));
	ASSERT_EQ(LOWER(LOWER(reply)), LOWER(LOWER(number)));
	shiftr256(&number, 1, &reply);
	ASSERT_EQ(UPPER(UPPER(reply)), 0x1);
	ASSERT_EQ(LOWER(UPPER(reply)), LOWER(UPPER(number)));
	ASSERT_EQ(UPPER(LOWER(reply)), 0x8);
	ASSERT_EQ(LOWER(LOWER(reply)), LOWER(LOWER(number)));
	shiftr256(&number, 64, &reply);
	ASSERT_EQ(UPPER(UPPER(reply)), 0x0);
	ASSERT_EQ(LOWER(UPPER(reply)), UPPER(UPPER(number)));
	ASSERT_EQ(UPPER(LOWER(reply)), 0x0);
	ASSERT_EQ(LOWER(LOWER(reply)), UPPER(LOWER(number)));
	shiftr256(&number, 65, &reply);
	ASSERT_EQ(UPPER(UPPER(reply)), 0x0);
	ASSERT_EQ(LOWER(UPPER(reply)), 0x1);
	ASSERT_EQ(UPPER(LOWER(reply)), 0x0);
	ASSERT_EQ(LOWER(LOWER(reply)), 0x8);
}

