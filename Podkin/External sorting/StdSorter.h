#ifndef STDSORTER_H
#define STDSORTER_H

#include <algorithm>

class StdSorter{
    public:
        template<typename Iterator, typename Comparator>
            void operator (Iterator begin, Iterator end, Comparator comp){
                std::sort(begin, end, comp);
            }
}
