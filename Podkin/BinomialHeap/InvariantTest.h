#ifndef INVARIANTTEST_H

#define INVARIANTTEST_H

#include <vector>
#include "BinomialHeap.h"
#include "VectorHeap.h"
#include <time.h>
#include <cstdlib>
#include <gtest/gtest.h>
#include <iostream>

using namespace std;

TEST(InvariantTest, TestWithCheckHeap){
	srand(time(0));
	BinomialHeap< Point, PointComparator> my_heap;
	int cc = 0;
	PointComparator comp;
	for (int i = 0; i < 1000; i++){
		int type = rand() % 3;
		if (type == 0){
			int x = rand() % 100000, y = rand() % 100000;
			Point point(x, y);
			cc++;
			my_heap.insert(point);
			EXPECT_TRUE(my_heap.checkHeap());
		}
		if (type == 1){
			if (cc == 0)
				continue;
			cc--;
			my_heap.extractMin();
			EXPECT_TRUE(my_heap.checkHeap());
		}
		if (type == 2){
			cc = 0;
			my_heap.clearHeap();
		}
	}
}

#endif