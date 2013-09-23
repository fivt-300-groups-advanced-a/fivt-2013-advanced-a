#ifndef BINARYFILEREADER_H
#define BINARYFILEREADER_H

#include <fstream>
#include <string>

#include <cassert>

#include "abstractreader.h"

/**
 * Class implementing AbstractReader interface
 * Used to read fixed-type data in binary format from file input stream
 */
template<typename DataType> class BinaryFileReader : public AbstractReader<DataType>
{
	public:
		/**
		 * Initilise reader with file input stream
		 */
		explicit BinaryFileReader(std::ifstream &in)
		{
			this->stream = &in;
			ownStream = false;
		}

		/**
		 * Initialise reader with file name (std::string). New file input stream will be created
		 */
		explicit BinaryFileReader(const std::string &fileName)
		{
			this->stream = new std::ifstream(fileName.c_str());
			ownStream = true;
		}

		/**
		 * Initialise reader with file name (const char *). New file input stream will be created
		 */
		explicit BinaryFileReader(const char *fileName)
		{
			this->stream = new std::ifstream(fileName);
			ownStream = true;
		}

		/**
		 * Unbinds currently binded stream, deletes it if needed
		 */
		void unbindStream()
		{
			if (ownStream) delete this->stream;
			this->stream = 0;
		}

		/**
		 * Binds stream to the reader, unbinds an old one if needed
		 */
		void bindStream(std::ifstream &in)
		{
			unbindStream();
			this->stream = &in;
			ownStream = false;
		}

		/**
		 * Reads one element from binary file input stream
		 * Returns true in case of success, false otherwise
		 */
		bool operator () (DataType &element)
		{
			return this->ready() && this->stream->read(reinterpret_cast<char*>(&element), sizeof(DataType));
		}

		/**
		 * Skips specified number of elements
		 * Return value is true if skip was successfull and false otherwise
		 */
		bool skip(int elements)
		{
			return this->stream->seekg(elements * sizeof(DataType), std::ios_base::cur);
		}

	private:
		bool ownStream;
};

#endif // BINARYFILEREADER_H
