#ifndef SORTER_H
#define SORTER_H

#include <algorithm>
#include "radixsort.h"


/** This class is sample functor
* It calls std :: sort with template comparator
*/
template<typename Comparator> class StandartSorter
{
public:
    template<typename RandomAccessIterator>
    inline void operator () (RandomAccessIterator begin, RandomAccessIterator end)
    {
        std::sort(begin, end, Comparator());
    }
};


/** This class is sample functor
* It calls RadixSort :: sort with template Extractor
*/
template<typename Exstractor> class RadixSorter
{
public:
    template<typename RandomAccessIterator>
    inline void operator () (RandomAccessIterator begin, RandomAccessIterator end)
    {
        RadixSort :: sort(begin, end, Exstractor());
    }
};

#endif
