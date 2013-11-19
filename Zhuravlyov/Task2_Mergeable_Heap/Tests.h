#ifndef TESTS
#define TESTS

#include "unHeap.h"
#include "gtest/gtest.h"
#include "FileIO.h"
#include <vector>
#include <algorithm>


TEST(UnitTests,Insert_Merge_getMin_extractMin)
{
	BinominalyHeap<int,std::less<int> > heap1;
	BinominalyHeap<int,std::less<int> > heap2;
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
	BinominalyHeap<int,std::less<int> > heap;
	std::vector<int> mass(1000);
	for (int i=0;i<1000;i++)
		mass[i]=rand()%1000000;
	typedef BinominalyHeap<int,std::less<int>>::pointer pointer;
	std::vector<pointer> pointers(1000);
	for (int i=0;i<1000;i++)
		pointers[i]=heap.insert(mass[i]);
	for (int i=0;i<1000;i++)
	{
		int delta=rand()%100000;
		heap.change_key_on(pointers[i],-delta);
		mass[i]-=delta;
	}
	std::sort(mass.begin(),mass.end());
	for (unsigned int i=0;i<mass.size();i++)
		EXPECT_EQ(heap.extract(),mass[i]);
	EXPECT_TRUE(heap.empty()); 
}

TEST(UnitTests,Insert_Delete)
{
	BinominalyHeap<int, std::greater<int> > heap;
	std::vector<int> mass(1000);
	for (int i=0;i<1000;i++)
		mass[i]=rand()%1000000;
	typedef BinominalyHeap<int,std::greater<int>>::pointer pointer;
	std::vector<pointer> index(1000);
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
}

TEST(UnitTests,Insert_Delete_Change)
{
	BinominalyHeap<int,std::less<int> > heap;
	std::vector<int> mass(1000);
	for (int i=0;i<1000;i++)
		mass[i]=rand()%1000000;
	typedef BinominalyHeap<int,std::less<int>>::pointer pointer;
	std::vector<pointer> pointers(1000);
	for (int i=0;i<1000;i++)
		pointers[i]=heap.insert(mass[i]);
    EXPECT_TRUE(heap.check_invariants());
	heap.delete_element(pointers[5]);	
	EXPECT_TRUE(heap.check_invariants());
	heap.delete_element(pointers[500]);
	EXPECT_TRUE(heap.check_invariants());
	heap.change_key_to(pointers[100],-600);
	heap.change_key_on(pointers[0],-100000);
	heap.delete_element(pointers[0]);
	EXPECT_TRUE(heap.check_invariants());
	for (int i=0;i<1000;i++)
		if (i!=5 && i!=0 && i!=100 && i!=500)
			EXPECT_EQ(mass[i],heap.get_by_ptr(pointers[i]));
	mass[100]=-600;
	mass[0]-=100000; 
	mass.erase(mass.begin()+500);
	mass.erase(mass.begin()+5); 
	mass.erase(mass.begin());
	sort(mass.begin(),mass.end());
	for (unsigned int i=0;i<mass.size();i++)
		EXPECT_EQ(heap.extract(),mass[i]);
	EXPECT_TRUE(heap.empty()); 
}

TEST(UnitTests,Insert_Change_Merge)
{
	BinominalyHeap<int,std::less<int>> heap1;
	BinominalyHeap<int,std::less<int>> heap2;
	BinominalyHeap<int,std::less<int>> heap3;
	std::vector<int> mass(3000);
	typedef BinominalyHeap<int,std::less<int>>::pointer pointer;
	std::vector<pointer> pointers(3000);
	for (int i=0;i<3000;i++)
	{
		mass[i]=0;
		if (i%3==0) pointers[i]=heap1.insert(mass[i]);
		else if (i%3==1) pointers[i]=heap2.insert(mass[i]);
		else pointers[i]=heap3.insert(mass[i]);
	}
	EXPECT_TRUE(heap1.check_invariants());
	EXPECT_TRUE(heap2.check_invariants());
	EXPECT_TRUE(heap3.check_invariants());
	for (int i=0;i<3000;i++)
	{
		int delta=rand()%100000-rand()%100000;
		mass[i]+=delta;
		if (i%3==0) heap1.change_key_on(pointers[i],delta);
		else if (i%3==1) heap2.change_key_on(pointers[i],delta);
		else heap3.change_key_on(pointers[i],delta);
	}
	EXPECT_TRUE(heap1.check_invariants());
	EXPECT_TRUE(heap2.check_invariants());
	EXPECT_TRUE(heap3.check_invariants());
	heap2.merge(heap3);
	EXPECT_TRUE(heap2.check_invariants());
	heap1.merge(heap2);
	EXPECT_TRUE(heap1.check_invariants());

	std::sort(mass.begin(),mass.end());
	for (unsigned int i=0;i<mass.size();i++)
		EXPECT_EQ(heap1.extract(),mass[i]);
	EXPECT_TRUE(heap1.empty()); 
}

struct point
{
    int x,y,z;
};

struct point_cmp{
	bool operator()(const point &x,const point &y)
	{
	        std::pair< std::pair<int,int>, int > a=std::make_pair(std::make_pair(x.x,x.y),x.z);
            std::pair< std::pair<int,int>, int > b=std::make_pair(std::make_pair(y.x,y.y),y.z);
            return a<b;
	}
} my_point_cmp;

TEST(IntegrationTests,SampleTypeTesting)
{
	BinominalyHeap<point,point_cmp> heap1,heap2;
	typedef BinominalyHeap<point,point_cmp>::pointer pointer;
	std::vector<point> mass(1000);
	std::vector<pointer> pointers(1000);
	for (int i=0;i<1000;i++)
	{
		mass[i].x=rand()%10000;
		mass[i].y=rand()%10000;
		mass[i].z=rand()%10000;
		if (i%2) {
			pointers[i]=heap1.insert(mass[i]);
		} else pointers[i]=heap2.insert(mass[i]);
	}
	EXPECT_TRUE(heap1.check_invariants());
	EXPECT_TRUE(heap2.check_invariants());
	for (int i=0;i<1000;i++)
	{
		int delta1=rand()%1000;
		int delta2=rand()%1000;
		int delta3=rand()%1000;
		mass[i].x+=delta1;
		mass[i].y+=delta2;
		mass[i].z+=delta3;
		point temp=heap1.get_by_ptr(pointers[i]);
		temp.x+=delta1;temp.y+=delta2;temp.z+=delta3;
		if (i%2) heap1.change_key_to(pointers[i],temp);
		else heap2.change_key_to(pointers[i],temp);
	}
	EXPECT_TRUE(heap1.check_invariants());
	EXPECT_TRUE(heap2.check_invariants());
	heap1.merge(heap2);
	std::sort(mass.begin(),mass.end(),my_point_cmp);
	for (unsigned int i=0;i<mass.size();i++)
	{
		point temp=heap1.extract();
		EXPECT_EQ(temp.x,mass[i].x);
		EXPECT_EQ(temp.y,mass[i].y);
		EXPECT_EQ(temp.z,mass[i].z);
		EXPECT_TRUE(heap1.check_invariants());
	}
	EXPECT_TRUE(heap1.empty()); 
}

const int number_of_vectors=2000;
const int size_of_vector=50;

TEST(IntegrationTests,TimeCheckTesting_MakeInput)
{
	BinaryFileWriter write("filename.dat");
	for (int i=0;i<number_of_vectors*size_of_vector;i++)
		write(rand()%1000000000);
	write.close();
}

void mergeVectors(std::vector<int>& a,std::vector<int>& b)
{
	for (int i=0;i<b.size();i++)
		a.push_back(b[i]);
	std::sort(a.begin(),a.end());
}

void insert_to_vector(std::vector<int>& a,int b)
{
	std::vector<int> c;
	c.push_back(b);
	mergeVectors(a,c);
}

TEST(IntegrationTests,TimeCheckTesting_Vector)
{
	BinaryFileReader read("filename.dat");
	typedef std::vector<int> vecint;
	std::vector<vecint> vectors(number_of_vectors);
	vecint common_vector;
	for (int i=0;i<number_of_vectors;i++)
		for (int j=0;j<size_of_vector;j++) 
		{
			int x;
			read(x);
			insert_to_vector(vectors[i],x);
		}
	for (int i=0;i<number_of_vectors;i++)
		mergeVectors(common_vector,vectors[i]);
	BinaryFileWriter write("vector.res");
	for (int i=0;i<common_vector.size();i++)
		write(common_vector[i]);
	read.close();
	write.close();
}

TEST(IntegrationTests,TimeCheckTesting_Heap)
{
	BinaryFileReader read("filename.dat");
	typedef BinominalyHeap<int,std::less<int>> BinHint;
	std::vector<BinHint> heaps(number_of_vectors);
	BinHint common_heap;
	for (int i=0;i<number_of_vectors;i++)
		for (int j=0;j<size_of_vector;j++)
		{
			int x;read(x);
			heaps[i].insert(x);
		}
	for  (int i=0;i<number_of_vectors;i++)
		common_heap.merge(heaps[i]);
	BinaryFileWriter write("heap.res");
	while (!common_heap.empty())
		write(common_heap.extract());
	read.close();
	write.close();
}

TEST(IntegrationTests,TimeCheckTesting_CheckResult)
{
	BinaryFileReader Read1("vector.res");
	BinaryFileReader Read2("heap.res");
	int x,y;
	while (Read1(x) && Read2(y))
		EXPECT_EQ(x,y);
	Read1.close();
	Read2.close();
	remove("filename.dat");
	remove("vector.res");
	remove("heap.res");
}

#endif