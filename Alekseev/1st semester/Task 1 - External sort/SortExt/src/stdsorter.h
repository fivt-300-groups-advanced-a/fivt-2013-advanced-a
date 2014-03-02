#ifndef STDSORTER_H
#define STDSORTER_H

#include <algorithm>

template<class Comparator>
class StdSorter
{
public:
    template<class It>
    void operator() (It begin, It end)
    {
        std::sort(begin, end, pred);
    }
private:
    Comparator pred;
};

#endif // STDSORTER_H
