#include <numeric>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "editdist.h"

struct RNG
{
	std::mt19937 generator;
	std::uniform_int_distribution<int> dist;

	RNG(int a, int b, std::size_t seed): generator(seed), dist(a, b) {}

	void setBounds(int a, int b)
	{
		dist = std::uniform_int_distribution<int>(a, b);
	}

	int operator () ()
	{
		return dist(generator);
	}
};

struct TestCase
{
	std::vector<int> first, second;

	TestCase(std::size_t type, std::size_t n, std::size_t m, int maxc, std::size_t seed)
	{
		if (type == 0)
		{
			RNG generator(-maxc, maxc, seed);
			first.resize(n);
			std::generate_n(first.begin(), n, generator);
			second.resize(m);
			std::generate_n(second.begin(), m, generator);
		}
		if (type == 1)
		{
			RNG numGenerator(-maxc, maxc, seed), idxGenerator(0, n, seed ^ 0xDEADBEEF);
			first.resize(n);
			std::generate_n(first.begin(), n, numGenerator);
			second = first;

			for (std::size_t it = 0; it < m; ++it)
			{
				idxGenerator.setBounds(0, 2);
				int t = idxGenerator();
				if (second.empty() || t == 0)
				{
					idxGenerator.setBounds(0, second.size());
					second.insert(second.begin() + idxGenerator(), numGenerator());
				}
				else if (t == 1)
				{
					idxGenerator.setBounds(0, second.size() - 1);
					second.erase(second.begin() + idxGenerator());
				}
				else if (t == 2)
				{
					idxGenerator.setBounds(0, second.size() - 1);
					second[idxGenerator()] = numGenerator();
				}
			}
		}
	}
};

template<typename DataType, typename RAIter> bool checkAnswer(std::vector<DataType> first,
															  const std::vector<DataType> &second,
															  const std::vector<editdist::EditAction<RAIter> > &actions)

{
	typedef editdist::EditAction<RAIter> Action;
	for (const Action &act: actions)
	{
		if (act.getType() == Action::ACTION_CHANGE)
			first[act.getIndex()] = *act.getAssignedValue();
		if (act.getType() == Action::ACTION_ERASE)
			first.erase(first.begin() + act.getIndex());
		if (act.getType() == Action::ACTION_INSERT)
			first.insert(first.begin() + act.getIndex(), *act.getAssignedValue());
	}
	return first == second;
}

TEST(EditDistance, SimpleTests)
{
	std::vector< std::pair< std::vector<int>, std::vector<int> > > tests =
	{
		{{4, 5}, {5}},
		{{1, 2, 3}, {4, 5, 1, 2, 3}},
		{{4, 1, 3}, {4, 3, 1}},
		{{1, 6, 8, 3, 5, 2, 3, 3, 5}, {5, 5, 2, 4, 1}},
		{{1}, {2}}
	};
	for (auto test : tests)
	{
		const std::vector<int> &a = test.first, &b = test.second;
		auto clever = editdist::editDist(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<int>());
		std::size_t answer = editdist::dummyEditDist(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<int>());
		ASSERT_EQ(answer, clever.size());
		ASSERT_TRUE(checkAnswer(a, b, clever));
	}
}

TEST(EditDistance, NonScalarDataTests)
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
		auto clever = editdist::editDist(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<std::string>());
		std::size_t answer = editdist::dummyEditDist(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<std::string>());
		ASSERT_EQ(answer, clever.size());
		ASSERT_TRUE(checkAnswer(a, b, clever));
	}
}

std::vector<TestCase> tests =
{
	TestCase(0, 3, 4, 5, 1),
	TestCase(0, 3, 5, 5, 2),
	TestCase(0, 4, 2, 5, 3),
	TestCase(0, 6, 1, 10, 4),
	TestCase(0, 6, 4, 10, 5),
	TestCase(0, 6, 6, 10, 6),
	TestCase(0, 7, 7, 5, 7),
	TestCase(0, 8, 4, 5, 8),
	TestCase(0, 10, 5, 3, 9),
	TestCase(0, 10, 10, 2, 10),
	TestCase(0, 100, 100, 5, 11),
	TestCase(0, 1000, 1000, 50, 12),
	TestCase(0, 2000, 2000, 100, 13),

	TestCase(1, 3, 4, 5, 14),
	TestCase(1, 3, 5, 5, 15),
	TestCase(1, 4, 2, 5, 16),
	TestCase(1, 6, 1, 10, 17),
	TestCase(1, 6, 4, 10, 18),
	TestCase(1, 6, 6, 10, 19),
	TestCase(1, 7, 7, 5, 20),
	TestCase(1, 8, 4, 5, 21),
	TestCase(1, 10, 5, 3, 22),
	TestCase(1, 10, 10, 2, 23),
	TestCase(1, 100, 100, 5, 24),
	TestCase(1, 1000, 10, 50, 25),
	TestCase(1, 2000, 20, 100, 26)
};

TEST(EditDistance, RandomTests)
{
	for (auto test : tests)
	{
		const std::vector<int> &a = test.first, &b = test.second;
		auto clever = editdist::editDist(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<int>());
		std::size_t answer = editdist::dummyEditDist(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<int>());
		ASSERT_EQ(answer, clever.size());
		ASSERT_TRUE(checkAnswer(a, b, clever));
	}
}
