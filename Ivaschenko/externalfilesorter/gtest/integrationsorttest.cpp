#include <vector>
#include <random>
#include <algorithm>

#include <gtest/gtest.h>

#include "externalsorter.h"

#include "utils/integerbitblockextractor.h"

#include "sorters/digitalsorter.h"
#include "sorters/standartsorter.h"
#include "sorters/standartstablesorter.h"

TEST(ExternalSorter, PermutaionSort)
{
	class ShuffledPermutationReader
	{
		public:
			ShuffledPermutationReader(int n, int seed): generator(seed),
														blockSize(100000), currentBlock(0),
														unread(0), testSize(n)
			{
				blocksPermutation.reserve(n / blockSize + 1);
				for (int i = 0; i < n; i += blockSize)
					blocksPermutation.push_back(i);
				std::shuffle(blocksPermutation.begin(), blocksPermutation.end(), generator);
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
					std::shuffle(permutation.begin(), permutation.end(), generator);
					++currentBlock, unread = 0;
				}
				x = permutation[unread++];
				return true;
			}

		private:
			std::mt19937 generator;

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

	ExternalSorter<int, std::less<int> > sorter;
	for (size_t i = 0; i < tests.size(); ++i)
	{
		std::cout << "test #" << i << std::endl;
		std::cout << "    size = " << tests[i].first << " blockSize = " << tests[i].second << " seed = " << i << std::endl;
		ShuffledPermutationReader reader(tests[i].first, i);
		SortedPermutationWriter writer;
		std::clock_t start = std::clock();
		sorter.sort(tests[i].second, reader, writer, StandartSorter<int>());
		std::cout << "    Sorting time = " << (clock() - start) / 1000 << "ms" << std::endl;
	}
}

TEST(ExternalSorter, RandomIntegerSequenceSorting)
{
	class RandomSequenceReader
	{
		public:
			RandomSequenceReader(std::size_t n, int seed): unread(n), generator(seed) {}

			bool operator () (int &x)
			{
				if (!unread) return false;
				--unread;
				x = generator();
				return true;
			}

		private:
			std::size_t unread;
			std::mt19937 generator;
	};

	class SortedSequenceWriter
	{
		public:
			SortedSequenceWriter(std::size_t n): readed(0), testSize(n) {}

			bool operator() (int x)
			{
				EXPECT_FALSE(readed == testSize) << " extra numbers in output";
				if (readed > 0) EXPECT_LE(prev, x) << "Error a[" << readed << "] > a[" << readed + 1 << "]";
				++readed, prev = x;
				return true;
			}

			std::size_t getReaded() const
			{
				return readed;
			}

		private:
			std::size_t readed, testSize;
			int prev;
	};

	ExternalSorter<int, std::less<int> > sorter;
	std::vector< std::pair<std::size_t, std::size_t> > tests =
	{
		{5, 4}, {6, 4}, {7, 8}, {8, 40}, {9, 100}, {10, 8},
		{100, 10}, {500, 500}, {300500, 16000}
	};

	for (size_t i = 0; i < tests.size(); ++i)
	{
		std::cout << "test #" << i << std::endl;
		std::cout << "    size = " << tests[i].first << " mem = " << tests[i].second << " seed = " << i << std::endl;
		RandomSequenceReader reader(tests[i].first, i);
		SortedSequenceWriter writer(tests[i].first);
		std::clock_t start = std::clock();
		sorter.sort(tests[i].second, reader, writer, StandartSorter<int>());
		EXPECT_TRUE(writer.getReaded() == tests[i].first);
		std::cout << "    Sorting time = " << (clock() - start) / 1000 << "ms" << std::endl;
	}
}



TEST(ExternalSorter, DigitalSignedPemutationSort)
{
	class ShuffledPermutationReader
	{
		public:
			ShuffledPermutationReader(int n, int seed): generator(seed),
														blockSize(100000), currentBlock(0),
														unread(0), testSize(n)
			{
				blocksPermutation.reserve(n / blockSize + 1);
				for (int i = 0; i < n; i += blockSize)
					blocksPermutation.push_back(i);
				std::shuffle(blocksPermutation.begin(), blocksPermutation.end(), generator);
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
					std::shuffle(permutation.begin(), permutation.end(), generator);
					++currentBlock, unread = 0;
				}
				x = permutation[unread++];
				return true;
			}

		private:
			std::mt19937 generator;

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

	ExternalSorter<int, std::less<int> > sorter;
	for (size_t i = 0; i < tests.size(); ++i)
	{
		std::cout << "test #" << i << std::endl;
		std::cout << "    size = " << tests[i].first << " blockSize = " << tests[i].second << " seed = " << i << std::endl;
		ShuffledPermutationReader reader(tests[i].first, i);
		SortedPermutationWriter writer;
		std::clock_t start = std::clock();
		sorter.sort(tests[i].second, reader, writer, DigitalSorter<int, IntegerBitBlockExtractor<int> >());
		std::cout << "    Sorting time = " << (clock() - start) / 1000 << "ms" << std::endl;
	}
}

TEST(ExternalSorter, RandomUnsignedIntegerSequenceSorting)
{
	class RandomSequenceReader
	{
		public:
			RandomSequenceReader(std::size_t n, int seed): unread(n), generator(seed) {}

			bool operator () (unsigned &x)
			{
				if (!unread) return false;
				--unread;
				x = generator();
				return true;
			}

		private:
			std::size_t unread;
			std::mt19937 generator;
	};

	class SortedSequenceWriter
	{
		public:
			SortedSequenceWriter(std::size_t n): readed(0), testSize(n) {}

			bool operator() (unsigned x)
			{
				EXPECT_FALSE(readed == testSize) << " extra numbers in output";
				if (readed > 0) EXPECT_LE(prev, x) << "Error a[" << readed << "] > a[" << readed + 1 << "]";
				++readed, prev = x;
				return true;
			}

			std::size_t getReaded() const
			{
				return readed;
			}

		private:
			std::size_t readed, testSize;
			int prev;
	};

	ExternalSorter<unsigned, std::less<unsigned> > sorter;
	std::vector< std::pair<std::size_t, std::size_t> > tests =
	{
		{5, 4}, {6, 4}, {7, 8}, {8, 40}, {9, 100}, {10, 8},
		{100, 10}, {500, 500}, {300500, 16000}
	};

	for (size_t i = 0; i < tests.size(); ++i)
	{
		std::cout << "test #" << i << std::endl;
		std::cout << "    size = " << tests[i].first << " mem = " << tests[i].second << " seed = " << i << std::endl;
		RandomSequenceReader reader(tests[i].first, i);
		SortedSequenceWriter writer(tests[i].first);
		std::clock_t start = std::clock();
		sorter.sort(tests[i].second, reader, writer, DigitalSorter<unsigned, IntegerBitBlockExtractor<unsigned> >());
		EXPECT_TRUE(writer.getReaded() == tests[i].first);
		std::cout << "    Sorting time = " << (clock() - start) / 1000 << "ms" << std::endl;
	}
}

TEST(ExternalSorter, CustomDataSorting)
{
	struct Animal
	{
		public:
			Animal() {}

			char name[10];
			int age;
			char eyeColor;
			std::size_t weight;
			long long cost, speed;

			bool operator < (const Animal &a) const
			{
				int namesCmp = strcmp(name, a.name);
				if (namesCmp) return namesCmp < 0;
				if (eyeColor != a.eyeColor)	return toupper(eyeColor) < toupper(a.eyeColor);
				if (cost + speed != a.cost + a.speed) return cost + speed < a.cost + a.speed;
				if (speed != a.speed) return speed > a.speed;
				return (weight ^ age) < (a.weight ^ a.age);
			}
	};

	class AnimalReader
	{
		public:
			AnimalReader(std::size_t n, int seed): generator(seed), unread(n) {}

			bool operator () (Animal &a)
			{
				if (!unread) return false;
				--unread;
				for (int i = 0; i < 9; ++i) a.name[i] = generator();
				a.name[9] = 0;
				a.age = ((generator() % 40) + 40) % 40;
				a.eyeColor = generator();
				a.weight = ((generator() % 3000) + 3000) % 3000;
				a.cost = generator();
				a.weight = generator() % 1000000;
				return true;
			}

		private:
			std::mt19937_64 generator;
			std::size_t unread;
	};

	class SortedAnimalsWriter
	{
		public:
			SortedAnimalsWriter(std::size_t n): readed(0), testSize(n) {}

			bool operator() (const Animal &x)
			{
				EXPECT_FALSE(readed == testSize) << " extra numbers in output";
				if (readed > 0) EXPECT_FALSE(x < prev) << "Error a[" << readed << "] > a[" << readed + 1 << "]";
				++readed, prev = x;
				return true;
			}

			std::size_t getReaded() const
			{
				return readed;
			}

		private:
			std::size_t readed, testSize;
			Animal prev;
	};

	ExternalSorter<Animal, std::less<Animal> > sorter;
	std::vector< std::pair<std::size_t, std::size_t> > tests =
	{
		{5, 1 * sizeof(Animal)}, {6, 1 * sizeof(Animal)}, {10, 2 * sizeof(Animal)},
		{100, 10 * sizeof(Animal)}, {500, 100 * sizeof(Animal)}, {100500, 16000 * sizeof(Animal)}
	};

	for (size_t i = 0; i < tests.size(); ++i)
	{
		std::cout << "test #" << i << std::endl;
		std::cout << "    size = " << tests[i].first << " mem = " << tests[i].second << " seed = " << i << std::endl;
		AnimalReader reader(tests[i].first, i);
		SortedAnimalsWriter writer(tests[i].first);
		std::clock_t start = std::clock();
		sorter.sort(tests[i].second, reader, writer, StandartSorter<Animal>());
		EXPECT_TRUE(writer.getReaded() == tests[i].first);
		std::cout << "    Sorting time = " << (clock() - start) / 1000 << "ms" << std::endl;
	}
}

TEST(ExternalSorter, StableSorting)
{
	struct Table
	{
		int legs;
		bool isOk;
		int id;

		bool realLess(const Table &b) const
		{
			if (legs != b.legs) return legs < b.legs;
			if (isOk != b.isOk) return isOk > b.isOk;
			return id > b.id;
		}

		bool operator < (const Table &t) const
		{
			if (legs != t.legs) return legs < t.legs;
			return isOk > t.isOk;
		}
	};

	class TableReader
	{
		public:
			TableReader(std::size_t n, int seed): generator(seed), unread(n) {}

			bool operator () (Table &a)
			{
				if (!unread) return false;
				--unread;
				a.legs = generator() & 3;
				a.isOk = generator() & 1;
				a.id = unread;
				return true;
			}

		private:
			std::mt19937 generator;
			std::size_t unread;
	};

	class SortedTablesWriter
	{
		public:
			SortedTablesWriter(std::size_t n): readed(0), testSize(n) {}

			bool operator() (const Table &x)
			{
				EXPECT_FALSE(readed == testSize) << " extra numbers in output";
				if (readed > 0) EXPECT_FALSE(x.realLess(prev)) << "Error a[" << readed << "] > a[" << readed + 1 << "]";
				++readed, prev = x;
				return true;
			}

			std::size_t getReaded() const
			{
				return readed;
			}

		private:
			std::size_t readed, testSize;
			Table prev;
	};

	ExternalSorter<Table, std::less<Table> > sorter;
	std::vector< std::pair<std::size_t, std::size_t> > tests =
	{
		{5, 1 * sizeof(Table)}, {6, 1 * sizeof(Table)}, {10, 2 * sizeof(Table)},
		{100, 10 * sizeof(Table)}, {500, 100 * sizeof(Table)}
	};
	for (size_t i = 0; i < tests.size(); ++i)
	{
		std::cout << "test #" << i << std::endl;
		std::cout << "    size = " << tests[i].first << " mem = " << tests[i].second << " seed = " << i << std::endl;
		TableReader reader(tests[i].first, i);
		SortedTablesWriter writer(tests[i].first);
		std::clock_t start = std::clock();
		sorter.stableSort(tests[i].second, reader, writer, StandartStableSorter<Table>());
		EXPECT_TRUE(writer.getReaded() == tests[i].first);
		std::cout << "    Sorting time = " << (clock() - start) / 1000 << "ms" << std::endl;
	}
}
