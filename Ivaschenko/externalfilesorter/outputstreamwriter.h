#ifndef OUTPUTSTREAMWRITER_H
#define OUTPUTSTREAMWRITER_H

#include "abstractwriter.h"

template<typename DataType> class OutputStreamWriter : public AbstractWriter<DataType>
{
	public:
		explicit OutputStreamWriter(std::ostream &out = std::cout)
		{
			this->stream = &out;
		}

		bool operator () (const DataType &element)
		{
			return this->stream && (*this->stream << element);
		}
};

template<> bool OutputStreamWriter<int>::operator () (const int &x)
{
	return this->stream && (*this->stream << x << " ");
}

#endif // OUTPUTSTREAMWRITER_H
