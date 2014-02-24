#include "gtest/gtest.h"

#include "leftistheap.h"
#include "testutils.h"

#include <random>

TEST(Melding, EmptyCases)
{
    Asserted<LeftistHeap<int>> a, b;
    a.absorb(b);

    for (int x : {3, 1, 4, 1, 5, 9, 2, 6})
        a.push(x);
    a.absorb(b);
    b.absorb(a);
}

TEST(Melding, ManySmallHeapsAbsorbingBigHeap)
{
    Asserted<LeftistHeap<int>> total, chunk;
    for (int i = 0; i < 100; ++i)
    {
        std::mt19937 gen(i);
        int n = std::abs((int)gen()) % 50;

        for (int j = 0; j < n; ++j)
            chunk.push(gen());

        chunk.absorb(total);
        total.absorb(chunk);
    }
}

TEST(Melding, BigHeapAbsorbingManySmallHeaps)
{
    Asserted<LeftistHeap<int>> total, chunk;
    for (int i = 0; i < 100; ++i)
    {
        std::mt19937 gen(i);
        int n = std::abs((int)gen()) % 50;

        for (int j = 0; j < n; ++j)
            chunk.push(gen());

        total.absorb(chunk);
    }
}
