#ifndef STANDARTSORTER_H
#define STANDARTSORTER_H

#include <algorithm>

template<typename Type>
class StandartSorter{
    public:
        template<typename Iterator, class Comparator>
        void operator()(Iterator Begin, Iterator End, Comparator comp){
            std::sort(Begin, End, comp);
        }
};

#endif
