#ifndef ABSTRACTREADER_H
#define ABSTRACTREADER_H

#include <iostream>

template <typename DataType> class AbstractReader
{
	public:
		~AbstractReader()
		{
			if (stream) delete stream;
		}

		virtual void operator () (DataType &element) = 0;

		bool hasNext() const
		{
			return stream && *stream;
		}

	protected:
		std::istream *stream;
};

#endif // ABSTRACTREADER_H
