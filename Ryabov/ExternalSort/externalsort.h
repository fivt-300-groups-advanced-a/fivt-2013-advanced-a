#ifndef EXTERNALSORTER_H
#define EXTERNALSORTER_H
#include <algorithm>
#include <functional>
#include <memory.h>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include "rwstreams.h"

using namespace std;

template <typename DataType>
class ExternalSorter
{
public:
    template<typename Reader, typename Writer,
				 typename Sorter, typename Comparator >
    bool sort(std::size_t memorysize,
					Reader &reader, Writer &writer,
					Sorter sort, Comparator cmp)
    {
        //clean_up_files(100000);
        int block = memorysize / sizeof (DataType);
        clean_up_files();
        vector<DataType> arr(block);
        int rsize;
        int parts = 0;
        while (rsize = readData(block, reader, arr))
        {
            sort(arr.begin(), arr.begin() + rsize, cmp);
            BinaryWriter<DataType> bw = BinaryWriter<DataType>(std :: unique_ptr<std :: ofstream>(new std :: ofstream (getFileName(parts).c_str(), ios::out | ios::binary)));
            writeData(rsize, bw, arr);
            bw.close();
            parts++;
        }
        reader.close();
        merge(parts, writer, cmp);
        writer.close();
        clean_up_files(parts);
        return true;
    }


private:

    struct mpair
    {
        DataType value;
        BinaryReader<DataType> *br;
    };


    template <typename Comparator>
    struct ComparatorClass
    {
        Comparator cmp;
        explicit ComparatorClass(Comparator &cmp) : cmp(cmp)
        {
        }
        bool operator()(const mpair &t1, const mpair &t2)
        {
            return cmp(t2.value, t1.value);
        }
    };

    template <typename Writer, typename Comparator>
    void merge(int parts, Writer &writer, Comparator cmp)
    {
        vector<mpair> heap(parts);
        int heapsize = parts;
        for (int i = 0; i < parts; ++i)
        {
            heap[i].br = new BinaryReader<DataType>(std :: unique_ptr<std :: ifstream>(new std :: ifstream(getFileName(i).c_str(), ios::in | ios::binary)));
            heap[i].br->read(heap[i].value);
        }
        std :: make_heap(heap.begin(), heap.begin() + heapsize, ComparatorClass<Comparator>(cmp));
        while (heapsize > 0)
        {
            writer.write(heap[0].value);
            if (heap[0].br->read(heap[0].value))
            {
                std :: pop_heap(heap.begin(), heap.begin() + heapsize, ComparatorClass<Comparator>(cmp));
                std :: push_heap(heap.begin(), heap.begin() + heapsize, ComparatorClass<Comparator>(cmp));
            }
            else
            {
                std :: pop_heap(heap.begin(), heap.begin() + heapsize, ComparatorClass<Comparator>(cmp));
                heapsize--;
            }
        }
        for (int i = 0; i < parts; ++i)
        {
            heap[i].br->close();
            delete heap[i].br;

        }
    }

    template <typename Writer>
    void writeData(int count, Writer &writer, vector<DataType> &arr)
    {
        for (int i = 0; i < count; ++i)
            writer.write(arr[i]);
    }

    template <typename Reader>
    int readData(int count, Reader &reader, vector<DataType> &arr)
    {
        for (int i = 0; i < count; ++i)
            if (!reader.read(arr[i]))
                return i;
        return count;
    }

    std :: string getFileName(int id)
    {
        char buffer[30];
        sprintf(buffer, ".temp_sort_file%d.bin", id);
        return std :: string(buffer);
    }

    bool clean_up_files(int files = 100)
    {
        for (int i = 0; i < files; ++i)
            remove(getFileName(i).c_str());
    }

};
#endif
