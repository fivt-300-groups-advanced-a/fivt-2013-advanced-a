#ifndef STDSORTER_H
#define STDSORTER_H

#include <algorithm>

class StdSorter
{
public:
    template<class It, class Comparator>
    void operator() (It begin, It end, Comparator pred)
    {
        std::sort(begin, end, pred);
    }
};

#endif // STDSORTER_H
