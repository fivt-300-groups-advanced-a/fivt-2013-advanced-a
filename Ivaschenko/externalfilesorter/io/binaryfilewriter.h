#ifndef BINARYFILEWRITER_H
#define BINARYFILEWRITER_H

#include "abstractwriter.h"

template <typename DataType> class BinaryFileWriter : public AbstractWriter<DataType>
{
	public:
		explicit BinaryFileWriter(std::ofstream &out)
		{
			this->stream = &out;
			ownStream = false;
		}

		explicit BinaryFileWriter(const std::string &fileName)
		{
			this->stream = new std::ofstream(fileName.c_str());
			ownStream = true;
		}

		explicit BinaryFileWriter(const char *fileName)
		{
			this->stream = new std::ofstream(fileName);
			ownStream = true;
		}

		void unbindStream()
		{
			if (ownStream) delete this->stream;
			this->stream = 0;
		}

		void bindStream(std::ofstream &out)
		{
			unbindStream();
			this->stream = &out;
			ownStream = false;
		}

		bool operator () (const DataType &element)
		{
			return this->ready() && this->stream->write((const char *) &element, sizeof(DataType));
		}


	private:
		bool ownStream;
};

#endif // BINARYFILEWRITER_H
