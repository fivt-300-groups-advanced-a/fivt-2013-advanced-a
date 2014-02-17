#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "editdist.h"

TEST(EditorDistance, SimpleTests)
{
	std::vector< std::pair< std::vector<int>, std::vector<int> > > tests =
	{
		{{1, 2, 3}, {4, 5, 1, 2, 3}},
		{{4, 1, 3}, {4, 3, 1}},
		{{1, 6, 8, 3, 5, 2, 3, 3, 5}, {5, 5, 2, 4, 1}},
		{{1}, {2}}
	};
	for (auto test : tests)
	{
		const std::vector<int> &a = test.first, &b = test.second;
		auto clever = largestCommonSubsequence(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<int>());
		auto dummy = dummyLargestCommonSubsequence(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<int>());
		ASSERT_EQ(clever.size(), dummy.size());
		std::size_t pos = 0;
		for (auto it : clever)
		{
			while (pos < a.size() && a[pos] != *it) ++pos;
			ASSERT_NE(pos, a.size());
			++pos;
		}
	}
}

TEST(EditorDistance, NonScalarDataTests)
{
	std::vector< std::pair< std::vector<std::string>, std::vector<std::string> > > tests =
	{
		{{"ab", "bc", "cd"}, {"qq", "ww", "ab", "bc", "cd"}},
		{{"abacaba", "xxx", "yyy"}, {"abacaba", "yyy", "xxx"}},
		{{"some string"}, {"another string"}}
	};
	for (auto test : tests)
	{
		const std::vector<std::string> &a = test.first, &b = test.second;
		auto clever = largestCommonSubsequence(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<std::string>());
		auto dummy = dummyLargestCommonSubsequence(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<std::string>());
		ASSERT_EQ(clever.size(), dummy.size());
		std::size_t pos = 0;
		for (auto it : clever)
		{
			while (pos < a.size() && a[pos] != *it) ++pos;
			ASSERT_NE(pos, a.size());
			++pos;
		}
	}
}

TEST(EditorDistance, RandomTests)
{
	struct TestCase
	{
		std::size_t n, m, seed;

		TestCase(std::size_t n, std::size_t m, std::size_t seed): n(n), m(m), seed(seed) {}

		std::pair< std::vector<int>, std::vector<int> > generateData()
		{
			std::mt19937 generator(seed);
			std::pair< std::vector<int>, std::vector<int> > answer;
			answer.first.resize(n);
			std::generate_n(answer.first.begin(), n, generator);
			answer.second.resize(m);
			std::generate_n(answer.second.begin(), m, generator);
			return answer;
		}
	};

	std::vector<TestCase> tests =
	{
		TestCase(3, 4, 1),
		TestCase(3, 5, 2),
		TestCase(4, 2, 3),
		TestCase(6, 1, 4),
		TestCase(6, 4, 5),
		TestCase(6, 6, 6),
		TestCase(7, 7, 7),
		TestCase(8, 4, 8),
		TestCase(10, 5, 9),
		TestCase(10, 10, 10),
		TestCase(100, 100, 11),
		TestCase(1000, 1000, 12),
		TestCase(2000, 2000, 13)
	};

	for (auto test : tests)
	{
		auto data = test.generateData();
		const std::vector<int> &a = data.first, &b = data.second;
		auto clever = largestCommonSubsequence(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<int>());
		auto dummy = dummyLargestCommonSubsequence(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<int>());
		ASSERT_EQ(clever.size(), dummy.size());
		std::size_t pos = 0;
		for (auto it : clever)
		{
			while (pos < a.size() && a[pos] != *it) ++pos;
			ASSERT_NE(pos, a.size());
			++pos;
		}
	}
}
