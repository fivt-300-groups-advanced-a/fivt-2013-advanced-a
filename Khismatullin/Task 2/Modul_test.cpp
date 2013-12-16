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
			return (a.x > b.x || (a.x == b.x && a.y > b.y));
		else {
			return (a.x < b.x || (a.x == b.x && a.y < b.y));
		}
	}
	void change(int _new){
		moon = _new;
	}
	int moon;

} cmp(0);

bool operator == (pair<int, int> a, point b)
{
	return (a.first == b.x && a.second == b.y);
}

TEST(Insert, Manual_test)
{
	Cheap<int, less<int> > heap;
	heap.insert(1);
	heap.insert(1);
	heap.insert(1);
	heap.insert(1);
}

TEST(Insert, some_big_insert)
{
	Cheap<int, less<int> > heap;
	for(int i = 0; i < 2000000; i++)
	{
		heap.insert(rand());
	}
}

TEST(Get_min, Manual_test)
{
	Cheap<int, less<int> > heap;
	heap.insert(3);
	heap.insert(2);
	heap.insert(1);
	heap.insert(0);
	EXPECT_EQ(0, heap.get_min());
}

TEST(Get_min, decreasing_sequence)
{
	Cheap<int, less<int> > heap;
	for(int i = 1024; i > 0; i--)
	{
		heap.insert(i);
		EXPECT_EQ(i, heap.get_min());
	}
}

TEST(Get_min, increasing_sequence)
{
	Cheap<int, less<int> > heap;
	for(int i = 0; i < 123; i++)
	{	
		heap.insert(i);
		EXPECT_EQ(0, heap.get_min());
	}
}

TEST(Get_min, random_sequence)
{
	Cheap<int, less<int> > heap;
	set<int> st;
	for(int i = 0; i < 1000; i++)
	{	
		int t = rand();
		st.insert(t);
		heap.insert(t);
		EXPECT_EQ((*st.begin()), heap.get_min());
	}
}

TEST(Extract_min, Manual_test)
{
	Cheap<int, less<int> > heap;
	heap.insert(1);
	heap.insert(7);
	heap.insert(6);
	heap.insert(5);
	EXPECT_EQ(heap.get_min(), 1);
	heap.extract_min();
	heap.insert(2);
	heap.insert(3);
	heap.insert(4);
	heap.insert(5);
	heap.insert(4);
	EXPECT_EQ(heap.get_min(), 2);
	heap.extract_min();
}

TEST(Extract_min, increasing_sequence)
{
	Cheap<int, less<int> > heap;
	for(int i = 1024; i > 0; i--)
	{
		heap.insert(i);
	}
	for(int i = 1; i <= 1024; i++) {
		EXPECT_EQ(heap.get_min(), i);
		heap.extract_min();
	}
}

TEST(Extract_min, random_big_sequence)
{
	int n = 4000;
	Cheap<int, less<int> > heap;
	multiset<int> st;
	for(int i = 0; i < n; i++)
	{
		int t = rand();
		st.insert(t);
		heap.insert(t);
	}
	for(int i = 0; i < n; i++) {
		EXPECT_EQ(heap.get_min(), (*(st.begin())));
		heap.extract_min();
		st.erase(st.begin());
	}
}

TEST(Merge, Manual_test)
{
	Cheap<int, less<int> > heap[3];
	for(int i = 0; i < 3; i++)
	{
		heap[0].insert(i);
		heap[1].insert(i + 3);
	}
	for(int i = 0; i < 7; i++)
		heap[2].insert(i + 6);
	heap[0].merge(heap[1]);
	heap[1].merge(heap[2]);
	heap[0].merge(heap[1]);
	for(int i = 0; i < 13; i++)
	{
		EXPECT_EQ(heap[0].get_min(), i);
		heap[0].extract_min();
	}
	
}

TEST(Merge, ten_heaps_modulo_ten)
{
	Cheap<int, less<int> > heap[10];
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 500; j++)
		{
			heap[i].insert(j * 10 + i);
		}
	}
	for(int i = 1; i < 10; i++)
	{
		heap[0].merge(heap[i]);
	}
	for(int i = 0; i < 5000; i++)
	{
		EXPECT_EQ(heap[0].get_min(), i);
		heap[0].extract_min();
	}
}

TEST(Merge, groups_of_integers)
{
	Cheap<int, less<int> > heap;
	int i = 1000; 
	while (i > 0)
	{
		int l = i;
		heap.insert(i);
		i--;
		int t = min(rand() % 10, i);
		Cheap<int, less<int> > tmp;
		for(; t > 0; t--, i--)
		{
			tmp.insert(i);
			EXPECT_EQ(tmp.get_min(), i);	
			EXPECT_EQ(heap.get_min(), l);	
		}
		heap.merge(tmp);
		EXPECT_EQ(heap.get_min(), i + 1);
		
	}
}


TEST(Clear, Manual_test)
{
	Cheap<int, less<int> > heap;
	heap.insert(1);
	heap.clear();
	heap.insert(2);
	EXPECT_EQ(heap.get_min(), 2);
}

TEST(Clear, groups)
{
	Cheap<int, less<int> > heap;
	int last = -1;
	for(int i = 0; i < 1000; i++)
	{
		if (last != -1) {
			EXPECT_EQ(heap.get_min(), last);
		}
		if (rand() % 4 == 0) {
			heap.clear();
			last = -1;
		} else {
			if (last == -1) {
				last = i;
			}
			heap.insert(i);
		}
	}
}

TEST(My_Comporator, random_sequence)
{
	int n = 4000;
	{
		Cheap<point, compare > heap(cmp);
		multiset<pair<int, int> > st;
		for(int i = 0; i < n; i++)
		{
			point t;
			t.x = rand();
			t.y = rand();
			st.insert(make_pair(t.x, t.y));
			heap.insert(t);
		}
		for(int i = 0; i < n; i++) {
			EXPECT_TRUE((*(st.begin())) == heap.get_min());
			heap.extract_min();
			st.erase(st.begin());
		}
	}
	cmp.change(1);
	Cheap<point, compare > heap(cmp);
	multiset<pair<int, int> > st;
		
	for(int i = 0; i < n; i++)
	{
		point t;
		t.x = rand();
		t.y = rand();
		st.insert(make_pair(-t.x, -t.y));
		heap.insert(t);
	}
	for(int i = 0; i < n; i++) {
		pair<int, int> tmp = make_pair(-(*(st.begin())).first, -(*(st.begin())).second);
		EXPECT_TRUE(tmp == heap.get_min());
		heap.extract_min();
		st.erase(st.begin());
	}
	
}

TEST(Decrease_key, Manual_test)
{
	Cheap<int, less<int> > heap;
	heap.insert(2);
	heap.insert(2);
	heap.insert(2);
	heap.decrease_key(heap.insert(2), 1);
	EXPECT_EQ(1, heap.get_min());
}

TEST(Decrease_key, some_big_test)
{
	Cheap<int, less<int> > heap;
	Cheap<int, less<int> > ::index a[1024];
	for(int i = 0; i < 1024; i++) {
		a[i] = heap.insert(i + 1024);
	}
	for(int i = 1023; i >= 0; i--) {
		heap.decrease_key(a[i], i);
	}
	for(int i = 0; i < 1024; i++) {
		EXPECT_EQ(heap.get_min(), i);
		heap.extract_min();
	}
}

TEST(Erase, Manual_test)
{
	Cheap<int, less<int> > heap;
	heap.insert(2);
	heap.insert(2);
	Cheap<int, less<int> > ::index id = heap.insert(1);
	heap.erase(id, -2);
	EXPECT_EQ(2, heap.get_min());
}

TEST(Decrease_key, some_big_test)
{
	Cheap<int, less<int> > heap;
	Cheap<int, less<int> > ::index a[1024];
	for(int i = 0; i < 1024; i++) {
		a[i] = heap.insert(i + 1024);
	}
	for(int i = 1023; i >= 0; i--) {
		heap.decrease_key(a[i], i);
	}
	for(int i = 0; i < 1024; i++) {
		EXPECT_EQ(heap.get_min(), i);
		heap.extract_min();
	}
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}