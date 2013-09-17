#ifndef BINARYFILEREADER_H
#define BINARYFILEREADER_H

#include <fstream>
#include <string>

#include <cassert>

#include "abstractreader.h"

template<typename DataType> class BinaryFileReader : public AbstractReader<DataType>
{
	public:
		explicit BinaryFileReader(std::ifstream &in)
		{
			this->stream = &in;
			ownStream = false;
		}

		explicit BinaryFileReader(const std::string &fileName)
		{
			this->stream = new std::ifstream(fileName.c_str());
			ownStream = true;
		}

		explicit BinaryFileReader(const char *fileName)
		{
			this->stream = new std::ifstream(fileName);
			ownStream = true;
		}

		~BinaryFileReader()
		{
			unbindStream();
		}

		void unbindStream()
		{
			if (ownStream) delete this->stream;
			this->stream = 0;
		}

		void bindStream(std::ifstream &in)
		{
			unbindStream();
			this->stream = &in;
			ownStream = false;
		}

		void operator () (DataType &element)
		{
			if (this->hasNext())
			{
				this->stream->read(buffer, sizeof(DataType));
				memcpy(&element, buffer, sizeof(DataType));
			}
		}

		void skip(int tokens)
		{
			this->stream->seekg(tokens * sizeof(DataType), std::ios_base::cur);
		}

	private:
		bool ownStream;
		char buffer[sizeof(DataType)];
};

#endif // BINARYFILEREADER_H
