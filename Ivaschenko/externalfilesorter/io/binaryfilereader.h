#ifndef BINARYFILEREADER_H
#define BINARYFILEREADER_H

#include <fstream>
#include <string>

#include <cassert>

namespace implementation
{
	class BinaryFileReaderHelper
	{
		public:
			/**
			 * Unbinds currently binded stream, deletes it if needed
			 */
			void unbindStream()
			{
				if (ownStream) delete stream;
				stream = 0;
			}

			/**
			 * Binds stream to the reader, unbinds an old one if needed
			 */
			void bindStream(std::ifstream &in)
			{
				unbindStream();
				stream = &in;
				ownStream = false;
			}

			/**
			 * Returns true is stream is binded, not corrupted and ready to read
			 */
			bool ready() const
			{
				return stream && *stream;
			}

			/**
			 * Returns a pointer to the binded stream
			 * It can be used for adjusting precision or other options for example
			 */
			std::ifstream* getStream() const
			{
				return stream;
			}

		protected:
			std::ifstream *stream;
			bool ownStream;
	};
}

/**
 * Class implementing AbstractReader interface
 * Used to read fixed-type data in binary format from file input stream
 * FIXME: std::string, std::vector works incorrectly
 */
template<typename DataType> class BinaryFileReader : implementation::BinaryFileReaderHelper
{
	public:
		/**
		 * Initilise reader with file input stream
		 */
		explicit BinaryFileReader(std::ifstream &in)
		{
			stream = &in;
			ownStream = false;
		}

		/**
		 * Initialise reader with file name (std::string). New file input stream will be created
		 */
		explicit BinaryFileReader(const std::string &fileName)
		{
			stream = new std::ifstream(fileName, std::ios_base::binary);
			ownStream = true;
		}

		/**
		 * Initialise reader with file name (const char*). New file input stream will be created
		 */
		explicit BinaryFileReader(const char *fileName)
		{
			stream = new std::ifstream(fileName, std::ios_base::binary);
			ownStream = true;
		}

		~BinaryFileReader()
		{
			unbindStream();
		}

		/**
		 * Reads one element from binary file input stream
		 * Returns true in case of success, false otherwise
		 */
		bool operator () (DataType &element)
		{
			return ready() && stream->read(reinterpret_cast<char*>(&element), sizeof(DataType));
		}

		/**
		 * Skips specified number of elements
		 * Return value is true if skip was successfull and false otherwise
		 */
		virtual bool skip(int elements)
		{
			return stream->seekg(elements * sizeof(DataType), std::ios_base::cur);
		}
};

#endif // BINARYFILEREADER_H
