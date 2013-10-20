#ifndef BINARYFILEWRITER_H
#define BINARYFILEWRITER_H

#include <fstream>

namespace impl
{
	class BinaryFileWriterHelper
	{
		public:
			/**
			 * Unbinds currently binded stream, deletes it if needed
			 */
			void unbindStream()
			{
				if (stream) stream->close();
				if (ownStream) delete stream;
				stream = 0;
			}

			/**
			 * Binds stream to the reader, unbinds an old one if needed
			 */
			void bindStream(std::ofstream &out)
			{
				unbindStream();
				stream = &out;
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
			std::ofstream* getStream() const
			{
				return stream;
			}

		protected:
			bool ownStream;
			std::ofstream *stream;
	};
}

/**
 * Class implementing AbstractWriter interface
 * Used to write fixed-type data in binary format to the file output stream
 */
template <typename DataType> class BinaryFileWriter : impl::BinaryFileWriterHelper
{
	public:
		/**
		 * Initilise reader with file output stream
		 */
		explicit BinaryFileWriter(std::ofstream &out)
		{
			stream = &out;
			ownStream = false;
		}

		/**
		 * Initialise writer with file name (std::string). New file output stream will be created
		 */
		explicit BinaryFileWriter(const std::string &fileName)
		{
			stream = new std::ofstream(fileName, std::ios_base::binary);
			ownStream = true;
		}

		/**
		 * Initialise writer with file name (const char *). New file output stream will be created
		 */
		explicit BinaryFileWriter(const char *fileName)
		{
			stream = new std::ofstream(fileName, std::ios_base::binary);
			ownStream = true;
		}

		/**
		 * Unbind stream and delete writer
		 */
		~BinaryFileWriter()
		{
			unbindStream();
		}

		/**
		 * Writes one element to binary file output stream
		 * Returns true in case of success, false otherwise
		 */
		bool operator () (const DataType &element)
		{
			if (!ready()) return false;
			return stream->write(reinterpret_cast<const char*>(&element), sizeof(DataType));
		}
};

template<> class BinaryFileWriter<std::string> : impl::BinaryFileWriterHelper
{
	public:
		/**
		 * Initilise reader with file output stream
		 */
		explicit BinaryFileWriter(std::ofstream &out)
		{
			stream = &out;
			ownStream = false;
		}

		/**
		 * Initialise writer with file name (std::string). New file output stream will be created
		 */
		explicit BinaryFileWriter(const std::string &fileName)
		{
			stream = new std::ofstream(fileName, std::ios_base::binary);
			ownStream = true;
		}

		/**
		 * Initialise writer with file name (const char *). New file output stream will be created
		 */
		explicit BinaryFileWriter(const char *fileName)
		{
			stream = new std::ofstream(fileName, std::ios_base::binary);
			ownStream = true;
		}

		/**
		 * Unbind stream and delete writer
		 */
		~BinaryFileWriter()
		{
			unbindStream();
		}

		bool operator () (std::string &element)
		{
			if (!ready()) return false;
			std::string::size_type size = element.size();
			if (!stream->write(reinterpret_cast<char *>(&size), sizeof(size))) return false;
			if (!stream->write(element.c_str(), element.size())) return false;
			return true;
		}
};

#endif // BINARYFILEWRITER_H
