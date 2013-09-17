#ifndef INPUTSTREAMREADER_H
#define INPUTSTREAMREADER_H

#include <iostream>

#include <cstring>

#include "abstractreader.h"

template<typename DataType> class InputStreamReader : public AbstractReader<DataType>
{
	public:
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			this->stream = &in;
		}
		
		void unbindStream()
		{
			this->stream = 0;
		}

		void bindStream(std::istream &in)
		{
			this->stream = in;
		}

		void operator() (DataType &element)
		{
			if (this->hasNext()) this->stream >> element;
		}
};

#endif
