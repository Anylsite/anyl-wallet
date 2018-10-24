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
	buffer = 0x0;

	UPPER(number) = 0x89ABCD;
	LOWER(number) = 0x123456;
	printf("Is this the correct logic for writeu128be?\n");
	printf("writeu128BE(&number, &buffer);\n");
	// printf("before) UPPER: %x LOWER: %x BUFFER: %x \n", UPPER(number), LOWER(number), buffer);
	writeu128BE(&number, &buffer);
	// printf("after) UPPER: %x LOWER: %x BUFFER: %x \n", UPPER(number), LOWER(number), buffer);
	// 1) 89abcd 123456 0
	// 2) 0 123456 0
	// TODO: Is this the correct logic for writeu128BE?
/*  ASSERT_EQ(UPPER(number), 0xdebc);
	ASSERT_EQ(LOWER(number), LOWER(number));
	ASSERT_EQ(buffer, 0); */

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
	ASSERT_TRUE(equal128(&number, &result));
}

TEST(TEST_UINT256, COPY256)
{
	uint256_t number, result;
	UPPER(LOWER(number)) = 1;
	UPPER(UPPER(number)) = 2;
	LOWER(LOWER(number)) = 3;
	LOWER(UPPER(number)) = 4;
	copy256(&result, &number);
	ASSERT_TRUE(equal256(&number, &result));
}

TEST(TEST_UINT256, CLEAR128)
{
	uint128_t number;
	UPPER(number) = 0;
	LOWER(number) = 1;
	clear128(&number);
	ASSERT_TRUE(zero128(&number));
}

TEST(TEST_UINT256, CLEAR256)
{
	uint256_t number;
	UPPER(LOWER(number)) = 0;
	LOWER(LOWER(number)) = 1;
	clear256(&number);
	ASSERT_TRUE(zero256(&number));
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

TEST(TEST_UINT256, BITS128)
{
	uint128_t number;
	uint32_t reply;
	UPPER(number) = 0x0;
	LOWER(number) = 0x3;
	reply = bits128(&number);
	ASSERT_EQ(reply, 2);
	UPPER(number) = 0x0;
	LOWER(number) = 0x2;
	reply = bits128(&number);
	ASSERT_EQ(reply, 2);
	UPPER(number) = 0x0;
	LOWER(number) = 0x20;
	reply = bits128(&number);
	ASSERT_EQ(reply, 6);
	UPPER(number) = 0x0;
	LOWER(number) = 0x00;
	reply = bits128(&number);
	ASSERT_EQ(reply, 0);
	UPPER(number) = 0x0;
	LOWER(number) = 0x80;
	reply = bits128(&number);
	ASSERT_EQ(reply, 8);
	UPPER(number) = 0x1;
	LOWER(number) = 0x0;
	reply = bits128(&number);
	ASSERT_EQ(reply, 65);
	UPPER(number) = 0x3;
	LOWER(number) = 0x0;
	reply = bits128(&number);
	ASSERT_EQ(reply, 66);
	UPPER(number) = 0x0;
	LOWER(number) = 0xFFFFFFFFFFFFFFFF;
	reply = bits128(&number);
	ASSERT_EQ(reply, 64);
	UPPER(number) = 0x20;
	LOWER(number) = 0x0;
	reply = bits128(&number);
	ASSERT_EQ(reply, 70);
	UPPER(number) = 0xFFFFFFFFFFFFFFFF;
	LOWER(number) = 0x0;
	reply = bits128(&number);
	ASSERT_EQ(reply, 128);
}


TEST(TEST_UINT256, BITS256)
{
	uint256_t number;
	uint32_t reply;
	UPPER(UPPER(number)) = 0x0;
	LOWER(UPPER(number)) = 0x0;
	UPPER(LOWER(number)) = 0x0;
	LOWER(LOWER(number)) = 0x3;
	reply = bits256(&number);
	ASSERT_EQ(reply, 2);
	UPPER(UPPER(number)) = 0x0;
	LOWER(UPPER(number)) = 0x0;
	UPPER(LOWER(number)) = 0x0;
	LOWER(LOWER(number)) = 0x2;
	reply = bits256(&number);
	ASSERT_EQ(reply, 2);
	UPPER(UPPER(number)) = 0x0;
	LOWER(UPPER(number)) = 0x0;
	UPPER(LOWER(number)) = 0x1;
	LOWER(LOWER(number)) = 0x0;
	reply = bits256(&number);
	ASSERT_EQ(reply, 65);
	UPPER(UPPER(number)) = 0x0;
	LOWER(UPPER(number)) = 0x0;
	UPPER(LOWER(number)) = 0x20;
	LOWER(LOWER(number)) = 0x0;
	reply = bits256(&number);
	ASSERT_EQ(reply, 70);
	UPPER(UPPER(number)) = 0x0;
	LOWER(UPPER(number)) = 0x1;
	UPPER(LOWER(number)) = 0x0;
	LOWER(LOWER(number)) = 0x0;
	reply = bits256(&number);
	ASSERT_EQ(reply, 129);
	UPPER(UPPER(number)) = 0x0;
	LOWER(UPPER(number)) = 0x10;
	UPPER(LOWER(number)) = 0x0;
	LOWER(LOWER(number)) = 0x0;
	reply = bits256(&number);
	ASSERT_EQ(reply, 133);
	UPPER(UPPER(number)) = 0x0;
	LOWER(UPPER(number)) = 0x0;
	UPPER(LOWER(number)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(LOWER(number)) = 0x0;
	reply = bits256(&number);
	ASSERT_EQ(reply, 128);
	UPPER(UPPER(number)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(UPPER(number)) = 0x0;
	UPPER(LOWER(number)) = 0x0;
	LOWER(LOWER(number)) = 0x0;
	reply = bits256(&number);
	ASSERT_EQ(reply, 256);
}

TEST(TEST_UINT256, EQUAL128)
{
	uint128_t n1, n2;
	UPPER(n1) = 1;
	LOWER(n1) = 2;
	UPPER(n2) = 1;
	LOWER(n2) = 2;
	ASSERT_TRUE(equal128(&n1, &n2));
	UPPER(n1) = 0;
	LOWER(n1) = 1;
	UPPER(n2) = 0;
	LOWER(n2) = 0;
	ASSERT_TRUE(!equal128(&n1, &n2));
	UPPER(n1) = 0;
	LOWER(n1) = 0;
	UPPER(n2) = 1;
	LOWER(n2) = 0;
	ASSERT_TRUE(!equal128(&n1, &n2));
}

TEST(TEST_UINT256, EQUAL256)
{
	uint256_t n1, n2;
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 2;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 1;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	ASSERT_TRUE(equal256(&n1, &n2));
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 2;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 0;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	ASSERT_TRUE(!equal256(&n1, &n2));
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 0;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 1;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	ASSERT_TRUE(!equal256(&n1, &n2));
}

TEST(TEST_UINT256, GT128)
{
	uint128_t n1, n2;
	UPPER(n1) = 1;
	LOWER(n1) = 2;
	UPPER(n2) = 1;
	LOWER(n2) = 2;
	ASSERT_TRUE(!gt128(&n1, &n2));
	UPPER(n1) = 0;
	LOWER(n1) = 1;
	UPPER(n2) = 0;
	LOWER(n2) = 0;
	ASSERT_TRUE(gt128(&n1, &n2));
	UPPER(n1) = 0;
	LOWER(n1) = 0;
	UPPER(n2) = 1;
	LOWER(n2) = 0;
	ASSERT_TRUE(!gt128(&n1, &n2));
}

TEST(TEST_UINT256, GT256)
{
	uint256_t n1, n2;
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 2;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 1;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	ASSERT_TRUE(!gt256(&n1, &n2));
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 2;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 0;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	ASSERT_TRUE(gt256(&n1, &n2));
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 0;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 1;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	ASSERT_TRUE(!gt256(&n1, &n2));
}


TEST(TEST_UINT256, GTE128)
{
	uint128_t n1, n2;
	UPPER(n1) = 1;
	LOWER(n1) = 2;
	UPPER(n2) = 1;
	LOWER(n2) = 2;
	ASSERT_TRUE(gte128(&n1, &n2));
	UPPER(n1) = 0;
	LOWER(n1) = 1;
	UPPER(n2) = 0;
	LOWER(n2) = 0;
	ASSERT_TRUE(gte128(&n1, &n2));
	UPPER(n1) = 0;
	LOWER(n1) = 0;
	UPPER(n2) = 1;
	LOWER(n2) = 0;
	ASSERT_TRUE(!gte128(&n1, &n2));
}

TEST(TEST_UINT256, GTE256)
{
	uint256_t n1, n2;
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 2;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 1;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	ASSERT_TRUE(gte256(&n1, &n2));
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 2;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 0;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	ASSERT_TRUE(gte256(&n1, &n2));
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 0;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 1;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	ASSERT_TRUE(!gte256(&n1, &n2));
}

TEST(TEST_UINT256, ADD128)
{
	uint128_t n1, n2, result;
	UPPER(n1) = 1;
	LOWER(n1) = 2;
	UPPER(n2) = 1;
	LOWER(n2) = 2;
	add128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 2);
	ASSERT_EQ(LOWER(result), 4);
	UPPER(n1) = 0;
	LOWER(n1) = 0xFFFFFFFFFFFFFFFF;
	UPPER(n2) = 0;
	LOWER(n2) = 1;
	add128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 1);
	ASSERT_EQ(LOWER(result), 0);
	UPPER(n1) = 0xFFFFFFFFFFFFFFFF;
	LOWER(n1) = 0;
	UPPER(n2) = 1;
	LOWER(n2) = 0;
	add128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 0);
	ASSERT_EQ(LOWER(result), 0);
}


TEST(TEST_UINT256, add256)
{
	uint256_t n1, n2, result;
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 2;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 1;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	add256(&n1, &n2, &result);
	ASSERT_EQ(UPPER(UPPER(result)),2);
	ASSERT_EQ(LOWER(UPPER(result)),4);
	ASSERT_EQ(UPPER(LOWER(result)),6);
	ASSERT_EQ(LOWER(LOWER(result)),8);
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 0;
	UPPER(LOWER(n1)) = 0;
	LOWER(LOWER(n1)) = 0;
	UPPER(UPPER(n2)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(UPPER(n2)) = 0;
	UPPER(LOWER(n2)) = 0;
	LOWER(LOWER(n2)) = 0;
	add256(&n1, &n2, &result);
	ASSERT_EQ(UPPER(UPPER(result)),0);
	ASSERT_EQ(LOWER(UPPER(result)),0);
	ASSERT_EQ(UPPER(LOWER(result)),0);
	ASSERT_EQ(LOWER(LOWER(result)),0);
	UPPER(UPPER(n1)) = 0;
	LOWER(UPPER(n1)) = 0xFFFFFFFFFFFFFFFF;
	UPPER(LOWER(n1)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(LOWER(n1)) = 0xFFFFFFFFFFFFFFFF;
	UPPER(UPPER(n2)) = 0;
	LOWER(UPPER(n2)) = 0;
	UPPER(LOWER(n2)) = 0;
	LOWER(LOWER(n2)) = 1;
	add256(&n1, &n2, &result);
	ASSERT_EQ(UPPER(UPPER(result)),1);
	ASSERT_EQ(LOWER(UPPER(result)),0);
	ASSERT_EQ(UPPER(LOWER(result)),0);
	ASSERT_EQ(LOWER(LOWER(result)),0);
}

TEST(TEST_UINT256, MINUS128)
{
	uint128_t n1, n2, result;
	UPPER(n1) = 1;
	LOWER(n1) = 2;
	UPPER(n2) = 1;
	LOWER(n2) = 2;
	minus128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 0);
	ASSERT_EQ(LOWER(result), 0);
	UPPER(n1) = 0x1;
	LOWER(n1) = 0;
	UPPER(n2) = 0;
	LOWER(n2) = 0xFFFFFFFFFFFFFFFF;
	minus128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 0);
	ASSERT_EQ(LOWER(result), 1);
	UPPER(n1) = 0;
	LOWER(n1) = 0;
	UPPER(n2) = 0;
	LOWER(n2) = 1;
	// TODO: same as minus256: because is unsigned int, check if this result is ok, or if the correct return is zero
	minus128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 0xFFFFFFFFFFFFFFFF);
	ASSERT_EQ(LOWER(result), 0xFFFFFFFFFFFFFFFF);
}


TEST(TEST_UINT256, MINUS256)
{
	uint256_t n1, n2, result;
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 2;
	UPPER(LOWER(n1)) = 3;
	LOWER(LOWER(n1)) = 4;
	UPPER(UPPER(n2)) = 1;
	LOWER(UPPER(n2)) = 2;
	UPPER(LOWER(n2)) = 3;
	LOWER(LOWER(n2)) = 4;
	minus256(&n1, &n2, &result);
	ASSERT_EQ(UPPER(UPPER(result)),0);
	ASSERT_EQ(LOWER(UPPER(result)),0);
	ASSERT_EQ(UPPER(LOWER(result)),0);
	ASSERT_EQ(LOWER(LOWER(result)),0);
	UPPER(UPPER(n1)) = 1;
	LOWER(UPPER(n1)) = 0;
	UPPER(LOWER(n1)) = 0;
	LOWER(LOWER(n1)) = 0;
	UPPER(UPPER(n2)) = 0;
	LOWER(UPPER(n2)) = 0xFFFFFFFFFFFFFFFF;
	UPPER(LOWER(n2)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(LOWER(n2)) = 0xFFFFFFFFFFFFFFFF;
	minus256(&n1, &n2, &result);
	ASSERT_EQ(UPPER(UPPER(result)),0);
	ASSERT_EQ(LOWER(UPPER(result)),0);
	ASSERT_EQ(UPPER(LOWER(result)),0);
	ASSERT_EQ(LOWER(LOWER(result)),1);
	UPPER(UPPER(n1)) = 0;
	LOWER(UPPER(n1)) = 0xFFFFFFFFFFFFFFFF;
	UPPER(LOWER(n1)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(LOWER(n1)) = 0xFFFFFFFFFFFFFFFF;
	UPPER(UPPER(n2)) = 1;
	LOWER(UPPER(n2)) = 0;
	UPPER(LOWER(n2)) = 0;
	LOWER(LOWER(n2)) = 0;
	minus256(&n1, &n2, &result);
	// TODO: same as minus128: because is unsigned int, check if this result is ok, or if the correct return is zero
	ASSERT_EQ(UPPER(UPPER(result)),0xFFFFFFFFFFFFFFFF);
	ASSERT_EQ(LOWER(UPPER(result)),0xFFFFFFFFFFFFFFFF);
	ASSERT_EQ(UPPER(LOWER(result)),0xFFFFFFFFFFFFFFFF);
	ASSERT_EQ(LOWER(LOWER(result)),0xFFFFFFFFFFFFFFFF);
}

TEST(TEST_UINT256, OR128)
{
	uint128_t n1, n2, result;
	UPPER(n1) = 0xF0F0;
	LOWER(n1) = 0xA0A0;
	UPPER(n2) = 0xA0A;
	LOWER(n2) = 0xF0F;
	or128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 0xFAFA);
	ASSERT_EQ(LOWER(result), 0xAFAF);
}

TEST(TEST_UINT256, OR256)
{
	uint256_t n1, n2, result;
	UPPER(UPPER(n1)) = 0x1010101010;
	LOWER(UPPER(n1)) = 0x2020202020;
	UPPER(LOWER(n1)) = 0x3030303030;
	LOWER(LOWER(n1)) = 0x4040404040;
	UPPER(UPPER(n2)) = 0xA0A0A0A0A;
	LOWER(UPPER(n2)) = 0xB0B0B0B0B;
	UPPER(LOWER(n2)) = 0xC0C0C0C0C;
	LOWER(LOWER(n2)) = 0xD0D0D0D0D;
	or256(&n1, &n2, &result);
	ASSERT_EQ(UPPER(UPPER(result)),0x1A1A1A1A1A);
	ASSERT_EQ(LOWER(UPPER(result)),0x2B2B2B2B2B);
	ASSERT_EQ(UPPER(LOWER(result)),0x3C3C3C3C3C);
	ASSERT_EQ(LOWER(LOWER(result)),0x4D4D4D4D4D);
}

TEST(TEST_UINT256, MUL128)
{
	uint128_t n1, n2, result;
	UPPER(n1) = 0x0;
	LOWER(n1) = 0x0;
	UPPER(n2) = 0xDADA;
	LOWER(n2) = 0xB4B4;
	mul128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 0x0);
	ASSERT_EQ(LOWER(result), 0x0);
	UPPER(n1) = 0x0;
	LOWER(n1) = 0x1;
	UPPER(n2) = 0xDADA;
	LOWER(n2) = 0xB4B4;
	mul128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 0xDADA);
	ASSERT_EQ(LOWER(result), 0xB4B4);
	UPPER(n1) = 0x1234;
	LOWER(n1) = 0x4321;
	UPPER(n2) = 0x0;
	LOWER(n2) = 0x1;
	mul128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 0x1234);
	ASSERT_EQ(LOWER(result), 0x4321);
	UPPER(n1) = 0x0;
	LOWER(n1) = 0xAAAAAAAAAAAAAAA1;
	UPPER(n2) = 0x0;
	LOWER(n2) = 0x3;
	mul128(&n1, &n2, &result);
	ASSERT_EQ(UPPER(result), 0x1);
	ASSERT_EQ(LOWER(result), 0xFFFFFFFFFFFFFFE3);
}

TEST(TEST_UINT256, MUL256)
{
	uint256_t n1, n2, result;
	UPPER(UPPER(n1)) = 0x0;
	LOWER(UPPER(n1)) = 0x0;
	UPPER(LOWER(n1)) = 0x0;
	LOWER(LOWER(n1)) = 0x0;
	UPPER(UPPER(n2)) = 0x1231230;
	LOWER(UPPER(n2)) = 0xB0B0B0B0B;
	UPPER(LOWER(n2)) = 0xC0C0C0C0C;
	LOWER(LOWER(n2)) = 0xD0D0D0D0D;
	mul256(&n1, &n2, &result);
	ASSERT_TRUE(zero256(&result));
	UPPER(UPPER(n1)) = 0x1231230;
	LOWER(UPPER(n1)) = 0xB0B0B0B0B;
	UPPER(LOWER(n1)) = 0xC0C0C0C0C;
	LOWER(LOWER(n1)) = 0xD0D0D0D0D;
	UPPER(UPPER(n2)) = 0x0;
	LOWER(UPPER(n2)) = 0x0;
	UPPER(LOWER(n2)) = 0x0;
	LOWER(LOWER(n2)) = 0x0;
    mul256(&n1, &n2, &result);
    ASSERT_TRUE(zero256(&result));
    UPPER(UPPER(n1)) = 0x1;
	LOWER(UPPER(n1)) = 0x2;
	UPPER(LOWER(n1)) = 0x4;
	LOWER(LOWER(n1)) = 0x8;
	UPPER(UPPER(n2)) = 0x0;
	LOWER(UPPER(n2)) = 0x0;
	UPPER(LOWER(n2)) = 0x0;
	LOWER(LOWER(n2)) = 0x2;
	mul256(&n1, &n2, &result);
	ASSERT_EQ(UPPER(UPPER(result)),0x2);
	ASSERT_EQ(LOWER(UPPER(result)),0x4);
	ASSERT_EQ(UPPER(LOWER(result)),0x8);
	ASSERT_EQ(LOWER(LOWER(result)),0x10);
    UPPER(UPPER(n1)) = 0x0;
	LOWER(UPPER(n1)) = 0x0;
	UPPER(LOWER(n1)) = 0x0;
	LOWER(LOWER(n1)) = 0xFFFFFFFFFFFFFFFF;
	UPPER(UPPER(n2)) = 0x0;
	LOWER(UPPER(n2)) = 0x0;
	UPPER(LOWER(n2)) = 0x0;
	LOWER(LOWER(n2)) = 0x2;
	mul256(&n1, &n2, &result);
	ASSERT_EQ(UPPER(UPPER(result)),0x0);
	ASSERT_EQ(LOWER(UPPER(result)),0x0);
	ASSERT_EQ(UPPER(LOWER(result)),0x1);
	ASSERT_EQ(LOWER(LOWER(result)),0xFFFFFFFFFFFFFFFE);
    UPPER(UPPER(n1)) = 0x0;
	LOWER(UPPER(n1)) = 0x0;
	UPPER(LOWER(n1)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(LOWER(n1)) = 0x0;
	UPPER(UPPER(n2)) = 0x0;
	LOWER(UPPER(n2)) = 0x0;
	UPPER(LOWER(n2)) = 0x0;
	LOWER(LOWER(n2)) = 0x2;
	mul256(&n1, &n2, &result);
	ASSERT_EQ(UPPER(UPPER(result)),0x0);
	ASSERT_EQ(LOWER(UPPER(result)),0x1);
	ASSERT_EQ(UPPER(LOWER(result)),0xFFFFFFFFFFFFFFFE);
	ASSERT_EQ(LOWER(LOWER(result)),0x0);
}

TEST(TEST_UINT256, DIVMOD128)
{
	uint128_t n1, n2, retDiv, retMod;
	UPPER(n1) = 0x0;
	LOWER(n1) = 0x8;
	UPPER(n2) = 0x0;
	LOWER(n2) = 0x2;
	clear128(&retDiv);
	clear128(&retMod);
	divmod128(&n1, &n2, &retDiv, &retMod);
	ASSERT_EQ(LOWER(retDiv), 4);
	ASSERT_TRUE(zero128(&retMod));
	UPPER(n1) = 0x0;
	LOWER(n1) = 0x9;
	UPPER(n2) = 0x0;
	LOWER(n2) = 0x2;
	clear128(&retDiv);
	clear128(&retMod);
	divmod128(&n1, &n2, &retDiv, &retMod);
	ASSERT_EQ(LOWER(retDiv), 4);
	ASSERT_EQ(LOWER(retMod), 1);
	UPPER(n1) = 0x4;
	LOWER(n1) = 0x0;
	UPPER(n2) = 0x0;
	LOWER(n2) = 0x2;
	clear128(&retDiv);
	clear128(&retMod);
	divmod128(&n1, &n2, &retDiv, &retMod);
	ASSERT_EQ(UPPER(retDiv), 2);
	ASSERT_EQ(LOWER(retDiv), 0);
	ASSERT_TRUE(zero128(&retMod));
	UPPER(n1) = 5;
	LOWER(n1) = 0;
	UPPER(n2) = 0;
	LOWER(n2) = 2;
	clear128(&retDiv);
	clear128(&retMod);

	divmod128(&n1, &n2, &retDiv, &retMod);
	ASSERT_EQ(UPPER(retDiv), 2);
	// TODO: Is zero the correct value to LOWER(retDiv) in this case?
	ASSERT_TRUE(LOWER(retDiv) != 0);
	//TODO: why retMod is zero in 5 / 1, should be 1, right?
	ASSERT_TRUE(zero128(&retMod));
}

TEST(TEST_UINT256, TOSTRING128)
{
	uint128_t number;
	char text[128];
	UPPER(number) = 0x1;
	LOWER(number) = 0x8;
	tostring128(&number, 16, text, 128);
	ASSERT_EQ(strcmp(text, "10000000000000008"), 0);
	UPPER(number) = 0x1;
	LOWER(number) = 0x0;
	tostring128(&number, 16, text, 128);
	ASSERT_EQ(strcmp(text, "10000000000000000"), 0);
	UPPER(number) = 0x0;
	LOWER(number) = 0x8;
	tostring128(&number, 16, text, 128);
	ASSERT_EQ(strcmp(text, "8"), 0);
}

TEST(TEST_UINT256, TOSTRING256)
{
	uint256_t number;
	char text[256];
    UPPER(UPPER(number)) = 0x1;
	LOWER(UPPER(number)) = 0x2;
	UPPER(LOWER(number)) = 0x4;
	LOWER(LOWER(number)) = 0x8;
	tostring256(&number, 16, text, 256);
	ASSERT_EQ(strcmp(text, "1000000000000000200000000000000040000000000000008"), 0);
    UPPER(UPPER(number)) = 0x0;
	LOWER(UPPER(number)) = 0x0;
	UPPER(LOWER(number)) = 0x4;
	LOWER(LOWER(number)) = 0x8;
	tostring256(&number, 16, text, 256);
	ASSERT_EQ(strcmp(text, "40000000000000008"), 0);
    UPPER(UPPER(number)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(UPPER(number)) = 0xFFFFFFFFFFFFFFFF;
	UPPER(LOWER(number)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(LOWER(number)) = 0xFFFFFFFFFFFFFFFF;
	tostring256(&number, 16, text, 256);
	ASSERT_EQ(strcmp(text, "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"), 0);
    UPPER(UPPER(number)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(UPPER(number)) = 0xFFFFFFFFFFFFFFFF;
	UPPER(LOWER(number)) = 0xFFFFFFFFFFFFFFFF;
	LOWER(LOWER(number)) = 0xFFFFFFFFFFFFFFFF;
	tostring256(&number, 2, text, 256);
	ASSERT_EQ(strcmp(text, "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111"), 0);
}
