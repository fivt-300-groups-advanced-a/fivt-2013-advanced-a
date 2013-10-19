#ifndef SORTER

#define SORTER

#include<vector>
#include <functional>

#include "usuallysort.h"
#include "digitalsort.h"

template<class T, class comp>
void head_sort(std :: vector<T> &v)
{
	comp cmp;
	usually_sort(v, cmp);
}

template<>
void head_sort<unsigned int, std::less<unsigned int> >(std::vector<unsigned int> &v)
{
	digital_sort(v);
}

template<>
void head_sort<int, std::less<int> >(std::vector<int> &v)
{
	for (std::size_t i = 0; i < v.size(); i++) 
	{
		if (v[i] < 0) 
		{
			usually_sort(v, std::less<int>());
			return;
		}
	}
	digital_sort(v);
}

template<>
void head_sort<long long, std::less<long long> >(std::vector<long long> &v)
{
	for (std::size_t i = 0; i < v.size(); i++) 
	{
		if (v[i] < 0) 
		{
			usually_sort(v, std::less<long long>());
			return;
		}
	}
	digital_sort(v);
}

template<>
void head_sort<unsigned char, std::less<unsigned char> >(std::vector<unsigned char> &v)
{
	digital_sort(v);
}

template<>
void head_sort<unsigned long long, std::less<unsigned long long> >(std::vector<unsigned long long> &v)
{
	digital_sort(v);
}

template<>
void head_sort<unsigned short, std::less<unsigned short> >(std::vector<unsigned short> &v)
{
	digital_sort(v);
}

#endif