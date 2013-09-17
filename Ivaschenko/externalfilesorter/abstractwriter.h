#ifndef ABSTRACTWRITER_H
#define ABSTRACTWRITER_H

#include <iostream>

template <typename DataType> class AbstractWriter
{
	public:
		~AbstractWriter()
		{
			unbindStream();
		}

		virtual bool operator () (const DataType &element) = 0;

		void bindStream(std::ostream &in)
		{
			stream = &in;
		}

		void unbindStream()
		{
			stream = 0;
		}

		bool ready()
		{
			return stream && *stream;
		}

	protected:
		std::ostream *stream;
};

#endif // ABSTRACTWRITER_H
