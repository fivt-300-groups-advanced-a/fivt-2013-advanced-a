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
		 * Uses not more than (avaialbeMemory) bytes for storing data at any moment
		 * Uses own realisation of binary heap because std::priority_queue is slow and inconvinient
		 * Returns true if succeeded and false if error occured or data set is empty
		 * Important: default sorting is unstable
		 *
		 * Template arguments:
		 *		Reader and Writer - functors that somehow reads input and writes final output
		 *		Sorter - functor defining sorting algorithm (accepts two iterators and comparator)
		 *		Comparator - functor defining operator < for DataType = std::less<DataType> by default
		 *
		 *		TemporaryReader and Temporary writers are functors for optimised (not for human reading)
		 *		temporary input-output. Both must have constructor with unsigned int parameter (stream ID).
		 *		Default values are BinaryFileReader<DataType> and BinaryFileWriter<DataType>
		 *
		 *		(!) Important: TemporaryReader and Writer must be organised in a such way that
		 *		data wrote to i-th stream with TemporaryWriter(i) could be read by TemporaryReader(i)
		 *		stream ID is always unsigned integer
		 *		Readers and writers will be creater by operator new and destroyed by delete
		 */
		template<typename Reader, typename Writer,
				 typename Sorter, typename Comparator,
				 typename TemporaryReader = BinaryFileReader<DataType>,
				 typename TemporaryWriter = BinaryFileWriter<DataType> >
		bool sort(  std::size_t availableMemory,
					Reader &reader, Writer &writer,
					Sorter sorter, Comparator comparator)
		{
			int bufferSize = availableMemory / sizeof(DataType);
			if (!bufferSize) return false;
			if (!readData<Reader, TemporaryWriter, Sorter, Comparator>(bufferSize, reader, sorter, comparator))
				return false;

			bool success = mergeFiles<Writer, TemporaryReader, Comparator, BinaryHeap<DataType> >(writer, comparator);
			return success;
		}

		/*
		 * Stable variant of ExternalFileSorter::sort function
		 * (see ExtenalFileSorter::sort function for details)
		 * Important: requires stable Sorter
		 */
		template<typename Reader, typename Writer,
				 typename Sorter, typename Comparator,
				 typename TemporaryReader = BinaryFileReader<DataType>,
				 typename TemporaryWriter = BinaryFileWriter<DataType> >
		bool stableSort(std::size_t availableMemory,
				  Reader &reader, Writer &writer,
				  Sorter sorter, Comparator comparator)
		{
			int bufferSize = availableMemory / sizeof(DataType);
			if (!bufferSize) return false;
			if (!readData<Reader, TemporaryWriter, Sorter, Comparator>(bufferSize, reader, sorter, comparator))
				return false;

			bool success = mergeFiles<Writer, TemporaryReader, Comparator, StableBinaryHeap<DataType> >(writer, comparator);
			return success;
		}

	private:
		std::size_t tempFiles, dataSize;

		/*
		 * Writes an array of data to file in binary format
		 * Returns true if succeeded
		 */
		template<typename TemporaryWriter>
		bool writeFile(unsigned int id, DataType *start, int items)
		{
			TemporaryWriter *writer = new TemporaryWriter(id);
			for (; items; --items, ++start)
				if (!writer->operator()(*start)) return false;
			delete writer;
			return true;
		}

		/*
		 * Reads data from reader to buffer, sorts it and outputs to temporary files
		 * Returns number of created files (less or equal 0 if error)
		 */
		template<typename Reader, typename TemporaryWriter, typename Sorter, typename Comparator>
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
					if (!writeFile<TemporaryWriter>(tempFiles, buffer, currentSize)) return 0;
					currentSize = 0;
					++tempFiles;
				}
			}
			if (currentSize != 0)
			{
				sorter(buffer, buffer + currentSize, comparator);
				if (!writeFile<TemporaryWriter>(tempFiles, buffer, currentSize)) return 0;
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
		template<typename Writer, typename TemporaryReader, typename Comparator, typename HeapClass>
		bool mergeFiles(Writer &writer, Comparator &comparator)
		{
			assert(tempFiles != 0);
			std::vector< BinaryFileReader<DataType>* > streams(tempFiles);

			HeapClass heap;
			heap.data = new std::pair<DataType, int>[tempFiles];

			for (std::size_t i = 0; i < tempFiles; i++)
			{
				streams[i] = new TemporaryReader(i);
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
