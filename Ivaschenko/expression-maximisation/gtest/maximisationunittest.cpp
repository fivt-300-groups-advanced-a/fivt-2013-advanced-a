#include <gtest/gtest.h>

#include "expression-maximistion.h"

#include <algorithm>
#include <numeric>
#include <vector>

struct TestCase
{
	std::vector<int> data;

	TestCase(const std::initializer_list<int> &list): data(list)  {}
	TestCase(std::size_t seed, std::size_t n, std::size_t c)
	{
		data.resize(n);
		std::mt19937 generator(seed);
		std::generate_n(data.begin(), n, generator);
		for (std::size_t i = 0; i < n; ++i)
		{
			data[i] %= 2 * c + 1;
			if (data[i] < 0) data[i] += 2 * c + 1;
			data[i] -= c;
		}
	}
};

TEST(ExpressionMaximisation, SimpleTests)
{
	std::vector<TestCase> tests =
	{
		TestCase({1, 2, 3}),
		TestCase({3, 2, 1}),
		TestCase({1, 0, 1}),
		TestCase({0, 0, 1}),
		TestCase({0, 0, 0}),
		TestCase({0, 5, 0}),
		TestCase({4, 3, 0}),
		TestCase({1, 2, 1}),
		TestCase({0, 3, 1}),
		TestCase({0, 10, 1}),
		TestCase({-13, 13, -13}),
		TestCase({-13, -13, -13}),
		TestCase({1, -1, 0}),
		TestCase({1, -1, -1})
	};

	for (auto test : tests)
	{
		ASSERT_EQ(maximizeExpression<int>(test.data.begin(), test.data.end()),
				  maximizeExpressionDummy<int>(test.data.begin(), test.data.end()));
	}
}

TEST(ExpressionMaximisation, Stress)
{
	std::vector<TestCase> tests =
	{
		TestCase(1, 3, 10),
		TestCase(2, 3, 10),
		TestCase(3, 4, 10),
		TestCase(4, 5, 10),
		TestCase(5, 5, 10),
		TestCase(6, 10, 10),
		TestCase(7, 10, 10),
		TestCase(8, 10, 10),
		TestCase(9, 15, 10),
		TestCase(10, 15, 10),
		TestCase(11, 15, 10),
		TestCase(12, 20, 2)
	};

	for (auto test : tests)
	{
		ASSERT_EQ(maximizeExpression<long long>(test.data.begin(), test.data.end()),
				  maximizeExpressionDummy<long long>(test.data.begin(), test.data.end()));
	}
}
