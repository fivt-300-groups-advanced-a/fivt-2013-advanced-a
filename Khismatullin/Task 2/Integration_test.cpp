#include "gtest/gtest.h"
#include "Cheap.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

struct point 
{
	int x, y;
};

class compare
{
	public:
	explicit compare(int m): moon(m) {}
	compare(): moon(0) {}
	bool operator () (point a, point b)
	{
		if (moon)
			return (a.x > b.x || (a.x == b.x && a.y < b.y));
		else {
			return (a.x < b.x || (a.x == b.x && a.y < b.y));
		}
	}
	int moon;

} cmp;

bool operator == (point a, point b)
{
	return (a.x == b.x && a.y == b.y);
}

TEST(Integration_test, INT)
{
	int size[10000];
	Cheap<int, less<int> > h[10000];
	multiset<int> st[10000];
	vector< Cheap<int, less<int> >::index > v[10000];
	int k = 0;
	for(int i = 0; i < 1000000; i++)
	{
		if (size[k] == 0)
		{
			int t = rand();
			v[k].push_back(h[k].insert(t));
			st[k].insert(t);
			size[k]++;
		} else {
			int c = rand() % 10;
			//cout << i << " "  < c << endl;
			if (c < 3) {
				int t = rand();
				v[k].push_back(h[k].insert(t));
				st[k].insert(t);
				size[k]++;
			} else {
				if (c < 5) {
					EXPECT_EQ(h[k].get_min(), (*st[k].begin()));
				} else {
					if (c < 7) {
						EXPECT_EQ(h[k].get_min(), (*st[k].begin()));
						Cheap<int, less<int> >::index it = h[k].extract_min();
						int mini, ans = 2147000000;
						st[k].erase(st[k].begin());
						for(int i = 0; i < v[k].size(); i++) {
							if (it -> get_value() < ans) {
								mini = i;
								break;
							}
						}
						size[k]--;
					} else {
						if (c == 7) {
							k++;
						} else {
							if (c == 8 && k > 0) {
								k--;
								h[k].merge(h[k + 1]);
								for(multiset<int>::iterator it = st[k + 1].begin(); it != st[k + 1].end(); it++) {
									st[k].insert((*it));
								}
								st[k + 1].clear();
								size[k] += size[k + 1];
								size[k + 1] = 0;
							} else {
								h[k].clear();
								st[k].clear();
								size[k] = 0;
							}
						}
					}
				}
			}
		}
	}
}

TEST(Integration_test, My_struct)
{
	int size = 0;
	Cheap<point, compare> h[10000];
	multiset<point, compare> st[10000];
	int k = 0;
	for(int i = 0; i < 100000; i++)
	{
		if (size == 0)
		{
			point t;
			t.x = rand();
			t.y = rand();
			h[k].insert(t);
			st[k].insert(t);
		} else {
			int c = rand() % 10;
			if (c < 3) {
				point t;
				t.x = rand();
				t.y = rand();
				h[k].insert(t);
				st[k].insert(t);
			} else {
				if (c < 5) {
					EXPECT_TRUE(h[k].get_min() == (*st[k].begin()));
				} else {
					if (c < 7) {
						EXPECT_TRUE(h[k].get_min() == (*st[k].begin()));
						h[k].extract_min();
						st[k].erase(st[k].begin());
					} else {
						if (c == 7) {
							k++;
						} else {
							if (c == 8 && k > 0) {
								k--;
								h[k].merge(h[k + 1]);
								for(multiset<point>::iterator it = st[k + 1].begin(); it != st[k + 1].end(); it++) {
									st[k].insert((*it));
								}
								st[k + 1].clear();
							} else {
								h[k].clear();
							}
						}
					}
				}
			}
		}
	}
}


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}