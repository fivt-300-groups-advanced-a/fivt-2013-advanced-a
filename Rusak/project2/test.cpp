#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>

#include "heap.h"
#include "heap_test.h"

TEST(PrepareForTests, GenerateRand) {
  srand(time(NULL));
}

int rand_int() {
  return rand()%((int)(1e9));
}

TEST(UnitTests, Insert_Merge_findMin_extractMin) {
  const int n = 300000;
	auto heap1 = MakeHeap<int>();
	auto heap2 = MakeHeap<int>();
	int mas[n];
	for (int i=0;i<n;i++)
		mas[i]=rand_int();
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

TEST(UnitTests, Empty) {
  const int oper = 4000;
  auto heap = MakeHeap<int>();
  auto test = MakeHeapTest<int>();
  EXPECT_EQ(test.size_heap(heap), 0);
  EXPECT_TRUE(heap.empty());
  EXPECT_TRUE(test.check_inv_heap(heap));
  for (int i=0;i<oper;i++) {
    heap.insert(rand_int()); 
    EXPECT_EQ(test.size_heap(heap), i+1);
    EXPECT_FALSE(heap.empty());
    EXPECT_TRUE(test.check_inv_heap(heap));
  }
  for (int i=0;i<oper;i++) {
    heap.extract_min();
    EXPECT_EQ(test.size_heap(heap), oper-i-1);
    if (i==oper-1) EXPECT_TRUE(heap.empty());
    else EXPECT_FALSE(heap.empty());
    EXPECT_TRUE(test.check_inv_heap(heap));
  }
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

struct Point_long_long {
  Point_long_long(long long x, long long y):x(x), y(y) {}
  Point_long_long():x(0), y(0) {}
  long long x, y;
};

struct Cmp_with_state {
  bool operator()(const Point_long_long &a, const Point_long_long &b) const {
    //manhattan distance
    long long vala = abs(fromx - a.x) + abs(fromy - a.y);
    long long valb = abs(fromx - b.x) + abs(fromy - b.y);
    return (vala < valb);
  }
  Cmp_with_state():fromx(0), fromy(0) {}
  Cmp_with_state(long long x, long long y):fromx(x), fromy(y) {}
  long long fromx, fromy;
};

TEST(IntegrationTests, ArbitaryType_CmpWithState) {
  Cmp_with_state Cmp1 = Cmp_with_state(0, 0); 
  auto heap1 = MakeHeap<Point_long_long>(Cmp1);
  
  Cmp_with_state Cmp2 = Cmp_with_state((long long)(1e9), (long long)(1e9));
  auto heap2 = MakeHeap<Point_long_long>(Cmp2);

  const int oper = 300000;
  //add some points on line y=x
  for (int i=0;i<oper;i++) {
    Point_long_long p(i, i);
    heap1.insert(p);
    heap2.insert(p);
  }
  
  //Extract all elems from both heaps
  std::vector<Point_long_long> vect1, vect2;
  while(!heap1.empty()) {
    vect1.push_back(heap1.find_min());
    heap1.extract_min();
  }
  while(!heap2.empty()) {
    vect2.push_back(heap2.find_min());
    heap2.extract_min();
  }
  EXPECT_EQ(vect1.size(), vect2.size());
  EXPECT_EQ(vect1.size(), oper);
  //Elements in vectors must have  order
  //And all points from (0, 0) to (oper, oper)
  //must be in vect1 in asc oreder
  reverse(vect2.begin(), vect2.end());
  for (int i=0;i<oper;i++) {
    EXPECT_EQ(vect1[i].x, vect2[i].x);
    EXPECT_EQ(vect1[i].y, vect2[i].y);
    EXPECT_EQ(vect1[i].x, i);
    EXPECT_EQ(vect1[i].y, i);
  }
}

TEST(InsideTests, FewInvariants) {
  const int n = 4500;
  auto test = MakeHeapTest<int>();
  auto heap = MakeHeap<int>();
  for (int i=0;i<n;i++) {
    heap.insert(rand_int());
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
      heap[j].insert(rand_int());
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
      for (int j=0;j<add;j++) added_heap.insert(rand_int());
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

void test_speed(const int oper) {
  auto heap = MakeHeap<int>();
  for (int i=0;i<oper/2;i++) heap.insert(rand_int());
  for (int i=0;i<oper/2;i++) heap.extract_min();  
}

TEST(SpeedTests, Small_10000) {
  test_speed(10000);
}

TEST(SpeedTests, Medium_300000) {
  test_speed(300000);
}

TEST(SpeedTests, Huge_5000000) {
  test_speed(5000000);
}



