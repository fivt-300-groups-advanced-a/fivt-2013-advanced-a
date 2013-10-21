#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include "sorters/digitalsorter.h"

#include "utils/integerbitblockextractor.h"

TEST(DigitalSorter, UnsignedIntegerSorting)
{
	std::vector< std::vector<unsigned> > tests =
	{
		{454u, 19848u, 29495839u, 198482u, 921u, 9482u, 294u},
		{1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 0u, 0u, 0u},
		{4929u, 0u, 1958u, 549u, 29911u, 2458u}
	};

	DigitalSorter<unsigned, IntegerBitBlockExtractor<unsigned> > sorter;
	for (auto test : tests)
	{
		std::vector<unsigned> answer = test, output = test;
		std::sort(answer.begin(), answer.end());
		sorter(output.begin(), output.end());
		EXPECT_EQ(answer, output);
	}
}

TEST(DigitalSorter, SignedIntegerSorting)
{
	std::vector< std::vector<int> > tests =
	{
		{1, -1, 0},
		{454, -19848, -29495839, -198482, 921, 9482, -294},
		{1, -1, 1, -1, 1, 1, -1, -1, 1, 0, 0, 0},
		{-4929, 0, 1958, -549, -29911, 2458}
	};

	DigitalSorter<int, IntegerBitBlockExtractor<int> > sorter;
	for (auto test : tests)
	{
		std::vector<int> answer = test, output = test;
		std::sort(answer.begin(), answer.end());
		sorter(output.begin(), output.end());
		EXPECT_EQ(answer, output);
	}
}

TEST(DigitalSorter, UnsignedLongLong)
{
	std::vector< std::vector<unsigned long long> > tests =
	{
		{45423598251256ull, 198482352135ull, 2213329495839ull, 198482352ull, 923523521ull, 123559482ull, 774294ull},
		{1ull, 1ull, 1ull, 1ull, 1ull, 1ull, 1ull, 1ull, 1ull, 0ull, 0ull, 0ull},
		{49291123490ull, 0ull, (1ull << 60) - 1, 523598852249ull, 29911354361ull, 241354364558ull}
	};

	DigitalSorter<unsigned long long, IntegerBitBlockExtractor<unsigned long long> > sorter;
	for (auto test : tests)
	{
		std::vector<unsigned long long> answer = test, output = test;
		std::sort(answer.begin(), answer.end());
		sorter(output.begin(), output.end());
		EXPECT_EQ(answer, output);
	}
}

TEST(DigitalSorter, SignedLongLong)
{
	std::vector< std::vector<long long> > tests =
	{
		{std::numeric_limits<long long>::min(), -492},
		{29481, std::numeric_limits<long long>::max()},
		{0ll, 1ll, -1ll},
		{45423598251256ll, -198482352135ll, -2213329495839ll, -198482352ll, -923523521ll, 123559482ll, 774294ll},
		{1ll, 1ll, 1ll, -1ll, 1ll, -1ll, -1ll, -1ll, -1ll, 0ll, 0ll, 0ll},
		{-49291123490ll, 0ll, (1ll << 60) - 1, 523598852249ll, 29911354361ll, 241354364558ll,
		 std::numeric_limits<long long>::max(), std::numeric_limits<long long>::min()}
	};

	DigitalSorter<long long, IntegerBitBlockExtractor<long long> > sorter;
	for (auto test : tests)
	{
		std::vector<long long> answer = test, output = test;
		std::sort(answer.begin(), answer.end());
		sorter(output.begin(), output.end());
		EXPECT_EQ(answer, output);
	}
}

