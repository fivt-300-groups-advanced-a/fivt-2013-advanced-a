#ifndef EXTERNALFILESORTER_H
#define EXTERNALFILESORTER_H

#include <cstdio>
#include <cassert>

#include <queue>
#include <string>

#include "io/binaryfilewriter.h"
#include "io/binaryfilereader.h"

/*
 * ExternalFileSorter class is used for sorting extenal fixed-type data files using
 * default or custom reader, writer, sorter and comparator functor
 */
template<typename DataType> class ExternalFileSorter
{
	public:
		/*
		 * Reads external data from reader spliting it into pieces each not more (availableMemory) bytes
		 * Sorts each piece and writes them into files in binary format
		 * Then performs merging of this files using mergesort algorithm with binary heap
		 * Outputs result using Writer
		 * Default values are:
		 *		- InputStreamReader<DataType> for Reader
		 *		- OutputStramWriter<DataType> for Writer
		 *		- std::sort for Sorter
		 *		- std::less for Comparator
		 * Uses not more than (avaialbeMemory) bytes for storing data at any moment
		 * Returns true if succeeded and false if error occured or data set is empty
		 */
		template<typename Reader, typename Writer, typename Sorter, typename Comparator>
		bool sort(std::size_t availableMemory,
				  Reader &reader,
				  Writer &writer,
				  Sorter sorter,
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
			bool success = mergeFiles(writer, comparator);
			cleanUp();
			return success;
		}

	private:
		std::size_t tempFiles, dataSize, heapSize;
		std::pair<DataType, int> *heap;

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
			for (std::size_t i = 0; i < tempFiles; ++i)
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
		 * Returns number of created files (<= 0 if error)
		 */
		template<typename Reader, typename Sorter, typename Comparator>
		int readData(std::size_t bufferSize, Reader &reader, Sorter &sorter, Comparator &comparator)
		{
			DataType *buffer = new DataType[bufferSize];
			tempFiles = 0;
			dataSize = 0;
			std::size_t currentSize = 0;

			while (reader(buffer[currentSize++]))
			{
				if (currentSize == bufferSize)
				{
					sorter(buffer, buffer + currentSize, comparator);
					if (!writeFile(getFileName(tempFiles), buffer, bufferSize)) return 0;
					currentSize = 0;
					++tempFiles;
				}
				dataSize++;
			}
			if (currentSize != 0)
			{
				if (!writeFile(getFileName(tempFiles), buffer, bufferSize)) return 0;
				currentSize = 0;
				++tempFiles;
			}
			delete [] buffer;
			assert(currentSize == 0);
			return tempFiles;
		}

		/*
		 * TODO: construct heap!
		 */
		template<typename RAccessIterator, typename Comparator>
		void constructHeap(RAccessIterator start, RAccessIterator end, Comparator &comparator)
		{

		}

		/*
		 * TODO: pop heap!
		 */
		template<typename Comparator> void popHeap(Comparator &comparator)
		{

		}

		/*
		 * TODO: push heap!
		 */
		template<typename Comparator> void pushHeap(Comparator &comparator)
		{

		}

		template<typename Writer, typename Comparator>
		bool mergeFiles(Writer &writer, Comparator &comparator)
		{
			assert(tempFiles != 0);
			std::vector< BinaryFileReader<DataType>* > streams(tempFiles);
			heap = new std::pair<DataType, int>[tempFiles];
			heapSize = tempFiles;

			for (std::size_t i = 0; i < tempFiles; i++)
			{
				streams[i] = new BinaryFileReader<DataType>(getFileName(i));
				streams[i]->operator() (heap[i].first);
				heap[i].second = i;
			}
			constructHeap(heap, heap + tempFiles, comparator);

			for (size_t i = 0; i < dataSize; i++)
			{
				if (!heapSize) return false;
				if (!writer(heap[0].first)) return false;
				int id = heap[0].second;
				popHeap(comparator);
				if (streams[id]->operator() (heap[heapSize].first))
					heap[heapSize++].second = id;
				pushHeap(comparator);
			}

			assert(heapSize == 0);
			delete [] heap;
			for (std::size_t i = 0; i < tempFiles; i++) delete streams[i];
			return true;
		}
};

#endif // EXTERNALFILESORTER_H
