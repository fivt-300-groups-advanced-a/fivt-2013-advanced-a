#ifndef SORTERS
#define SORTERS

#include <algorithm>
#include <functional>
#include <vector>

class StandartSorter {
public:
	template <class type,class Comparator>
	void operator()(std::vector<type>& data,Comparator cmp=std::less<type>)
	{
		std::sort(data.begin(),data.end(),cmp);
	}
};

// Digital Sorter not ready yet
/*
template<class type> 
int get_two_bytes(type element,int index) {
	return (element>>(index*16))%(1<<16);
};

template<class type> class SignedIntExtractor {
public:
	int size() {return sizeof(type)*2;}
	int getbyte(type element,type index) {
		unsigned type x=element+(1<<(sizeof(type)*8-1));
		return get_two_bytes(x,index);
	}
	const int maxkey=1<<16;
};

template <class type,class Extractor> class DigitalSorter{
public:
	//Extractor Must Have Two Methods: size() (returns maximal number of elementary elements)
	//and getbyte(element,index) (returns elementary element for index)
	void operator()(std::vector<type>& data)
	{
		Extractor OurExtractor;
		const int maxkey=OurExtractor.maxkey();
		int keys[maxkey];
		std::vector<type> data1(data.size());
		for (int i=0;i<OurExtractor.size();i++) {
			for (int j=0;i<maxkey;i++) keys[j]=0;
			for (int q=0;q<data.size();q++)
				keys[OurExtractor.getbyte(data[q],i)]++;
			for (int j=1;j<maxkey;j++)
				keys[j]=keys[j]+keys[j-1];
			for (int q=data.size()-1;q>=0;q--)
			{
				int d=OurExtractor.getbyte(data[q],i);
				data1[--keys[d]]=data[q];
			}
		}
		data=data1;
	}
};
*/

#endif