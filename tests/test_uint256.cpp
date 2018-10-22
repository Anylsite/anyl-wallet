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
#include <string.h>

TEST(ZERO128, ZERO128_SET)
{
	uint128_t n1, n2;
	UPPER(n1) = 123;
	LOWER(n1) = 456;
	UPPER(n2) = 0;
	LOWER(n2) = 0;
	ASSERT_TRUE(!zero128(&n1));
	ASSERT_TRUE(zero128(&n2));
}

TEST(ZERO256, ZERO256_SET)
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

TEST(COPY128, COPY128_SET)
{
	uint128_t number, result;
	UPPER(number) = 123;
	LOWER(number) = 456;
	copy128(&result, &number);
	ASSERT_EQ(UPPER(number), UPPER(result));
	ASSERT_EQ(LOWER(number), LOWER(result));
}

TEST(COPY256, COPY256_SET)
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

TEST(CLEAR256, CLEAR256_SET)
{
	uint256_t number;
	UPPER(LOWER(number)) = 0;
	LOWER(LOWER(number)) = 1;
	clear256(&number);
	ASSERT_EQ(LOWER(LOWER(number)), 0);
}

TEST(CLEAR128, CLEAR128_SET)
{
	uint128_t number;
	UPPER(number) = 0;
	LOWER(number) = 1;
	clear128(&number);
	ASSERT_EQ(LOWER(number), 0);
}
