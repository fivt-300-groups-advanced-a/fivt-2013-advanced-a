#ifndef BINARY_FILE_READER_H
#define BINARY_FILE_READER_H

#include <fstream>

template<typename DataType> class BinaryFileReader
{
	public:
		explicit BinaryFileReader(std::ifstream &in)
		{
			bindStream(in);
		}
		
		void bindStream(std::ifstream &in)
		{
			stream = in;
		}
		
		void operator() (DataType &element)
		{
			stream.read(buffer, sizeof(DataType));
			memcpy(&element, buffer, sizeof(DataType));
		}
		
	private:
		std::ifstream &stream;
		char buffer[sizeof(DataType)];
};

#endif
