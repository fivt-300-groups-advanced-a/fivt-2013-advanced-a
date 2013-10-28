#include "gtest/gtest.h"
#include "../Filesort/filesort.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

ofstream output;
ifstream input;

struct point 
{
	int x, y;
};

class ls
{
	public:
		bool operator () (point a, point b) 
		{
			return (a.x > b.x || (a.x == b.x && (a.y > b.y)));
		}
};

istream& operator >> (istream &in, point& a) 
{
	return (in >> a.x >> a.y);
}

ostream& operator << (ostream &out, const point& a) 
{
	return (out << a.x << " " << a.y);
}

bool operator <= (const point &a, const point &b)
{
	return (a.x > b.x || (a.x == b.x && (a.y >= b.y)));
}

void bubblesort(vector<int> &v)  
{
			for(int i = 0; i < v.size(); i++) {
				for(int j = 0; j < v.size() - i - 1; j++) {
					if (v[j] > v[j + 1]) {
						swap(v[j + 1], v[j]);
					}
				}
			}
}

void bubblesort(vector<point> &v)  
{
			ls cmp;
			for(int i = 0; i < v.size(); i++) {
				for(int j = 0; j < v.size() - i - 1; j++) {
					if (cmp(v[j + 1], v[j])) {
						swap(v[j + 1], v[j]);
					}
				}
			}
}

void generateints(const char* str, int count) 
{
	output.close();
	output.open("input.txt");
	for(int i = 0; i < count; i++) {
		output << rand() % 1000000000 << " ";
	}
}

template<class T>
bool check(int n) 
{
	T a;
	input.close();
	input.open("output.txt");
	input >> a;
	for(int i = 1; i < n; i++) {
		T b;
		input >> b;
		if (!(a <= b)) {
			return false;
		}
	}
	return true;
}

TEST(Integers, standard_1000) 
{
	generateints("input.txt", 1000);
	filesort<int, less<int> >("input.txt", "output.txt", 40000);
	EXPECT_TRUE(check<int>(1000));
}


TEST(Integers, standard_1000000) 
{
	generateints("input.txt", 1000000);
	filesort<int, less<int> >("input.txt", "output.txt", 40000);
	EXPECT_TRUE(check<int>(1000000));
}


TEST(Integers, bubblesort_1000) 
{
	generateints("input.txt", 1000);
	filesort<int, less<int> >("input.txt", "output.txt", 40000, bubblesort);
	EXPECT_TRUE(check<int>(1000));
}

TEST(Integers, small_memory) 
{
	generateints("input.txt", 10000);
	filesort<int, less<int> >("input.txt", "output.txt", 500);
	EXPECT_TRUE(check<int>(10000));
}

TEST(Integers, big_test) 
{
	generateints("input.txt", 10000000);
	filesort<int, less<int> >("input.txt", "output.txt", 1000000);
	EXPECT_TRUE(check<int>(10000000));
}

TEST(Structs, standard_1000) 
{
	generateints("input.txt", 2000);
	filesort<point, ls>("input.txt", "output.txt", 40000);
	EXPECT_TRUE(check<point>(1000));
}


TEST(Structs, standard_200000) 
{
	generateints("input.txt", 400000);
	filesort<point, ls>("input.txt", "output.txt", 40000);
	EXPECT_TRUE(check<point>(200000));
}


TEST(Structs, bubblesort_1000) 
{
	generateints("input.txt", 2000);
	filesort<point, ls>("input.txt", "output.txt", 40000, bubblesort);
	EXPECT_TRUE(check<point>(1000));
}

TEST(Structs, small_memory) 
{
	generateints("input.txt", 20000);
	filesort<point, ls>("input.txt", "output.txt", 500);
	EXPECT_TRUE(check<point>(10000));
	input.close();
	output.close();
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}