#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <map>

template<typename DataType> class Reader
{
	public:
		explicit Reader(std::istream &in = std::cin);
			
		void operator() (DataType &);
	private:
		std::istream &in;
};
