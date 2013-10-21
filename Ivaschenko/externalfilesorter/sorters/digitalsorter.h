#ifndef DIGITALSORTER_H
#define DIGITALSORTER_H

#include <numeric>
#include <utility>

/**
 * Sorter class, implementing digital stable sort algorithm to sort all data which can be represented as bit string
 * O(blocksCount * (N + blockRange)) complexity, uses O(N + blockRange) addiditional memory.
 * Stable.
 * Requires a BitBlockExtractor for DataType which should divide a complex object into a short
 * sortable blocks. BitBlockExtractor interface contains:
 *     std::size_t extract(const &T value, std::size_t block)
 *	   std::size_t getBlocksNumber(RAcessIterator begin, RAccessIterator end)
 *	   std::size_t getBlockRange(std::size_t block) (zero-based)
 */
template<typename DataType, typename BitBlockExtractor> class DigitalSorter
{
	public:
		template <typename RandomAccessIterator>
		void operator () (RandomAccessIterator begin, RandomAccessIterator end)
		{
			std::size_t n = std::distance(begin, end);
			if (n == 0) return;

			BitBlockExtractor extractor(begin, end);
			std::size_t iterations = extractor.getBlocksNumber();

			std::vector<std::size_t> permutation(n);
			for (std::size_t i = 0; i < permutation.size(); ++i)
				permutation[i] = i;

			for (std::size_t it = 0; it < iterations; ++it)
			{
				std::size_t range = extractor.getBlockRange(it);
				std::vector<std::size_t> cnt(range, 0);
				for (RandomAccessIterator jt = begin; jt != end; ++jt)
				{
					size_t cur = extractor(*jt, it);
					++cnt[extractor(*jt, it)];
				}
				std::partial_sum(cnt.begin(), cnt.end(), cnt.begin());

				std::vector<std::size_t> newPermutation(n);
				std::size_t current = n;
				do
				{
					--current;
					std::size_t part = extractor(*(begin + permutation[current]), it);
					newPermutation[--cnt[part]] = permutation[current];
				}
				while (current);
				permutation = newPermutation;
			}
			std::vector<DataType> result;
			result.reserve(n);
			for (std::size_t i = 0; i < n; ++i)
				result.push_back(*(begin + permutation[i]));
			std::copy(result.begin(), result.end(), begin);
		}
};

#endif // DIGITALSORTER_H
