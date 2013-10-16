#ifndef STDSORTER_H
#define STDSORTER_H

#include <algorithm>

class StdSorter
{
public:
    template<class It, class Comparator = std::less<typename std::iterator_traits<It>::reference> >
    void operator() (It begin, It end)
    {
        std::sort(begin, end, Comparator());
    }
};

#endif // STDSORTER_H
