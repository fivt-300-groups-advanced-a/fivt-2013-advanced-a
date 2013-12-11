#include "heap.h"
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

TEST(UnitTests,Insert_Merge_getMin_extractMin)
{
	auto heap1 = MakeHeap<int>();
	auto heap2 = MakeHeap<int>();
	int mas[1000];
	for (int i=0;i<1000;i++)
		mas[i]=rand()%10000000;
	for (int i=0;i<500;i++) {
		heap1.insert(mas[i]);
		heap2.insert(mas[i+500]);
	}
	heap1.merge(heap2);
	std::sort(mas,mas+1000);
	for (int i=0;i<1000;i++) {
		EXPECT_EQ(heap1.find_min(),mas[i]);
		heap1.extract_min();
	}
	EXPECT_TRUE(heap1.empty());
}

TEST(IntegrationTests, HeapSort)
{
	auto heap = MakeHeap<int>();
	int n=300000;
	std::vector<int> a(n);
	for (int i=0;i<n;i++)
		a[i]=rand();
	for (int i=0;i<n;i++)
		heap.insert(a[i]);
	std::sort(a.begin(),a.end());
	for (int i=0;i<n;i++) {
		EXPECT_EQ(heap.find_min(),a[i]);
	  heap.extract_min();
	}
	EXPECT_TRUE(heap.empty());
}
