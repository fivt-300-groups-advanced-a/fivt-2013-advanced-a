#ifndef TESTS
#define TESTS

#include "gtest/gtest.h"
#include "Readers.h"
#include "Writers.h"
#include "mainSorter.h"
#include <vector>

struct koordinate
{
	int x,y,z;
};

TEST(InputOutputUnitTests,txtReadingtxtWriting)
{
	TxtFileWriter output("output.txt");

	for (int i=1;i<=1000;i++)
		output(i);
	output.close();

	TxtFileReader input("output.txt");
	int x;
	int i=0;
	while (input(x))
	{
		i++;
		EXPECT_EQ(i,x);
	} 
	EXPECT_EQ(i,1000);
	input.close();

	remove("output.txt");
}

TEST(InputOutputUnitTests,binaryReadingbinaryWriting)
{
	BinaryFileWriter output("1.dat");

	for (int i=-1000;i<=1000;i++)
		output(i);
	output.close();

	BinaryFileReader input("1.dat");
	int x;
	int i=-1001;
	while (input(x))
	{
		i++;
		EXPECT_EQ(i,x);
	} 
	EXPECT_EQ(i,1000);
	input.close();
	remove("1.dat");
}

TEST(InputOutputUnitTests,binaryReadingbinaryWritingFromManyFiles)
{
	for (int i=0;i<10;i++)
	{
		BinaryFileWriter write(std::to_string((long long)i));
		for (int j=0;j<100;j++)
			write(j);
		write.close();
	}

	std::vector< BinaryFileReader > files(10);
	for (int i=0;i<10;i++)
		files[i].assign(std::to_string((long long)i));
	int x;
	for (int j=0;j<100;j++)
		for (int i=0;i<10;i++)
		{
			files[i](x);
			EXPECT_EQ(j,x);
		}
	for (int i=0;i<10;i++) {
		files[i].close();
		remove(std::to_string((long long)i).c_str());
	}
}


TEST(InputOutputUnitTests,UserDataBinaryTest)
{
	koordinate u;
	BinaryFileWriter write("data.dat");
	for (int i=0;i<10;i++)
		for (int j=0;j<10;j++)
			for (int k=0;k<10;k++)
			{
				u.x=i;
				u.y=j;
				u.z=k;
				write(u);
			}
	write.close();
	BinaryFileReader read("data.dat"); 
	for (int i=0;i<10;i++)
		for (int j=0;j<10;j++)
			for (int k=0;k<10;k++)
			{
				koordinate u;
				read(u);
				EXPECT_EQ(u.x,i);
				EXPECT_EQ(u.y,j);
				EXPECT_EQ(u.z,k);
			}
	read.close();
	remove("data.dat");
}

TEST(SorterUnitTests,SortPairs)
{
	std::vector< std::pair<int,int> > data(100);
	for (int i=0;i<100;i++)
		data[i]=std::make_pair( rand()%5000, rand()%50);
	StandartSorter sort;
	std::greater< std::pair<int,int> > cmp;
	sort(data,cmp);
	for (int i=1;i<100;i++)
		ASSERT_TRUE(data[i]<data[i-1]);
}

int RandomCount;
bool Random(int &x){
	x=rand()%1000000000;
	RandomCount++;
	return RandomCount<100000;
}

TEST(mainSorterUnitTests,SortFromRandomGenerator)
{
	StandartSorter sort;
	MainSorter<int> globalSorter;
	std::less<int> cmp;
	BinaryFileWriter write("output.dat");
	RandomCount=0;
	globalSorter(1<<20,Random,write,sort,cmp);
	write.close();

	BinaryFileReader input("output.dat");
	int x,y;
	input(x);
	bool f=1;
	while (input(y)) {
		if (y<x) f=0; 
		x=y;
	}
	EXPECT_TRUE(f);
	remove("output.dat");
}

TEST(IntegrationTests,RandomDoubleFromTxtIntegrationTest)
{
	TxtFileWriter output("input.txt");
	for (int i=0;i<100000;i++)
		output(static_cast<double>(rand()) / RAND_MAX);
	output.close();

	TxtFileReader input("input.txt");
	output.assign("output.txt");
	StandartSorter sort;
	MainSorter<double> globalSorter;
	std::less<double> cmp;
	globalSorter(1<<16,input,output,sort,cmp);
	input.close();
	output.close();

	input.assign("output.txt");
	int x,y;
	input(x);
	bool f=1;
	while (input(y)) {
		if (y<x) f=0; 
		x=y;
	}
	EXPECT_TRUE(f);
	remove("input.txt");
	remove("output.txt");
}

TEST(IntegrationTests,RandomIntFromBinaryIntegrationTest)
{
	BinaryFileWriter write("input.dat");
	for (int i=0;i<100000;i++)
		write(rand()%1000000000);
	write.close();

	BinaryFileReader input("input.dat");
	BinaryFileWriter output("output.dat");
	StandartSorter sort;
	MainSorter<int> globalSorter;
	std::less<int> cmp;
	globalSorter(1<<16,input,output,sort,cmp);
	input.close();
	output.close();

	BinaryFileReader read("output.dat");
	int x,y;
	read(x);
	bool f=1;
	while (read(y)) {
		if (y<x) f=0; 
		x=y;
	}

	EXPECT_TRUE(f);
	remove("input.dat");
	remove("output.dat");
}

class koor_cmp{
public:
	bool operator()(koordinate &x,koordinate &y)
	{
		std::pair< std::pair<int,int>, int > a=std::make_pair(std::make_pair(x.x,x.y),x.z);
		std::pair< std::pair<int,int>, int > b=std::make_pair(std::make_pair(y.x,y.y),y.z);
		return a<b;
	}
};

TEST(IntegrationTests,UserBinaryDataIntegrationTest)
{
	BinaryFileWriter write("data");
	for (int i=0;i<10000;i++)
	{
		koordinate u;
		u.x=rand()%1000000000;
		u.y=rand()%1000000000;
		u.z=rand()%1000000000;
		write(u);
	}
	write.close();

	BinaryFileReader input("data");
	BinaryFileWriter output("outdata");
	StandartSorter sort;
	MainSorter<koordinate> globalSorter;
	koor_cmp cmp;
	globalSorter(1<<16,input,output,sort,cmp);
	input.close();
	output.close();

	BinaryFileReader read("outdata");
	koordinate x,y;
	read(x);
	bool f=1;
	while (read(y)) {
		if (!(cmp(x,y))) f=0; 
		x=y;
	}

	EXPECT_TRUE(f);
	remove("data");
	remove("outdata");
}

#endif