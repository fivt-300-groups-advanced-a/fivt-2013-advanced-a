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
#include <assert.h>
#include <stdio.h>
#include "rwstreams.h"

/**
*This class provides Exsternal Sort.
*Template is the type of sorting data
*/

template <typename DataType>
class ExternalSorter
{
public:
    /**
    * This function creates readers and writers by itself
    * It gets filenames and uses fstream to read this data in text format
    * Template is Sorter and Comparator
    * Sorter may have operator() which have 2 args is iterator begin and end
    * Comparator compares DataType
    * Arguments:
    *       memorysize - avaliable memory in bytes
    *       sort - Sorter class
    *       cmp - Comparator class
    */
    template<typename Sorter, typename Comparator >
    void sort(std::size_t memorysize,
					std :: string fin, std :: string fout,
					Sorter sort, Comparator cmp)
    {
        Reader<DataType> reader(std :: unique_ptr<std :: ifstream>(new std :: ifstream(fin.c_str())));
        Writer<DataType> writer(std :: unique_ptr<std :: ofstream>(new std :: ofstream(fout.c_str())));

        this -> sort(8,
            reader,
            writer,
            sort,
            cmp);
    }

    /**
    * This function is main function for sorting
    * Template is Reader, Writer, Sorter and Comparator
    * Reader may have read function and close function
    * Writer may have write function and close function
    * Sorter may have operator() which have 2 args is iterator begin and end
    * Comparator compares DataType
    * Arguments:
    *       memorysize - avaliable memory in bytes
    *       &reader - class, which provides read functions
    *       &writer - class, which provides write functions
    *       sort - Sorter class
    *       cmp - Comparator class
    */
    template<typename Reader, typename Writer,
				 typename Sorter, typename Comparator >
    void sort(std::size_t memorysize,
					Reader &reader, Writer &writer,
					Sorter sort, Comparator cmp)
    {
        int block = memorysize / sizeof (DataType);
        std :: vector<DataType> arr(block);
        int rsize;
        int parts = 0;
        while (rsize = readData(block, reader, arr))
        {
            sort(arr.begin(), arr.begin() + rsize);
            BinaryWriter<DataType> bw = BinaryWriter<DataType>(std :: unique_ptr<std :: ofstream>(new std :: ofstream (getFileName(parts).c_str(), std :: ios::out | std :: ios::binary)));
            writeData(rsize, bw, arr);
            bw.close();
            parts++;
            assert(parts < 10000);

        }
        reader.close();
        merge(parts, writer, cmp);
        writer.close();
        clean_up_files(parts);
    }


private:
    /**
    * This struct is for comparation pair
    * This comparator compare by only first param
    * template is type of comparator of first params
    */

    template <typename Comparator>
    struct PairComparator
    {
        Comparator cmp;
        explicit PairComparator(Comparator &cmp) : cmp(cmp)
        {
        }

        template <typename CmpType>
        bool operator()(const CmpType &t1, const CmpType &t2)
        {
            return cmp(t2.first, t1.first);
        }
    };

    template <typename Writer, typename Comparator>
    void merge(int parts, Writer &writer, Comparator cmp)
    {
        typedef std :: pair<DataType, BinaryReader<DataType>*> mpair;
        PairComparator<Comparator> pcmp(cmp);
        std :: vector<mpair> heap(parts);
        int heapsize = parts;
        for (int i = 0; i < parts; ++i)
        {
            heap[i].second = new BinaryReader<DataType>(std :: unique_ptr<std :: ifstream>(new std :: ifstream(getFileName(i).c_str(), std :: ios::in | std :: ios::binary)));
            heap[i].second->read(heap[i].first);
        }
        std :: make_heap(heap.begin(), heap.begin() + heapsize, pcmp);
        while (heapsize > 0)
        {
            writer.write(heap[0].first);
            if (heap[0].second->read(heap[0].first))
            {
                mpair x = heap[0];
                std :: pop_heap(heap.begin(), heap.begin() + heapsize, pcmp);
                heap[heapsize - 1] = x;
                std :: push_heap(heap.begin(), heap.begin() + heapsize, pcmp);
            }
            else
            {
                std :: pop_heap(heap.begin(), heap.begin() + heapsize, pcmp);
                heapsize--;
            }
        }
        for (int i = 0; i < parts; ++i)
        {
            heap[i].second->close();
            delete heap[i].second;

        }
    }

    /**
    * This function writes a block of data
    * Arguments:
    *       count - quantity of data
    *       &writer - class, which provides writing 1 block of data
    *       &arr - array of data
    */
    template <typename Writer>
    void writeData(int count, Writer &writer, std :: vector<DataType> &arr)
    {
        for (int i = 0; i < count; ++i)
            writer.write(arr[i]);
    }

    /**
    * This function reads a block of data
    * Arguments:
    *       count - quantity of data
    *       &reader - class, which provides reading 1 block of data
    *       &arr - array of data
    */
    template <typename Reader>
    int readData(int count, Reader &reader, std :: vector<DataType> &arr)
    {
        for (int i = 0; i < count; ++i)
            if (!reader.read(arr[i]))
                return i;
        return count;
    }

    /**
    * This function generates the names of temporary files
    * Arguments:
    *       id - is a number of part
    */
    std :: string getFileName(int id)
    {
        char buffer[40];
        sprintf(buffer, ".temp_sort_file%d.bin", id);
        return std :: string(buffer);
    }


    /**
    * This function removes temorary files
    * Arguments:
    *       files - is a number of temporary files
    */
    void clean_up_files(int files)
    {
        for (int i = 0; i < files; ++i)
            remove(getFileName(i).c_str());
    }

};
#endif
