#ifndef SORTHELPER_H
#define SORTHELPER_H

#include <vector>
#include <algorithm>

#include "testutils.h"

#include "gtest/gtest.h"

template<class Heap>
class HeapSortChecker
{
public:
    typedef typename Heap::Key Value;

    HeapSortChecker(Heap &&heap = Heap()): heap(std::move(heap)) {}

    void check(std::vector<Value> vals)
    {
        for (Value v : vals)
            heap.push(v);

        std::sort(vals.begin(), vals.end(), *heap.comparator());
        for (Value expected : vals)
            ASSERT_EQ(expected, heap.takeTop());
        ASSERT_TRUE(heap.isEmpty());
    }

private:
    Heap heap;
};

#endif // SORTHELPER_H
