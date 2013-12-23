#include <gtest/gtest.h>

#include "applications/maximalsumsubsegment.h"

#include <vector>
#include <algorithm>

namespace
{
	class DummyMaxSubSegment
	{
		public:
			template<typename ForwardIterator> DummyMaxSubSegment(ForwardIterator begin, ForwardIterator end)
				: data(begin, end) {}

			template<typename ForwardIterator> DummyMaxSubSegment(std::size_t n, const int &value)
				: data(n, value) {}


			struct ReturnType
			{
				int sum, left, right;

				ReturnType(const int &nValue): sum(nValue), left(-1), right(-1)	{}
			};

			void update(std::size_t left, std::size_t right, int value)
			{
				std::fill(data.begin() + left, data.begin() + right + 1, value);
			}

			ReturnType maximalSubsegment(std::size_t left, std::size_t right)
			{
				ReturnType answer(0);
				for (size_t i = left; i <= right; ++i)
				{
					int sum = 0;
					for (size_t j = i; j <= right; ++j)
					{
						sum += data[j];
						if (sum > answer.sum)
							answer.sum = sum, answer.left = i, answer.right = j + 1;
					}
				}
				return answer;
			}

			int sum(std::size_t left, std::size_t right)
			{
				int sum = 0;
				for (std::size_t i = left; i < right; ++i)
					sum += data[i];
				return sum;
			}

		private:
			std::vector<int> data;
	};
}

TEST(MaximalSumSubSegmentTree, Stress)
{
	class Query
	{
		public:
			Query() {}
			Query(std::size_t nType, std::size_t nLeft, std::size_t nRight, int nValue):
				type(nType), left(nLeft), right(nRight), value(nValue) {}

			std::size_t type, left, right;
			int value;

	};

	class TestCase
	{
		public:
			TestCase(const std::initializer_list<int> &nData, const std::initializer_list<Query> &nQueries):
				data(nData), queries(nQueries) {}

			std::vector<int> data;
			std::vector<Query> queries;
	};

	class TestCaseGenerator
	{
		public:
			static Query genQuery(std::size_t n, std::size_t maxValue, std::mt19937 &generator)
			{
				std::size_t left = generator() % n, right = generator() % n;
				if (left > right) std::swap(left, right);
				int value = generator() % (2 * maxValue + 1) - maxValue;
				return Query(generator() & 1, left, right, value);
			}

			static TestCase genTestCase(std::size_t n, std::size_t m, std::size_t maxValue, std::size_t seed)
			{
				std::mt19937 generator(seed);
				TestCase test({}, {});
				test.data.resize(n);
				std::generate(test.data.begin(), test.data.end(),
							  [&generator, maxValue] () { return generator() % (2 * maxValue + 1) - maxValue; });
				test.queries.resize(m);
				std::generate(test.queries.begin(), test.queries.end(),
							  [n, maxValue, &generator] () { return genQuery(n, maxValue, generator); });
				return test;
			}
	};

	std::vector<TestCase> tests =
	{
		TestCase({-5, -10, -10, -5}, {Query(0, 0, 2, 0)}),
		TestCaseGenerator::genTestCase(4, 1, 10, 1),
		TestCaseGenerator::genTestCase(6, 3, 10, 2),
		TestCaseGenerator::genTestCase(8, 5, 10, 3),
		TestCaseGenerator::genTestCase(10, 9, 10, 4),
		TestCaseGenerator::genTestCase(8, 1, 10, 5),
		TestCaseGenerator::genTestCase(100, 1000, 1000, 5)
	};

	for (auto test : tests)
	{
		MaximalSumSubSegmentTree<int> tree(test.data.begin(), test.data.end());
		DummyMaxSubSegment dummy(test.data.begin(), test.data.end());
		for (Query q : test.queries)
		{
			if (q.type)
			{
				dummy.update(q.left, q.right, q.value);
				tree.update(q.left, q.right, q.value);
			}
			else
			{
				auto answer = tree.maximalSubsegment(q.left, q.right);
				auto correct = dummy.maximalSubsegment(q.left, q.right);
				EXPECT_EQ(answer.sum, correct.sum);
				EXPECT_EQ(dummy.sum(answer.left, answer.right), answer.sum);
			}
		}
	}
}
