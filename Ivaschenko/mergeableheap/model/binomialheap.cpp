#include <gtest/gtest.h>

#include <algorithm>
#include <vector>
#include <random>

#include "model/binomialheap.h"
#include "model/binomialheapnode.h"

#include "model/testaccess.h"

template<typename Class> class TestAccess;

namespace
{
	class RandomSequenceGenerator
	{
		public:
			static std::vector<int> genRandomSequence(std::size_t size, int seed,
													  int minValue = std::numeric_limits<int>::min(),
													  int maxValue = std::numeric_limits<int>::max())
			{
				std::mt19937 generator(seed);
				std::uniform_int_distribution<int> distribution(minValue, maxValue);

				std::vector<int> result(size);
				std::generate_n(result.begin(), size, [&] () { return distribution(generator); });
				return result;
			}
	};
}

TEST(BinomialHeapIdentifier, TestInvariants)
{
	std::vector< std::vector<int> > tests =
	{
		{0, 4, -1, 438, 82, 954, 91, 19, 92, 29},
		RandomSequenceGenerator::genRandomSequence(5, 1, 1, 10),
		RandomSequenceGenerator::genRandomSequence(10, 2, -10, 10),
		RandomSequenceGenerator::genRandomSequence(100, 3),
		RandomSequenceGenerator::genRandomSequence(1000, 4)
	};

	for (auto test : tests)
	{
		std::vector< std::pair<BinomialHeap<int>::NodeIdType, int> > ids;
		BinomialHeap<int> heap;
		for (int x : test)
		{
			ids.push_back(std::make_pair(heap.push(x), x));
			for (auto p : ids)
				ASSERT_EQ(*p.first, p.second);
		}
	}
}

TEST(BinomialHeap, TestBaseInvariants)
{
	std::vector< std::vector<int> > tests =
	{
		{0, 4, -1, 438, 82, 954, 91, 19, 92, 29},
		RandomSequenceGenerator::genRandomSequence(5, 1, 1, 10),
		RandomSequenceGenerator::genRandomSequence(10, 2, -10, 10),
		RandomSequenceGenerator::genRandomSequence(100, 3),
		RandomSequenceGenerator::genRandomSequence(1000, 4)
	};

	TestAccess< BinomialHeap<int, std::less<int> > > heapAccess;
	for (auto test : tests)
	{
		BinomialHeap<int> heap;
		for (int x : test)
		{
			heap.push(x);
			heapAccess.checkInvariants(heap);
		}
		while (!heap.empty())
		{
			heap.pop();
			heapAccess.checkInvariants(heap);
		}
	}
}

TEST(BinomialHeap, TestMergeInvariants)
{
	struct TestCase
	{
		std::vector<int> first, second;

		TestCase(const std::initializer_list<int> &nFirst,
				 const std::initializer_list<int> &nSecond): first(nFirst), second(nSecond) {}

		TestCase(const std::vector<int> &nFirst, const std::vector<int> &nSecond): first(nFirst), second(nSecond) {}
	};

	std::vector<TestCase> tests =
	{
		TestCase({1, 2, 3, 4}, {5, 4, 3, 2}),
		TestCase({1}, {3, 2, 1}),
		TestCase({1, 2, 3, 4, 5}, {2, 4}),
		TestCase(RandomSequenceGenerator::genRandomSequence(119, 34), RandomSequenceGenerator::genRandomSequence(298, 51))
	};
	for (auto test : tests)
	{
		BinomialHeap<int> first, second;
		TestAccess< BinomialHeap<int> > heapAccess;
		for (auto x : test.first) first.push(x);
		for (auto x : test.second) second.push(x);
		first.merge(second);
		heapAccess.checkInvariants(first);
		ASSERT_TRUE(second.empty());
		ASSERT_EQ(first.size(), test.first.size() + test.second.size());

		std::vector<int> answer;
		answer.insert(answer.end(), test.first.begin(), test.first.end());
		answer.insert(answer.end(), test.second.begin(), test.second.end());
		sort(answer.begin(), answer.end());

		for (size_t i = 0; i < answer.size(); ++i)
		{
			ASSERT_FALSE(first.empty());
			ASSERT_TRUE(answer[i] == first.pop());
		}
	}
}
