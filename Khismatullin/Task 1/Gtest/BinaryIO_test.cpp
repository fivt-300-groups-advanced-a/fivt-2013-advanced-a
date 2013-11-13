#include "gtest/gtest.h"
#include "../IO/breader.h"
#include "../IO/bwriter.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

struct point 
{
	int x, y;
};

TEST(BinaryReaderTest, Constructors_c_str)
{
	{
		ofstream fout("input.txt", ofstream::binary);
		int n = 5;
		fout.write((char*) &n, sizeof(int));
		n = 3;
		fout.write((char*) &n, sizeof(int));
		n = 7;
		fout.write((char*) &n, sizeof(int));
		fout.close();

		BinReader<int> r("input.txt");
		int a;
		ASSERT_TRUE(r(a));
		EXPECT_EQ(a, 5);
		r(a);
		EXPECT_EQ(a, 3);
		r(a);
		EXPECT_EQ(a, 7);
		EXPECT_FALSE(r(a));
	}
	remove("input.txt");
	
}
TEST(BinaryReaderTest, Constructors_string)
{	
	{
		ofstream fout("input.txt", ofstream::binary);
		int n = 5;
		fout.write((char*) &n, sizeof(int));
		n = 3;
		fout.write((char*) &n, sizeof(int));
		n = 7;
		fout.write((char*) &n, sizeof(int));
		fout.close();
		
		string s = "input.txt";
		BinReader<int> rs("input.txt");
		int a;
		ASSERT_TRUE(rs(a));
		EXPECT_EQ(a, 5);
		rs(a);
		EXPECT_EQ(a, 3);
		rs(a);
		EXPECT_EQ(a, 7);
		EXPECT_FALSE(rs(a));
	}
	remove("input.txt");
}

TEST(BinaryReaderTest, Constructors_stream)
{	
	{
		ofstream fout("input.txt", ofstream::binary);
		int n = 5;
		fout.write((char*) &n, sizeof(int));
		n = 3;
		fout.write((char*) &n, sizeof(int));
		n = 7;
		fout.write((char*) &n, sizeof(int));
		fout.close();

		ifstream fin("input.txt");
		BinReader<int> rf(fin);
		int a;
		ASSERT_TRUE(rf(a));
		EXPECT_EQ(a, 5);
		rf(a);
		EXPECT_EQ(a, 3);
		rf(a);
		EXPECT_EQ(a, 7);
		EXPECT_FALSE(rf(a));
	}
	remove("input.txt");
}

TEST(BinaryReaderTest, somestructs)
{
	ofstream fout("input.txt", ofstream::binary);
	point n;
	n.x = 1;
	n.y = 2;
	fout.write((char*) &n, sizeof(point));
	n.x = 3;
	n.y = 4;
	fout.write((char*) &n, sizeof(point));
	int x = 5;
	fout.write((char*) &n, sizeof(int));
	fout.close();
	
	BinReader<point> r("input.txt");
	point a;
	ASSERT_TRUE(r(a));
	EXPECT_EQ(a.x, 1);
	EXPECT_EQ(a.y, 2);
	r(a);
	EXPECT_EQ(a.x, 3);
	EXPECT_EQ(a.y, 4);
	EXPECT_FALSE(r(a));	
	
	remove("input.txt");
}

TEST(BinaryWriterTest, Constructors_c_str)
{	
	{	
		{
			BinWriter<int> w("output.txt");
			EXPECT_TRUE(w(5));
			EXPECT_TRUE(w(3));
			EXPECT_TRUE(w(7));
		}
		ifstream fin("output.txt", ifstream::binary);
		int x;
		ASSERT_TRUE(fin.read((char*) &x, sizeof(int)));
		EXPECT_EQ(x, 5);
		fin.read((char*) &x, sizeof(int));
		EXPECT_EQ(x, 3);
		fin.read((char*) &x, sizeof(int));
		EXPECT_EQ(x, 7);
		EXPECT_FALSE(fin.read((char*)&x, sizeof(int)));
		fin.close();
	}
	remove("output.txt");
}

TEST(BinaryWriterTest, Constructors_string)
{
	{	
		{
			string s = "output.txt";
			BinWriter<int> w(s);
			EXPECT_TRUE(w(5));
			EXPECT_TRUE(w(3));
			EXPECT_TRUE(w(7));
		}
		ifstream fin("output.txt", ifstream::binary);
		int x;
		ASSERT_TRUE(fin.read((char*) &x, sizeof(int)));
		EXPECT_EQ(x, 5);
		fin.read((char*) &x, sizeof(int));
		EXPECT_EQ(x, 3);
		fin.read((char*) &x, sizeof(int));
		EXPECT_EQ(x, 7);
		EXPECT_FALSE(fin.read((char*)&x, sizeof(int)));
		fin.close();
	}
	remove("output.txt");
}

TEST(BinaryWriterTest, Constructors_stream)
{
	{	
		{
			ofstream fout("output.txt", ofstream::binary);
			BinWriter<int> w(fout);
			EXPECT_TRUE(w(5));
			EXPECT_TRUE(w(3));
			EXPECT_TRUE(w(7));
		}
		ifstream fin("output.txt", ifstream::binary);
		int x;
		ASSERT_TRUE(fin.read((char*) &x, sizeof(int)));
		EXPECT_EQ(x, 5);
		fin.read((char*) &x, sizeof(int));
		EXPECT_EQ(x, 3);
		fin.read((char*) &x, sizeof(int));
		EXPECT_EQ(x, 7);
		EXPECT_FALSE(fin.read((char*)&x, sizeof(int)));
		fin.close();
	}
	remove("output.txt");
}

TEST(BinaryWriterTest, somestructs)
{
	{	
		BinWriter<point> w("output.txt");
		point a;
		a.x = 1;
		a.y = 2;
		EXPECT_TRUE(w(a));
		a.x = 3;
		a.y = 4;
		EXPECT_TRUE(w(a));
	}
	{
		point a;
		ifstream fin("output.txt", ifstream::binary);
		ASSERT_TRUE(fin.read((char*) &a, sizeof(point)));
		EXPECT_EQ(a.x, 1);
		EXPECT_EQ(a.y, 2);
		
		ASSERT_TRUE(fin.read((char*) &a, sizeof(point)));
		EXPECT_EQ(a.x, 3);
		EXPECT_EQ(a.y, 4);
		
		ASSERT_FALSE(fin.read((char*) &a, sizeof(point)));
		fin.close();
	}
	remove("output.txt");
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}