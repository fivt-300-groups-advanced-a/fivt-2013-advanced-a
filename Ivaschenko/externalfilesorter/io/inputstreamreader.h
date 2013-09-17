#ifndef INPUTSTREAMREADER_H
#define INPUTSTREAMREADER_H

#include "abstractreader.h"

template<typename DataType> class InputStreamReader : public AbstractReader<DataType>
{
	public:
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			this->stream = &in;
		}

		bool operator() (DataType &element)
		{
			return this->hasNext() && (*this->stream >> element);
		}
};

#endif // INPUTSTREAMREADER_H
