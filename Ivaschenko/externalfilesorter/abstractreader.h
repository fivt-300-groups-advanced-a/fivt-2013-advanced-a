#ifndef ABSTRACTREADER_H
#define ABSTRACTREADER_H

#include <iostream>

template <typename DataType> class AbstractReader
{
	public:
		~AbstractReader()
		{
			unbindStream();
		}

		virtual bool operator () (DataType &element) = 0;

		void bindStream(std::istream &in)
		{
			stream = &in;
		}

		void unbindStream()
		{
			stream = 0;
		}

		bool hasNext() const
		{
			return stream && *stream;
		}

		std::istream* getStream() const
		{
			return stream;
		}

	protected:
		std::istream *stream;
};

#endif // ABSTRACTREADER_H
