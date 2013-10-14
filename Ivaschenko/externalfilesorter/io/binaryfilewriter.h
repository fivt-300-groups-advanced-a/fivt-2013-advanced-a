#ifndef BINARYFILEWRITER_H
#define BINARYFILEWRITER_H

#include "abstractwriter.h"

/**
 * Class implementing AbstractWriter interface
 * Used to write fixed-type data in binary format to the file output stream
 */
template <typename DataType> class BinaryFileWriter : public AbstractWriter<DataType>
{
	public:
		/**
		 * Initilise reader with file output stream
		 */
		explicit BinaryFileWriter(std::ofstream &out)
		{
			this->stream = &out;
			ownStream = false;
		}

		/**
		 * Initialise writer with file name (std::string). New file output stream will be created
		 */
		explicit BinaryFileWriter(const std::string &fileName)
		{
			this->stream = new std::ofstream(fileName, std::ios_base::binary);
			ownStream = true;
		}

		/**
		 * Initialise writer with file name (const char *). New file output stream will be created
		 */
		explicit BinaryFileWriter(const char *fileName)
		{
			this->stream = new std::ofstream(fileName, std::ios_base::binary);
			ownStream = true;
		}

		/**
		 * Initialise writer with stream ID. BinaryWriter is compatible with TemporaryWriter interface
		 */
		explicit BinaryFileWriter(unsigned int streamID)
		{
			this->stream = new std::ofstream(getFileName(streamID), std::ios_base::binary);
			ownStream = true;
		}



		/**
		 * Unbinds currently binded stream, deletes it if needed
		 */
		void unbindStream()
		{
			if (this->stream) this->stream->flush();
			if (ownStream) delete this->stream;
			this->stream = 0;
		}

		/**
		 * Binds stream to the reader, unbinds an old one if needed
		 */
		void bindStream(std::ofstream &out)
		{
			unbindStream();
			this->stream = &out;
			ownStream = false;
		}

		/**
		 * Writes one element to binary file output stream
		 * Returns true in case of success, false otherwise
		 */
		bool operator () (const DataType &element)
		{
			if (!this->ready()) return false;
			return this->stream->write(reinterpret_cast<const char*>(&element), sizeof(DataType));
		}

	private:
		bool ownStream;

		std::string getFileName(unsigned int streamID) const
		{
			char buffer[20];
			sprintf(buffer, ".sorter.part.%u", streamID);
			return std::string(buffer);
		}
};

#endif // BINARYFILEWRITER_H
