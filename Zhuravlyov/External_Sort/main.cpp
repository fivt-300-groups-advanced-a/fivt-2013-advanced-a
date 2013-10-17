#include "Writers.h"
#include "Readers.h"
#include "Sorters.h"
#include "mainSorter.h"
void test_genere()
{
	TxtFileWriter output("input.txt");
	for (int i=0;i<1000000;i++)
		output(static_cast<double>(rand()) / RAND_MAX);
	output.close();
}

void test_check()
{
	TxtFileReader input("output.txt");
	int x,y;
	input(x);
	bool f=1;
	while (input(y)) {
		if (y<x) f=0; 
		x=y;
	}
	if (f) std::cout << "OK";
	else std::cout << "BAD";
	input.close();
}
int main()
{
	test_genere();
	
	TxtFileReader input("input.txt");
	TxtFileWriter output("output.txt");
	StandartSorter sort;
	MainSorter<double> globalSorter;
	std::less<double> cmp;
	globalSorter(1<<16,input,output,sort,cmp);
	input.close();
	output.close();

	test_check();
	getchar();
}