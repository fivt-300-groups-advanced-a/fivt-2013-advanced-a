#include "gtest/gtest.h"
#include "../IO/reader.h"
#include "../IO/writer.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

struct point 
{
	int x, y;
};

istream& operator >> (istream &in, point& a) 
{
	return (in >> a.x >> a.y);
}

ostream& operator << (ostream &out, const point& a) 
{
	return (out << a.x << " " << a.y);
}

TEST(ReaderTest, Constructors_c_str)
{
	ofstream fout("input.txt");
	fout << 5 << " " << 3 << " " << 7 << endl;
	fout.close();
	
	{
		Reader<int> r("input.txt");
		int a;
		EXPECT_TRUE(r(a));
		EXPECT_EQ(a, 5);
		r(a);
		EXPECT_EQ(a, 3);
		r(a);
		EXPECT_EQ(a, 7);
		EXPECT_FALSE(r(a));
	}
	remove("input.txt");
}

TEST(ReaderTest, Constructors_string)
{
	ofstream fout("input.txt");
	fout << 5 << " " << 3 << " " << 7 << endl;
	fout.close();
	
	{
		string s = "input.txt";
		Reader<int> rs("input.txt");
		int a;
		EXPECT_TRUE(rs(a));
		EXPECT_EQ(a, 5);
		rs(a);
		EXPECT_EQ(a, 3);
		rs(a);
		EXPECT_EQ(a, 7);
		EXPECT_FALSE(rs(a));
	}
	remove("input.txt");
}

TEST(ReaderTest, Constructors_stream)
{
	ofstream fout("input.txt");
	fout << 5 << " " << 3 << " " << 7 << endl;
	fout.close();

	{
		ifstream fin("input.txt");
		Reader<int> rf(fin);
		int a;
		EXPECT_TRUE(rf(a));
		EXPECT_EQ(a, 5);
		rf(a);
		EXPECT_EQ(a, 3);
		rf(a);
		EXPECT_EQ(a, 7);
		EXPECT_FALSE(rf(a));
	}
	remove("input.txt");
}

TEST(ReaderTest, somestructs)
{
	ofstream fout("input.txt");
	fout << 5 << " " << 3 << " " << 7 << endl;
	fout.close();
	
	{
		ofstream fout("input.txt");
		fout << 1 << " " << 2 << " " << 3 << " " << 4 << " " << 5 << endl;
		fout.close();
		Reader<point> r("input.txt");
		point a;
		EXPECT_TRUE(r(a));
		EXPECT_EQ(a.x, 1);
		EXPECT_EQ(a.y, 2);
		r(a);
		EXPECT_EQ(a.x, 3);
		EXPECT_EQ(a.y, 4);
		EXPECT_FALSE(r(a));	
	}
	remove("input.txt");
}

TEST(WriterTest, Constructors_c_str)
{	
	{	
		Writer<int> w("output.txt");
		EXPECT_TRUE(w(5));
		EXPECT_TRUE(w(3));
		EXPECT_TRUE(w(7));
		ifstream fin("output.txt");
		int x;
		fin >> x;
		EXPECT_EQ(x, 5);
		fin >> x;
		EXPECT_EQ(x, 3);
		fin >> x;
		EXPECT_EQ(x, 7);
		EXPECT_FALSE(fin >> x);
		fin.close();
	}
	remove("output.txt");
}	

TEST(WriterTest, Constructors_string)
{
	{
		string s = "output.txt";
		Writer<int> ws(s);
		EXPECT_TRUE(ws(5));
		EXPECT_TRUE(ws(3));
		EXPECT_TRUE(ws(7));
		ifstream fin("output.txt");
		int x;
		fin >> x;
		EXPECT_EQ(x, 5);
		fin >> x;
		EXPECT_EQ(x, 3);
		fin >> x;
		EXPECT_EQ(x, 7);
		EXPECT_FALSE(fin >> x);
		fin.close();
	}
	remove("output.txt");
}

TEST(WriterTest, Constructors_stream)
{
	{
		ofstream fout("output.txt");
		Writer<int> wf(fout);
		EXPECT_TRUE(wf(5));
		EXPECT_TRUE(wf(3));
		EXPECT_TRUE(wf(7));
		ifstream fin("output.txt");
		int x;
		fin >> x;
		EXPECT_EQ(x, 5);
		fin >> x;
		EXPECT_EQ(x, 3);
		fin >> x;
		EXPECT_EQ(x, 7);
		EXPECT_FALSE(fin >> x);
		fin.close();
	}
	remove("output.txt");
}

TEST(WriterTest, somestructs)
{
	{	
		Writer<point> w("output.txt");
		point a;
		a.x = 1;
		a.y = 2;
		EXPECT_TRUE(w(a));
		a.x = 3;
		a.y = 4;
		EXPECT_TRUE(w(a));
		ifstream fin("output.txt");
		ASSERT_TRUE(fin >> a.x >> a.y);
		EXPECT_EQ(a.x, 1);
		EXPECT_EQ(a.y, 2);
		
		ASSERT_TRUE(fin >> a.x >> a.y);
		EXPECT_EQ(a.x, 3);
		EXPECT_EQ(a.y, 4);
		
		ASSERT_FALSE(fin >> a.x >> a.y);
		fin.close();
	}
	remove("output.txt");
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}