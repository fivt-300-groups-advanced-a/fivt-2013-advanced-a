#include "gtest/gtest.h"
#include "../RAMsort/digitalsort.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <algorithm>


using namespace std;

int a[1000];

vector<int> v;

unsigned long long nextlong() 
{
	unsigned long long a = 0;
	for(int i = 0; i < 64; i++) {
		a += (1LL << i) * (rand()&1);
	}
	return a;
}

unsigned int nextuint() 
{
	unsigned int a = 0;
	for(int i = 0; i < 32; i++) {
		a += (1 << i) * (rand()&1);
	}
	return a;

}

int nextint() 
{
	return rand()%2147000000;
}

TEST(Int, small) 
{
	for(int i = 0; i < 1000; i++) {
		a[i] = nextint();
		v.push_back(a[i]);
	}
	sort(a, a + 1000);
	digital_sort<int>(v);
	bool flag = true;
	for(int i = 0; i < 1000; i++) 
	{
		flag = (flag && (a[i] == v[i]));
	}
	EXPECT_TRUE(flag);
	v.clear();
}

TEST(Int, big) 
{
	for(int i = 0; i < 10000; i++) {
		a[i] = nextint();
		v.push_back(a[i]);
	}
	sort(a, a + 10000);
	digital_sort<int>(v);
	bool flag = true;
	for(int i = 0; i < 10000; i++) 
	{
		flag = (flag && (a[i] == v[i]));
	}
	EXPECT_TRUE(flag);
	v.clear();
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}