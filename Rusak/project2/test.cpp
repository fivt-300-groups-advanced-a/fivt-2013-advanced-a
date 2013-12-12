#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>

#include "heap.h"
#include "heap_test.h"

TEST(PrepareForTest, GenerateRand) {
  srand(time(NULL));
}

TEST(UnitTests,Insert_Merge_getMin_extractMin) {
	auto heap1 = MakeHeap<int>();
	auto heap2 = MakeHeap<int>();
  const int n = 300000;
	int mas[n];
	for (int i=0;i<n;i++)
		mas[i]=rand()%10000000;
	for (int i=0;i<n/2;i++) {
		heap1.insert(mas[i]);
		heap2.insert(mas[i+n/2]);
	}
	heap1.merge(heap2);
	std::sort(mas,mas+n);
	for (int i=0;i<n;i++) {
		EXPECT_EQ(heap1.find_min(),mas[i]);
		heap1.extract_min();
	}
	EXPECT_TRUE(heap1.empty());
}

TEST(IntegrationTests, HeapSort) {
	auto heap = MakeHeap<int>();
	int n=300000;
	std::vector<int> a(n);
	for (int i=0;i<n;i++)
		a[i]=rand();
	for (int i=0;i<n;i++)
		heap.insert(a[i]);
  EXPECT_FALSE(heap.empty());
	std::sort(a.begin(),a.end());
	for (int i=0;i<n;i++) {
		EXPECT_EQ(heap.find_min(),a[i]);
	  heap.extract_min();
	}
	EXPECT_TRUE(heap.empty());
}

TEST(InsideTests, FewInvariants) {
  const int n = 4500;
  auto test = MakeHeapTest<int>();
  auto heap = MakeHeap<int>();
  for (int i=0;i<n;i++) {
    heap.insert(rand()%1000000);
    EXPECT_TRUE( test.check_inv_heap(heap) );
    EXPECT_EQ( test.size_heap(heap), i+1);
  }
  EXPECT_FALSE(heap.empty());
  for (int i=0;i<n;i++) {
    heap.extract_min();
    EXPECT_TRUE( test.check_inv_heap(heap) );
    EXPECT_EQ( test.size_heap(heap), n-i-1);
  }
  EXPECT_TRUE(heap.empty());
}

TEST(InsideTests, InvariantsAfterBigMerges) {
  const int on_each = 30000;
  const int count = 10;
  auto test = MakeHeapTest<int>();

  Heap<int, std::less<int> > heap[count];
  for (int i=0;i<count;i++) heap[i] = MakeHeap<int>();
  for (int i=0;i<on_each;i++) {
    for (int j=0;j<count;j++) {
      heap[j].insert(rand()%1000000);
    }
  }
  for (int j=count-2;j>=0;j--) {
    heap[j].merge(heap[j+1]);
    EXPECT_TRUE( test.check_inv_heap(heap[j]) );
    EXPECT_EQ( test.size_heap(heap[j]), (count-j)*on_each );
    EXPECT_TRUE(heap[j+1].empty());
  }
  EXPECT_FALSE(heap[0].empty());
}

TEST(InsideTests, StressTest) {
  const int oper = 6000;
  int col = 0;
  auto main = MakeHeap<int>();
  auto test = MakeHeapTest<int>();
  for (int op = 0; op<oper;op++) {
    int type = rand()%2;
    if (col==0) type = 0;
    if (type==0) {
      int add = rand()%10+1;
      auto added_heap = MakeHeap<int>();
      for (int j=0;j<add;j++) added_heap.insert(rand()%1000000);
      EXPECT_TRUE( test.check_inv_heap(added_heap) );
      EXPECT_EQ( test.size_heap(added_heap), add);
      main.merge(added_heap);
      col+=add;
      EXPECT_TRUE( added_heap.empty() );
    }
    else if (type==1) {
      col--;
      main.extract_min();
    }
    EXPECT_TRUE( test.check_inv_heap(main) );
    EXPECT_EQ( test.size_heap(main), col);
  }
}
