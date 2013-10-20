#ifndef EXTERNALSORT_H
#define EXTERNALSORT_H

#include <queue>
#include <memory>
#include <functional>

#include "io/istreamreader.h"
#include "io/ostreamwriter.h"
#include "io/fstreamqueue.h"
#include "io/optimalstreamio.h"
#include "stdsorter.h"

template<typename DataT,      // type of the data to be sorted
         class Comparator   = std::less<DataT>,       // should have a bool operator()(DataT&) method
         class LocalSorter  = StdSorter,              // should have an operator()(Iterator begin, Iterator end) method
         class TempQueue    = FStreamQueue<DataT,
                                           typename OptimalStreamIO<DataT>::ReaderType,
                                           typename OptimalStreamIO<DataT>::WriterType>
                                                      // should have push(DataT) and bool pop(Data&) methods.
                                                      // may be one-go, i.e. it's guaranteed that push
                                                      // operation will not be called after pop operation
         >
class ExternalSorter
{
public:
    template<class InputReader,   // should have a bool operator()(DataT&) method
             class OutputWriter>  // should have an operator()(DataT) method
    bool sort(InputReader &inputReader, OutputWriter &outputWriter, std::size_t bufferSize)
    {
        std::vector< std::unique_ptr<TempQueue> > buckets;
        return prepareBuckets(inputReader, buckets, bufferSize) && mergeBuckets(outputWriter, buckets);
    }

    void sort(const char *inputFile, const char *outputFile, std::size_t bufferSize)
    {
        std::ifstream inputStream(inputFile);
        std::ofstream outputStream(outputFile);
        IStreamReader<DataT> reader(inputStream);
        OStreamWriter<DataT> writer(outputStream);
        return sort(reader, writer, bufferSize);
    }

    void sort(std::istream &in, std::ostream &out, std::size_t bufferSize)
    {
        IStreamReader<DataT> reader(in);
        OStreamWriter<DataT> writer(out);
        return sort(reader, writer, bufferSize);
    }

private:
    bool dumpBuffer(TempQueue *dest, DataT *buffer, std::size_t n)
    {
        localSort(buffer, buffer + n);
        if (!checkSorted(buffer, buffer + n))
            return false;
        for (std::size_t i = 0; i < n; ++i)
            dest->push(buffer[i]);
        return true;
    }

    template<class It>
    bool checkSorted(It a, It b)
    {
        if (a == b)
            return true;
        while (true)
        {
            It t = a++;
            if (a == b)
                return true;
            if (!compare(*t, *a))
                return false;
        }
    }

    template<class InputReader>
    bool prepareBuckets(InputReader &read, std::vector<std::unique_ptr<TempQueue>> &buckets, std::size_t bufferSize)
    {
        std::unique_ptr<DataT> buffer(new DataT[bufferSize]);
        if (!buffer.get())
            return false;

        std::size_t currentLoad = 0;
        while (read(buffer.get()[currentLoad]))
        {
            ++currentLoad;
            if (currentLoad == bufferSize)
            {
                buckets.emplace_back(new TempQueue);
                if (!dumpBuffer(buckets.back(), buffer.get(), currentLoad))
                    return false;
                currentLoad = 0;
            }
        }
        if (currentLoad)
        {
            buckets.emplace_back(new TempQueue);
            if (!dumpBuffer(buckets.back(), buffer.first, currentLoad))
                return false;
        }

        return true;
    }

    typedef std::pair<DataT, std::size_t> HeapNode;
    struct HeapNodeCompare
    {
        bool operator() (const HeapNode& a, const HeapNode& b)
        {
            if (compare(b.first, a.first))
                return true;
            if (compare(a.first, b.first))
                return false;
            return a.second < b.second;
        }
    };

    template<class OutputWriter>
    bool mergeBuckets(OutputWriter &write, std::vector<std::unique_ptr<TempQueue>> &buckets)
    {
        std::priority_queue<HeapNode, std::vector<HeapNode>, HeapNodeCompare> heap;
        for (std::size_t i = 0; i < buckets.size(); ++i)
        {
            DataT value;
            buckets[i].pop(value);
            heap.emplace(value, i);
        }

        while (!heap.empty())
        {
            HeapNode node = heap.top();
            heap.pop();
            write(node.first);

            if (buckets[node.second]->pop(node.first))
                heap.push(node);
            else
                buckets[node.second]->reset();
        }
        return true;
    }

    Comparator compare;
    LocalSorter localSort;
};

#endif // EXTERNALSORT_H
