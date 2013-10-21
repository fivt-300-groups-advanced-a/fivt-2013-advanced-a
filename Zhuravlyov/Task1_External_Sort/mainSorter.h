#ifndef MAINSORTER
#define MAINSORTER

#include <vector>
#include <queue>
#include <cstdio>
#include "Readers.h"
#include "Writers.h"
#include "Sorters.h"

template <class Reader,class type>
void getelements(Reader &read,std::vector<type>& data,type first,int NumberOfElements)
{
	data.clear();
	data.push_back(first);
	type q;
	for (int i=1;i<NumberOfElements && read(q);i++)
		data.push_back(q);
}

template <class type>
void temporarywriter(int filenumber,std::vector<type>& data)
{
	char FileName[9];
	itoa(filenumber,FileName,10);
	BinaryFileWriter write(FileName);
	for (int i=0;i<data.size();i++)
		write(data[i]);
	write.close();
}

template <class type,class Comparator>
class KeyPairComparator
{
public:
	bool operator()(std::pair<type,int> a,std::pair<type,int> b)
	{
		Comparator cmp;
		return cmp(b.first,a.first);
	}
};

template <class type>
class MainSorter
{
public:
	template <class Reader,class Writer,class Sorter,class Comparator>
	void operator()(int RAM,Reader &read,Writer &write,Sorter sort,Comparator cmp)
	{
		int quantityOfFiles=0;
		std::vector<type> data;
		type x;
		while (read(x)){
			quantityOfFiles++;
			getelements(read,data,x,RAM/sizeof(type));
			sort(data,cmp);
			temporarywriter(quantityOfFiles,data);
		}

		std::priority_queue< std::pair<type,int>, std::vector< std::pair<type,int> >, KeyPairComparator< type, Comparator > > heap;
		std::vector<BinaryFileReader> readers(quantityOfFiles+1);
		for (int i=1;i<=quantityOfFiles;i++)
		{
			char FileName[9];
			itoa(i,FileName,10);
			readers[i].assign(FileName);
			type first_element;
			readers[i](first_element);
			heap.push(std::make_pair(first_element,i));
		}

		while (!heap.empty())
		{
			std::pair<type,int> u=heap.top();
			write(u.first);
			int index;
			index=u.second;
			heap.pop();
			type next;
			if (readers[index](next)) 
			{ 
				heap.push(std::make_pair(next,index)); 
			}
		}
		for (int i=1;i<=quantityOfFiles;i++)
		{
			readers[i].close();
			char FileName[9];
			itoa(i,FileName,10);
			remove(FileName);
		}
	}
};
#endif