#ifndef SORTER

#define SORTER

#include <vector>
#include <algorithm>

template <class T, class Comparator> class Sorter {
    public:
        void operator() (std::vector<T> &to_sort){
            Comparator cmp;
            std::sort(to_sort.begin(), to_sort.end(), cmp);
        }
};

#endif
