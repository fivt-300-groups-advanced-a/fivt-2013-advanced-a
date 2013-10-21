#include <gtest/gtest.h>

#include "io/binaryfilereader.h"

#include <cstdio>

#include <fstream>
#include <vector>
#include <memory>

TEST(BinaryFileReader, ReadingIntegers)
{
	const char *fileName = "testFile.txt";
	std::vector< std::vector<int> > tests = {{0, 1, 58, 185, 294, 1945, -395}, {193, 2941, -1941}, {}};
	for (auto test : tests)
	{
		FILE *f = fopen(fileName, "wb");
		for (auto x : test)
			std::fwrite(&x, sizeof(x), 1, f);
		fclose(f);
		std::unique_ptr<std::ifstream> in(new std::ifstream(fileName, std::ios_base::binary));
		BinaryFileReader<int> reader(*in);
		int current;
		for (int x : test)
		{
			ASSERT_TRUE(reader(current));
			EXPECT_EQ(x, current);
		}
		ASSERT_FALSE(reader(current));
	}
	unlink(fileName);
}

TEST(BinaryFileReader, ReadingStructures)
{
	struct myPair
	{
		int x;
		long long y;

		myPair() {}
		myPair(int nx, long long ny): x(nx), y(ny) {}

		bool operator == (const myPair &p) const
		{
			return x == p.x && y == p.y;
		}
	};

	const char *fileName = "testFile.txt";
	std::vector< std::vector<myPair> > tests =
	{
		{myPair(1, 49), myPair(1 << 30, 1LL << 60), myPair(-184, 285), myPair(943000, -192919193292LL)},
		{myPair(5, 5), myPair(5, 5), myPair(5, 6)}
	};
	for (auto test : tests)
	{
		FILE *f = fopen(fileName, "wb");
		for (auto x : test)
			std::fwrite(&x, sizeof(x), 1, f);
		fclose(f);
		std::unique_ptr<std::ifstream> in(new std::ifstream(fileName, std::ios_base::binary));
		BinaryFileReader<myPair> reader(*in);
		myPair current;
		for (auto x : test)
		{
			ASSERT_TRUE(reader(current));
			EXPECT_EQ(x, current);
		}
		ASSERT_FALSE(reader(current));
	}
	unlink(fileName);
}

TEST(BinaryFileReader, ReadingContainers)
{
	const char *fileName = "testFile.txt";
	std::vector< std::vector<std::string> > tests =
	{
		{"aas"},
		{"194", "28458", "dslkf", "8f=sjsddk", "|}#@!($||DSAjsvss9"},
		{"sdfo", "aaaaa", "asdj93", "9tge0j"}
	};
	for (auto test : tests)
	{
		FILE *f = fopen(fileName, "wb");
		for (auto x : test)
		{
			std::size_t sz = x.size();
			std::fwrite(&sz, sizeof(x.size()), 1, f);
			std::fwrite(x.c_str(), sizeof(char), sz, f);
		}
		fclose(f);
		BinaryFileReader<std::string> reader(fileName);
		std::string current;
		for (auto x : test)
		{
			ASSERT_TRUE(reader(current));
			EXPECT_EQ(x, current);
		}
		ASSERT_FALSE(reader(current));
	}
	unlink(fileName);
}
