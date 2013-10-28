#include "gtest/gtest.h"
#include "../Heap/heap.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <algorithm>


using namespace std;

int a[1000000];

struct point 
{
	int x, y;
} b[1000000];

class ls
{
	public:
		bool operator () (point a, point b) 
		{
			return (a.x > b.x || (a.x == b.x && (a.y > b.y)));
		}
};

bool operator == (const point &a, const point &b)
{
	return (a.x == b.x && a.y == b.y);
}

int nextint() 
{
	return rand()%1000000000;
}

TEST(Integers, heapsort_1000) 
{
	Heap<int, less<int> > h(1000);
	for(int i = 0; i < 1000; i++) {
		a[i] = nextint();
		h.add(a[i]);
	}
	sort(a, a + 1000);
	bool flag = true;
	for(int i = 0; i < 1000; i++) 
	{
		flag = (flag && (a[i] == h.Extract_min()));
	}
	EXPECT_TRUE(flag);
}

TEST(Integers, heapsort_1000000) 
{
	
	Heap<int, less<int> > h(1000000);
	for(int i = 0; i < 1000000; i++) {
		a[i] = nextint();
		h.add(a[i]);
	}
	sort(a, a + 1000000);
	bool flag = true;
	for(int i = 0; i < 1000000; i++) 
	{
		flag = (flag && (a[i] == h.Extract_min()));
	}
	EXPECT_TRUE(flag);
}

TEST(Structs, heapsort_1000) 
{
	Heap<point, ls> h(1000);
	for(int i = 0; i < 1000; i++) {
		b[i].x = nextint();
		b[i].y = nextint();
		h.add(b[i]);
	}
	sort(b, b + 1000, ls());
	bool flag = true;
	for(int i = 0; i < 1000; i++) 
	{
		flag = (flag && (b[i] == h.Extract_min()));
	}
	EXPECT_TRUE(flag);
}

TEST(Structs, heapsort_200000) 
{
	Heap<point, ls> h(200000);
	for(int i = 0; i < 200000; i++) {
		b[i].x = nextint();
		b[i].y = nextint();
		h.add(b[i]);
	}
	sort(b, b + 200000, ls());
	bool flag = true;
	for(int i = 0; i < 200000; i++) 
	{
		flag = (flag && (b[i] == h.Extract_min()));
	}
	EXPECT_TRUE(flag);
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}