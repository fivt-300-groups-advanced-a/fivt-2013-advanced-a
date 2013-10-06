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
 * default or custom reader, writer, sorter and comparator functors
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
		 * Uses own realisation of binary heap because std::priority_queue is slow and inconvinient
		 * Returns true if succeeded and false if error occured or data set is empty
		 * Important: default sorting is unstable!
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
		 * Constructs heap from array (heap) of size (tempFiles)
		 * Has linear complexity (at most 2 * tempFiles calls of comparator())
		 */
		template<typename Comparator> void constructHeap(Comparator &comparator)
		{
			heapSize = 0;
			for (size_t i = 0; i < tempFiles; i++)
				pushHeap(comparator);
		}

		/*
		 * Pops min element from heap
		 * Does not more than 2 * log(heapSize) calls of comparator()
		 */
		template<typename Comparator> void popHeap(Comparator &comparator)
		{
			assert(heapSize > 0);
			swap(heap[0], heap[--heapSize]);
			size_t curpos = 0;
			while (2 * curpos + 1 < heapSize)
			{
				size_t child = 2 * curpos + 1;
				if (child + 1 < heapSize && comparator(heap[child + 1].first, heap[child].first))
					++child;
				if (comparator(heap[child].first, heap[curpos].first))
					heap[curpos].swap(heap[child]), curpos = child;
				else break;
			}
		}

		/*
		 * Inserts element to heap assuming that it is in position (heapSize) from begining of (heap)
		 * Does not more than log(heapSize) calls of comparator()
		 */
		template<typename Comparator> void pushHeap(Comparator &comparator)
		{
			size_t curpos = heapSize++;
			while (curpos != 0)
			{
				if (comparator(heap[curpos].first, heap[(curpos - 1) >> 1].first))
					heap[curpos].swap(heap[(curpos - 1) >> 1]);
				curpos = (curpos - 1) >> 1;
			}
		}

		/*
		 * Merges temporary sorted files created after reading data into one and outputs to (writer)
		 * Returns true if no error occured
		 */
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
			constructHeap(comparator);

			for (size_t i = 0; i < dataSize; i++)
			{
				if (!heapSize) return false;
				if (!writer(heap[0].first)) return false;
				int id = heap[0].second;
				popHeap(comparator);
				if (streams[id]->operator() (heap[heapSize].first))
				{
					heap[heapSize].second = id;
					pushHeap(comparator);
				}
			}

			assert(heapSize == 0);
			delete [] heap;
			for (std::size_t i = 0; i < tempFiles; i++) delete streams[i];
			return true;
		}
};

#endif // EXTERNALFILESORTER_H
