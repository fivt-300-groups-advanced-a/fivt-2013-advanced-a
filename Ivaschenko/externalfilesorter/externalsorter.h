#ifndef EXTERNALSORTER_H
#define EXTERNALSORTER_H

#include <cstdio>
#include <cassert>

#include <queue>
#include <string>

#include "io/binaryfilewriter.h"
#include "io/binaryfilereader.h"

#include "utils/binaryheap.h"
#include "utils/stablebinaryheap.h"

/*
 * ExternalFileSorter class is used for sorting extenal fixed-type data files using
 * default or custom reader, writer, sorter and comparator functors
 */
template<typename DataType> class ExternalSorter
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
		 * Uses own realisation of binary heap because std::priority_queue is slow and inconvinient
		 * Returns true if succeeded and false if error occured or data set is empty
		 * Important: default sorting is unstable
		 */
		template<typename Reader, typename Writer, typename Sorter, typename Comparator>
		bool sort(  std::size_t availableMemory,
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
			BinaryHeap<DataType> heap;
			bool success = mergeFiles(writer, comparator, heap);
			cleanUp();
			return success;
		}

		/*
		 * Stable variant of ExternalFileSorter::sort function
		 * Important: requires stable Sorter
		 */
		template<typename Reader, typename Writer, typename Sorter, typename Comparator>
		bool stableSort(std::size_t availableMemory,
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
			StableBinaryHeap<DataType> heap;
			bool success = mergeFiles(writer, comparator, heap);
			cleanUp();
			return success;
		}

	private:
		std::size_t tempFiles, dataSize;

		/*
		 * Returns temporary file name #number.
		 * TODO: maybe it worth to insert some obfuscation to resolve file name conflicts
		 */
		std::string getFileName(int number)
		{
			char name[100]; // FIXME: fixed size
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
		 * Returns number of created files (less or equal 0 if error)
		 */
		template<typename Reader, typename Sorter, typename Comparator>
		int readData(std::size_t bufferSize, Reader &reader, Sorter &sorter, Comparator &comparator)
		{
			DataType *buffer = new DataType[bufferSize];
			tempFiles = 0;
			dataSize = 0;
			std::size_t currentSize = 0;

			while (reader(buffer[currentSize]))
			{
				++currentSize, ++dataSize;
				if (currentSize == bufferSize)
				{
					sorter(buffer, buffer + currentSize, comparator);
					if (!writeFile(getFileName(tempFiles), buffer, currentSize)) return 0;
					currentSize = 0;
					++tempFiles;
				}
			}
			if (currentSize != 0)
			{
				sorter(buffer, buffer + currentSize, comparator);
				if (!writeFile(getFileName(tempFiles), buffer, currentSize)) return 0;
				currentSize = 0;
				++tempFiles;
			}
			delete [] buffer;
			assert(currentSize == 0);
			return tempFiles;
		}

		/*
		 * Merges temporary sorted files created after reading data into one and outputs to (writer)
		 * Uses mergesort algorithm with binary heap (HeapClass)
		 * Returns true if no error occured
		 */
		template<typename Writer, typename Comparator, typename HeapClass>
		bool mergeFiles(Writer &writer, Comparator &comparator, HeapClass &heap)
		{
			assert(tempFiles != 0);
			std::vector< BinaryFileReader<DataType>* > streams(tempFiles);
			heap.data = new std::pair<DataType, int>[tempFiles];

			for (std::size_t i = 0; i < tempFiles; i++)
			{
				streams[i] = new BinaryFileReader<DataType>(getFileName(i));
				streams[i]->operator() (heap.data[i].first);
				heap.data[i].second = i;
			}
			heap.construct(tempFiles, comparator);

			for (std::size_t i = 0; i < dataSize; i++)
			{
				if (!heap.size) return false;
				if (!writer(heap.data[0].first)) return false;
				int id = heap.data[0].second;
				heap.pop(comparator);
				if (streams[id]->operator() (heap.data[heap.size].first))
				{
					heap.data[heap.size].second = id;
					heap.push(comparator);
				}
			}

			assert(heap.size == 0);
			delete [] heap.data;
			for (std::size_t i = 0; i < tempFiles; i++) delete streams[i];
			return true;
		}
};

#endif // EXTERNALSORTER_H
