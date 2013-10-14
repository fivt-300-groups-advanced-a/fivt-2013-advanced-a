#ifndef EXTERNALSORTER_H
#define EXTERNALSORTER_H

#include <cstdio>
#include <cassert>

#include <queue>
#include <string>
#include <memory>

#include "io/binaryfilewriter.h"
#include "io/binaryfilereader.h"

namespace implementation
{
	template<typename T, typename Comparator> class StableHeapElementComparator
	{
		Comparator cmp;

		bool operator () (const std::pair<T, int> &a, const std::pair<T, int> &b)
		{
			return (cmp(b.first, a.first) || (!cmp(a.first, b.first) && a.second > b.second));
		}
	};

	template<typename T, typename Comparator> class HeapElementComparator
	{
		public:
			Comparator cmp;

			bool operator () (const std::pair<T, int> &a, const std::pair<T, int> &b)
			{
				return cmp(b.first, a.first);
			}
	};
}

/**
 * ExternalFileSorter class is used for sorting extenal fixed-type data files using
 * default or custom reader, writer, sorter and comparator functors
 */
template<typename DataType> class ExternalSorter
{
	public:
		/**
		 * Reads external data from reader spliting it into pieces each not more (availableMemory) bytes
		 * Sorts each piece and writes them into files in binary format
		 * Then performs merging of this files using mergesort algorithm with binary heap
		 * Outputs result using Writer
		 * Uses not more than (avaialbeMemory) bytes for storing data at any moment
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

			bool success = mergeFiles<Writer, TemporaryReader,
									  implementation::HeapElementComparator<DataType, Comparator> >(writer);
			return success;
		}

		/**
		 * Stable variant of ExternalFileSorter::sort function
		 * (see ExtenalFileSorter::sort function for details)
		 * Important: requires stable Sorter
		 */
		template<typename Reader, typename Writer,
				 typename Sorter, typename Comparator,
				 typename TemporaryReader, typename TemporaryWriter>
		bool stableSort(std::size_t availableMemory,
				  Reader &reader, Writer &writer,
				  Sorter sorter, Comparator comparator)
		{
			int bufferSize = availableMemory / sizeof(DataType);
			if (!bufferSize) return false;
			if (!readData<Reader, TemporaryWriter, Sorter, Comparator>(bufferSize, reader, sorter, comparator))
				return false;

			bool success = mergeFiles<Writer, TemporaryReader,
									  implementation::StableHeapElementComparator<DataType, Comparator> >(writer);
			return success;
		}

	private:
		std::size_t tempFiles;

		/**
		 * Writes an array of data to file in binary format
		 * Returns true if succeeded
		 */
		template<typename TemporaryWriter, typename ForwardIterator>
		bool writeFile(unsigned int id, ForwardIterator start, std::size_t items)
		{
			TemporaryWriter *writer = new TemporaryWriter(getFileName(id));
			for (; items; --items, ++start)
				if (!writer->operator()(*start)) return false;
			delete writer;
			return true;
		}

		/**
		 * Reads data from reader to buffer, sorts it and outputs to temporary files
		 * Returns number of created files (less or equal 0 if error)
		 */
		template<typename Reader, typename TemporaryWriter, typename Sorter, typename Comparator>
		int readData(std::size_t bufferSize, Reader &reader, Sorter &sorter, Comparator &comparator) // TODO: bad name!
		{
			std::vector<DataType> buffer(bufferSize);
			tempFiles = 0;
			std::size_t currentSize = 0;

			while (reader(buffer[currentSize]))
			{
				++currentSize;
				if (currentSize == bufferSize)
				{
					sorter(buffer.begin(), buffer.begin() + currentSize, comparator);
					if (!writeFile<TemporaryWriter>(tempFiles, buffer.begin(), currentSize)) return 0;
					currentSize = 0;
					++tempFiles;
				}
			}
			if (currentSize != 0)
			{
				sorter(buffer.begin(), buffer.begin() + currentSize, comparator);
				if (!writeFile<TemporaryWriter>(tempFiles, buffer.begin(), currentSize)) return 0;
				currentSize = 0;
				++tempFiles;
			}
			assert(currentSize == 0);
			return tempFiles;
		}

		std::string getFileName(std::size_t id)
		{
			char buffer[30]; // FIXME: fixed size
			sprintf(buffer, ".part.%lu", id);
			return std::string(buffer);
		}

		/**
		 * Merges temporary sorted files created after reading data into one and outputs to (writer)
		 * Uses mergesort algorithm with binary heap (HeapClass)
		 * Returns true if no error occured
		 * TODO: std::unique_ptr & std::priority_queue
		 */
		template<typename Writer, typename TemporaryReader, typename PairComparator>
		bool mergeFiles(Writer &writer)
		{
			typedef std::pair<DataType, int> HeapElement;

			assert(tempFiles != 0);
			std::vector< std::unique_ptr<TemporaryReader> > streams;

			std::priority_queue<HeapElement, std::vector<HeapElement>, PairComparator> heap;
			HeapElement current;

			for (std::size_t i = 0; i < tempFiles; i++)
			{
				streams.push_back(std::unique_ptr<TemporaryReader>(new TemporaryReader(getFileName(i)))); // TODO: factory to construct readers
				streams[i]->operator() (current.first);
				current.second = i;
				heap.push(current);
			}

			while (!heap.empty())
			{
				current = heap.top();
				heap.pop();
				if (!writer(current.first)) return false;
				if (streams[current.second]->operator() (current.first))
					heap.push(current);
			}

			return true;
		}
};

#endif // EXTERNALSORTER_H
