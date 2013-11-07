#ifndef TESTS
#define TESTS

#include "unHeap.h"
#include "gtest/gtest.h"
#include <vector>
#include <algorithm>
#include <cstdio>


TEST(UnitTests,Insert_Merge_extractMin)
{
	BinominalyHeap<int,std::less<int> > heap1;
	int mas[1000];
	for (int i=0;i<1000;i++)
		mas[i]=rand()%1000000;
	for (int i=0;i<500;i++)
		heap1.insert(mas[i]);
	EXPECT_TRUE(heap1.check_invariants());
	BinominalyHeap<int,std::less<int> > heap2;
	for (int i=500;i<1000;i++)
		heap2.insert(mas[i]);
	EXPECT_TRUE(heap2.check_invariants());
	heap1.merge(heap2);
	EXPECT_TRUE(heap1.check_invariants());
	std::sort(mas,mas+1000);
	for (int i=0;i<1000;i++)
		EXPECT_EQ(heap1.extract(),mas[i]);
	EXPECT_TRUE(heap1.empty());
	heap1.adressa.resize(0);
}

TEST(UnitTests,Insert_Delete_Change)
{
	BinominalyHeap<int,std::less<int> > heap;
	std::vector<int> mass(1000);
	for (int i=0;i<1000;i++)
		mass[i]=rand()%1000000;
	std::vector<unsigned int> pointers(1000);
	for (int i=0;i<1000;i++)
		pointers[i]=heap.insert(mass[i]);
    EXPECT_TRUE(heap.check_invariants());
	heap.delete_element(pointers[5]);	
	EXPECT_TRUE(heap.check_invariants());
	heap.delete_element(pointers[500]);
	EXPECT_TRUE(heap.check_invariants());
	heap.change_key(pointers[100],-600);
	heap.change_key(pointers[0],heap.get_by_id(pointers[0])-110);
	heap.delete_element(pointers[0]);
	EXPECT_TRUE(heap.check_invariants());
	for (int i=0;i<1000;i++)
		if (i!=5 && i!=0 && i!=100 && i!=500)
			EXPECT_EQ(mass[i],heap.get_by_id(pointers[i]));
	mass[100]=-600;
	mass[0]-=110; 
	mass.erase(mass.begin()+500);
	mass.erase(mass.begin()+5); 
	mass.erase(mass.begin());
	sort(mass.begin(),mass.end());
	for (unsigned int i=0;i<mass.size();i++)
		EXPECT_EQ(heap.extract(),mass[i]);
	EXPECT_TRUE(heap.empty()); 
	heap.adressa.resize(0);
}

TEST(UnitTests,Insert_Delete)
{
	BinominalyHeap<int, std::greater<int> > heap;
	std::vector<int> mass(1000);
	for (int i=0;i<1000;i++)
		mass[i]=rand()%1000000;
	std::vector<int> index(1000);
	for (unsigned int i=0;i<1000;i++)
		index[i]=heap.insert(mass[i]);
	EXPECT_TRUE(heap.check_invariants());
	for (unsigned int i=0;i<1000;i+=2)
		heap.delete_element(index[i]);
	EXPECT_TRUE(heap.check_invariants());
	for (unsigned int i=998;i>0;i-=2)
		mass.erase(mass.begin()+i);
	mass.erase(mass.begin());
	sort(mass.rbegin(),mass.rend());
	for (unsigned int i=0;i<mass.size();i++)
		EXPECT_EQ(mass[i],heap.extract());
	EXPECT_TRUE(heap.empty());
	heap.adressa.resize(0);
}


#endif