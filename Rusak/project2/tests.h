#include "heap.h"
#include "gtest/gtest.h"
#include <vector>
#include <algorithm>


TEST(UnitTests,Insert_Merge_getMin_extractMin)
{
	BinomialHeap<int,std::less<int> > heap1;
	BinomialHeap<int,std::less<int> > heap2;
	int mas[1000];
	for (int i=0;i<1000;i++)
		mas[i]=rand()%1000000;
	for (int i=0;i<500;i++) {
		heap1.insert(mas[i]);
		heap2.insert(mas[i+500]);
	}
	EXPECT_TRUE(heap1.check_invariants());
	EXPECT_TRUE(heap2.check_invariants());
	heap1.merge(heap2);
	EXPECT_TRUE(heap1.check_invariants());
	std::sort(mas,mas+1000);
	for (int i=0;i<1000;i++) {
		EXPECT_EQ(heap1.getM(),mas[i]);
		EXPECT_EQ(heap1.extract(),mas[i]);
	}
	EXPECT_TRUE(heap1.empty());
}

TEST(UnitTests,Insert_Decresaing)
{
	BinomialHeap<int,std::less<int> > heap;
	std::vector<int> mass(1000);
	for (int i=0;i<1000;i++)
		mass[i]=rand()%1000000;
	typedef BinomialHeap<int,std::less<int>>::Pointer pointer;
	std::vector<pointer> pointers(1000);
	for (int i=0;i<1000;i++)
		pointers[i]=heap.insert(mass[i]);
	for (int i=0;i<1000;i++)
	{
		int delta=rand()%100000;
		heap.change_key_on(pointers[i],-delta);
		heap.check_invariants();
		mass[i]-=delta; 
	}
	for (int i=0;i<1000;i++)
		EXPECT_EQ(mass[i],heap.get_by_ptr(pointers[i]));
	heap.check_invariants();
	std::sort(mass.begin(),mass.end());
	for (unsigned int i=0;i<mass.size();i++)
		EXPECT_EQ(heap.extract(),mass[i]);
	EXPECT_TRUE(heap.empty()); 
}

TEST(IntegrationTests, HeapSort)
{
	auto heap = MakeHeap<int>();
	int n=1000000;
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
}

int number_of_operations=1000000;
TEST(StressTesting,Test)
{
	typedef BinomialHeap<int,std::less<int>> BinH;
	std::vector<BinH> heaps(2);
	typedef BinH::Pointer pointer;
	std::vector<std::vector<pointer>> ptrs(2);
	for (int i=0;i<number_of_operations;i++)
	{
		int operation=rand()%6;
		int number=rand()%100000000;
		int hi=rand()%2;
		int ptrnumber;
		if (ptrs[hi].size())
			ptrnumber=rand()%ptrs[hi].size();
		if (heaps[hi].empty()) ptrs[hi].push_back(heaps[hi].insert(number));
		else 
			if (operation==0) {
			ptrs[hi].push_back(heaps[hi].insert(number));
		} else if (operation==1) {
			if (!heaps[hi].empty()) {
				heaps[hi].extract();
				ptrs[hi].resize(0);
			}
		} 
		else if (operation==2) {
			if (ptrs[hi].size())
				if (ptrs[hi][ptrnumber])
					heaps[hi].delete_element(ptrs[hi][ptrnumber]);
		} 
		else if (operation==3) {
			if (ptrs[hi].size())
				if (ptrs[hi][ptrnumber]) 
					heaps[hi].change_key_on(ptrs[hi][ptrnumber],number);
		} else if (operation==4) {
			if (ptrs[hi].size())
				if (ptrs[hi][ptrnumber]) 
					heaps[hi].change_key_to(ptrs[hi][ptrnumber],number);
		}
		else {
			heaps[hi].merge(heaps[1-hi]);
			for (unsigned int i=0;i<ptrs[1-hi].size();i++)
				ptrs[hi].push_back(ptrs[1-hi][i]);
			ptrs[1-hi].resize(0);
		}
		heaps[0].check_invariants();
		heaps[1].check_invariants();
	}
}