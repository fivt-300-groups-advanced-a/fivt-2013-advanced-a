#include <vector>
#include <algorithm>

#include <gtest/gtest.h>

#include "externalsorter.h"
#include "sorters/standartsorter.h"

TEST(ExternalSorter, BaseIntegrationTest)
{
	class ShuffledPermutationReader
	{
		public:
			ShuffledPermutationReader(int n, int seed): blockSize(100000), currentBlock(0), unread(0), testSize(n)
			{
				blocksPermutation.reserve(n / blockSize + 1);
				for (int i = 0; i < n; i += blockSize)
					blocksPermutation.push_back(i);
				srand(seed);
				std::random_shuffle(blocksPermutation.begin(), blocksPermutation.end());
			}

			bool operator() (int &x)
			{
				if (unread >= permutation.size())
				{
					if (currentBlock >= blocksPermutation.size()) return false;
					permutation.clear();
					int start = blocksPermutation[currentBlock];
					for (std::size_t i = 0; i < blockSize && i + start < testSize; ++i)
						permutation.push_back(start + i);
					std::random_shuffle(permutation.begin(), permutation.end());
					++currentBlock, unread = 0;
				}
				x = permutation[unread++];
				return true;
			}

		private:
			const std::size_t blockSize;

			std::vector<int> permutation;
			std::vector<int> blocksPermutation;
			std::size_t currentBlock, unread, testSize;
	};

	class SortedPermutationWriter
	{
		public:
			SortedPermutationWriter(): current(0) {}

			bool operator() (int x)
			{
				EXPECT_EQ(x, current);
				current++;
				return true;
			}

		private:
			int current;
	};

	std::vector< std::pair<int, std::size_t> > tests = {{5, 4}, {5, 12}, {5, 100}, {10, 40}, {10, 4}, {10, 12},
														{100, 40}, {100000, 10000}, {1000000, 100000}};

	ExternalSorter<int> sorter;
	for (size_t i = 0; i < tests.size(); ++i)
	{
		std::cout << "test #" << i << std::endl;
		std::cout << "    size = " << tests[i].first << " blockSize = " << tests[i].second << " seed = " << i << std::endl;
		ShuffledPermutationReader reader(tests[i].first, i);
		SortedPermutationWriter writer;
		std::clock_t start = std::clock();
		sorter.sort(tests[i].second, reader, writer, StandartSorter<int>(), std::less<int>());
		std::cout << "    Sorting time = " << (clock() - start) / 1000 << "ms" << std::endl;
	}
}
