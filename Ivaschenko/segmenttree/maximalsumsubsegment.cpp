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

			ReturnType get(std::size_t left, std::size_t right)
			{

			}

		private:
			std::vector<int> data;
	};
}

TEST(MaximalSumSubSegmentTree, Integration)
{

}
