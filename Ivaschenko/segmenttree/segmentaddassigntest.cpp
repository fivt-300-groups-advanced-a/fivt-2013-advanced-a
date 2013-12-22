#include <functional>
#include <algorithm>
#include <numeric>
#include <vector>
#include <array>

#include <gtest/gtest.h>

#include "model/segmentadditionassignmenttree.h"

namespace
{
	template<typename DataType> class DummyStructure
	{
		public:
			DummyStructure(std::size_t size): data(size) {}
			template<typename ForwardIterator> DummyStructure(ForwardIterator begin, ForwardIterator end): data(begin, end) {}

			DataType getMin(std::size_t left, std::size_t right)
			{
				return *std::min_element(data.begin() + left, data.begin() + right + 1);
			}

			DataType getMax(std::size_t left, std::size_t right)
			{
				return *std::max_element(data.begin() + left, data.begin() + right + 1);
			}

			DataType getSum(std::size_t left, std::size_t right)
			{
				return std::accumulate(data.begin() + left, data.begin() + right + 1, 0);
			}

			void assign(std::size_t left, std::size_t right, const DataType &value)
			{
				for (size_t i = left; i <= right; ++i)
					data[i] = value;
			}

			void add(std::size_t left, std::size_t right, const DataType &value)
			{
				for (size_t i = left; i <= right; ++i)
					data[i] += value;
			}

		private:
			std::vector<DataType> data;
	};
}

struct TestCase
{
	std::size_t size, queries, seed;

	TestCase(std::size_t nSize, std::size_t nQueries, std::size_t nSeed):
		size(nSize), queries(nQueries), seed(nSeed) {}
};

TEST(SegmentAdditionAssignmentTree, Stress)
{
	std::vector<TestCase> tests = {
		TestCase(1, 3, 1), TestCase(2, 4, 1), TestCase(3, 5, 2), TestCase(4, 6, 3),
		TestCase(5, 10, 5),	TestCase(6, 15, 8), TestCase(7, 15, 13), TestCase(8, 15, 21),
		TestCase(14, 20, 34), TestCase(16, 20, 55)
	};
	for (auto test : tests)
	{
		std::mt19937 generator(test.seed);
		std::vector<int> data(test.size);
		std::generate(data.begin(), data.end(), [&generator] () { return generator() % 100; });

		DummyStructure<int> dummy(data.begin(), data.end());
		SegmentAdditionAssignmentTree<int> tree(data.begin(), data.end(),
												std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 0);

		for (size_t i = 0; i < test.queries; ++i)
		{
			std::size_t left = generator() % test.size, right = generator() % test.size, type = generator() % 2;
			if (left > right) std::swap(left, right);

			if (type == 0)
			{
				SegmentAdditionAssignmentTree<int>::ReturnType answer = tree.get(left, right);
				EXPECT_EQ(answer.min, dummy.getMin(left, right))
					<< "min query #" << i + 1 << " on test with n = " << test.size;
				EXPECT_EQ(answer.max, dummy.getMax(left, right))
					<< "max query #" << i + 1 << " on test with n = " << test.size;
				EXPECT_EQ(answer.sum, dummy.getSum(left, right))
					<< "sum query #" << i + 1 << " on test with n = " << test.size;
			}
			else if (type == 1)
			{
				int value = generator() % 100;
				tree.add(left, right, value);
				dummy.add(left, right, value);
			}
			else
			{
				int value = generator() % 100;
				tree.assign(left, right, value);
				dummy.assign(left, right, value);
			}
		}
	}
}

// TODO: SegmentAddtionAssignmentTree integration
