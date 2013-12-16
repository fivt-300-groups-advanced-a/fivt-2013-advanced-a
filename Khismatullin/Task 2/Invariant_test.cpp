#include "gtest/gtest.h"
#include "Cheap.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <set>

using namespace std;

const int insert_N = 1000;
const int bigtest_N = 1000000;
const int pointtest_N = 100000;


struct point 
{
	int x, y;
};

class compare
{
	public:
	bool operator () (point a, point b)
	{
		return (a.x > b.x || (a.x == b.x && a.y < b.y));
	}
	int moon;

};

bool operator == (point a, point b)
{
	return ((a.x == b.x) && (a.y == b.y));
}

template<class Type, class Comparator> class Tester
{
	public:
		explicit Tester(Cheap<Type, Comparator>* h): heap(h), comp() {}
		Tester(Cheap<Type, Comparator>* h, Comparator &cmp): heap(h), comp(cmp) {}
		
		void change(Cheap<Type, Comparator>* h) {
			heap = h;
		}
		bool check_heap(int size)
		{
			Cvertex<Type> *cur = heap -> head;
			if (cur == NULL) {
				return (size == 0);
			}
			while (cur)
			{
				if (!check_tree(cur, NULL)) { // проверка дерева
					return false;
				}
				size -= (1 << (cur -> degree));
				if ((cur -> brother != NULL) && (cur -> brother -> degree <= cur -> degree)) { // проверка на повторяемость степени дерева, порядок следования
					return false;
				}
				cur = cur -> brother;
			}
			return (size == 0); // проверка на сумму степеней дерева и общее количество элементов
		}
		bool check_tree(Cvertex<Type>* v, Cvertex<Type>* p)
		{
			if (v -> parent != p) {
				return false;
			}
			if (v -> idx -> ptr != v) {
				return false;
			}
			Cvertex<Type> *cur = v -> child;
			if (v -> degree == 0) {
				return (cur == NULL);
			}
			if ((cur != NULL) && cur -> degree != v -> degree - 1) {
				return false;
			}
			int deg = v -> degree;
			Cvertex<Type> *last = NULL;
			for(int i = deg - 1; i >= 0; i--) {
				if (cur == NULL || cur -> degree != i) {
					return false;
				}
				//if (cur -> prev != last)
				//{
			//		return false;
			//	}
				if ((cur -> brother != NULL) && (cur -> brother -> degree + 1 != cur -> degree)) { // порядок следования сыновей
					return false;
				}
				if (!check_tree(cur, v) || comp(cur -> value, v -> value)) { // проверка на кучесть
					return false;
				}
				if (cur -> brother == NULL && cur -> degree != 0) {
					return false;
				}
				last = cur;
				cur = cur -> brother;
			}
			return true;
		}
		
	private:
		Cheap<Type, Comparator>* heap;
		Comparator comp;
};

TEST(Invariant, inserttest)
{
	for(int test = 0; test < insert_N; test++)
	{
		Cheap<int, less<int> > heap;
		Tester<int, less<int> > tester(&heap);
		int n = rand() % 10000 + 100;
		for(int i = 0; i < n; i++)
		{
			heap.insert(rand());
		}
		ASSERT_TRUE(tester.check_heap(n));
	}
}


TEST(Invariant, bigtest)
{
	int size[bigtest_N / 100];
	memset(size, 0, sizeof(size));
	Cheap<int, less<int> > h[bigtest_N / 100];
	Tester<int, less<int> > tester(&h[0]);
	multiset<int> st[bigtest_N / 100];
	int k = 0;
	
	for(int i = 0; i < bigtest_N; i++)
	{
		//cout << k << " " << size[k] << " " << i << endl;
		ASSERT_TRUE(tester.check_heap(size[k]));
		if (size[k] == 0)
		{
			int t = rand();
			h[k].insert(t);
			st[k].insert(t);
			size[k]++;
		} else {
			int c = rand() % 10;
			//cout << c << endl;
			if (c < 3) {
				int t = rand();
				h[k].insert(t);
				st[k].insert(t);
				size[k]++;
			} else {
				if (c < 5) {
					EXPECT_EQ(h[k].get_min(), (*st[k].begin()));
				} else {
					if (c < 7) {
						EXPECT_EQ(h[k].get_min(), (*st[k].begin()));
						h[k].extract_min();
						st[k].erase(st[k].begin());
						size[k]--;
					} else {
						if (c == 7) {
							k++;
							tester.change(&h[k]);
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
								tester.change(&h[k]);
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

TEST(Invariant, pointtest)
{
	int size[bigtest_N / 100];
	memset(size, 0, sizeof(size));
	Cheap<point, compare > h[bigtest_N / 100];
	Tester<point, compare > tester(&h[0]);
	multiset<point, compare> st[bigtest_N / 100];
	int k = 0;
	
	for(int i = 0; i < bigtest_N; i++)
	{
		//cout << k << " " << size[k] << " " << i << endl;
		ASSERT_TRUE(tester.check_heap(size[k]));
		if (size[k] == 0)
		{
			point t;
			t.x = rand();
			t.y = rand();
			h[k].insert(t);
			st[k].insert(t);
			size[k]++;
		} else {
			int c = rand() % 10;
			//cout << c << endl;
			if (c < 3) {
				point t;
				t.x = rand();
				t.y = rand();
				h[k].insert(t);
				st[k].insert(t);
				size[k]++;
			} else {
				if (c < 5) {
					EXPECT_EQ(h[k].get_min(), (*st[k].begin()));
				} else {
					if (c < 7) {
						EXPECT_EQ(h[k].get_min(), (*st[k].begin()));
						h[k].extract_min();
						st[k].erase(st[k].begin());
						size[k]--;
					} else {
						if (c == 7) {
							k++;
							tester.change(&h[k]);
						} else {
							if (c == 8 && k > 0) {
								k--;
								h[k].merge(h[k + 1]);
								for(multiset<point, compare>::iterator it = st[k + 1].begin(); it != st[k + 1].end(); it++) {
									st[k].insert((*it));
								}
								st[k + 1].clear();
								size[k] += size[k + 1];
								size[k + 1] = 0;
								tester.change(&h[k]);
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


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}