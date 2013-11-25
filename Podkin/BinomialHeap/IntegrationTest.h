#ifndef CHECKHEAP_H

#define CHECKHEAP_H

#include <vector>
#include "BinomialHeap.h"
#include "VectorHeap.h"
#include <time.h>
#include <cstdlib>
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

class Point{
	public:
		Point(){
		}
		explicit Point(int a, int b){
			x = a;
			y = b;
		}
		int x, y;
};

class PointComparator{
	public:
		PointComparator(){
		}
		bool operator ()(Point a, Point b){
			return comp(b.x, a.x) || (!comp(b.x, a.x) && !comp(a.x, b.x) && comp(b.y, a.y));
		}
        std::less<int> comp;
};

TEST(IntegrationTest, TestStandartType){
	srand(time(0));
	BinomialHeap<int, std::less<int> > my_heap;
	VectorHeap<int, std::less<int> > vector_heap;
	int cc = 0;
	for (int i = 0; i < 1000; i++){
		int type = rand() % 3;
        if (type == 0){
			int x = rand() % 100000;
			cc++;
			my_heap.insert(x);
			vector_heap.insert(x);
			EXPECT_EQ(my_heap.getMin(), vector_heap.getMin());
		}
		if (type == 1){
			if (cc == 0)
				continue;
			cc--;
			my_heap.extractMin();
			vector_heap.extractMin();
			EXPECT_EQ(my_heap.getMin(), vector_heap.getMin());
		}
		if (type == 2){
			cc = 0;
			my_heap.clearHeap();
			vector_heap.clearHeap();
		}
	}
}

TEST(IntegrationTest, TestMyType){
	srand(time(0));
	BinomialHeap< Point, PointComparator> my_heap;
	VectorHeap< Point, PointComparator> vector_heap;
	int cc = 0;
	PointComparator comp;
	for (int i = 0; i < 1000; i++){
		int type = rand() % 3;
		if (type == 0){
			int x = rand() % 100000, y = rand() % 100000;
			Point point(x, y);
			cc++;
			my_heap.insert(point);
			vector_heap.insert(point);
			EXPECT_TRUE(!comp(my_heap.getMin(), vector_heap.getMin()) && !comp(vector_heap.getMin(), my_heap.getMin()));
		}
		if (type == 1){
			if (cc == 0)
				continue;
			cc--;
			my_heap.extractMin();
			vector_heap.extractMin();
			EXPECT_TRUE(!comp(my_heap.getMin(), vector_heap.getMin()) && !comp(vector_heap.getMin(), my_heap.getMin()));
		}
		if (type == 2){
			cc = 0;
			my_heap.clearHeap();
			vector_heap.clearHeap();
		}
	}
}

#endif
