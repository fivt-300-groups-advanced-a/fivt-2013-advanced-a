#ifndef READER_H
#define READER_H

#include <iostream>

template<typename DataType> class Reader
{
	public:
		explicit Reader(std::istream &in = std::cin) 
		{
			bindStream(in);
		}
		
		void bindStream(std::istream &in)
		{
			stream = in;
		}

		void operator() (DataType &element)
		{
			stream >> element;
		}

	private:
		std::istream &stream;
};

#endif
