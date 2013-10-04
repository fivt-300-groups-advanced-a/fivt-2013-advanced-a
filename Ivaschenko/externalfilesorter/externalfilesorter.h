#ifndef EXTERNALFILESORTER_H
#define EXTERNALFILESORTER_H

#include <cstdio>

#include <algorithm>
#include <string>>
#include <cassert>

#include "io/binaryfilewriter.h"

/*
 * ExternalFileSorter class is used for sorting extenal fixed-type data files using
 * default or custom reader, writer, sorter and comparator functor
 */
template<typename DataType> class ExternalFileSorter
{
	public:
		/*
		 * Reads external data from reader spliting it into pieces each not more availableMemory bytes
		 * Sorts each piece and writes them into files in binary format
		 * Then performs merging of this files using mergesort algorithm with binary heaps
		 * Outputs result using Writer
		 * Default values are:
		 *		- InputStreamReader<DataType> for Reader
		 *		- OutputStramWriter<DataType> for Writer
		 *		- std::sort for Sorter
		 *		- std::less for Comparator
		 * Uses not more than avaialbeMemory bytes for storing data at any moment
		 * Returns true if succeeded and false if error occured or data set is empty
		 */
		template<typename Reader, typename Writer, typename Sorter, typename Comparator>
		bool sort(std::size_t availableMemory,
				  Reader &reader = InputStreamReader<DataType>(),
				  Writer &writer = OutputStreamWriter<DataType>(),
				  Sorter sorter = std::sort,
				  Comparator comparator = std::less<DataType>())
		{
			cleanUp();
			int bufferSize = availableMemory / sizeof(DataType);
			if (!bufferSize) return false;
			if (!readData(bufferSize, reader, sorter, comparator))
			{
				cleanUp();
				return false;
			}
			bool success = mergeFiles(tempFiles, writer);
			cleanUp();
			return success;
		}

	private:
		int tempFiles;

		/*
		 * Returns temporary file name #number.
		 * TODO: maybe it worth to insert some obfuscation to resolve file name conflicts
		 */
		std::string getFileName(int number)
		{
			char name[100];
			sprintf(name, "%d", number);
			return ".part" + std::string(name) + ".tmp";
		}

		/*
		 * Cleans up all created files
		 * Returns true if succeeded
		 */
		bool cleanUp()
		{
			for (int i = 0; i < tempFiles; ++i)
				if (remove(getFileName(i).c_str())) return false;
			tempFiles = 0;
			return true;
		}

		/*
		 * Writes an array of data to file in binary format
		 * Returns true if succeeded
		 */
		bool writeFile(const std::string &name, DataType *start, int items)
		{
			BinaryFileWriter<DataType> writer(name);
			for (; items; --items, ++start)
				if (!writer(*start)) return false;
			return true;
		}

		/*
		 * Reads data from reader to buffer, sorts it and outputs to temporary files
		 * Returns number of created files (0 if error)
		 */
		template<typename Reader, typename Sorter, typename Comparator>
		int readData(int bufferSize, Reader &reader, Sorter &sorter, Comparator &comparator)
		{
			DataType *buffer = new DataType[bufferSize, std::nothrow_t()];
			tempFiles = 0;
			if (!buffer) return tempFiles = 0;
			int currentSize = 0;

			while (reader(buffer[currentSize++]))
				if (currentSize == bufferSize)
				{
					sorter(buffer, buffer + currentSize, comparator);
					if (!writeFile(getFileName(tempFiles), buffer, bufferSize)) return 0;
					currentSize = 0;
					++tempFiles;
				}
			if (currentSize != 0)
			{
				if (!writeFile(getFileName(tempFiles), buffer, bufferSize)) return 0;
				currentSize = 0;
				++tempFiles;
			}
			delete buffer;
			assert(currentSize == 0);
			return tempFiles;
		}
};

#endif // EXTERNALFILESORTER_H
