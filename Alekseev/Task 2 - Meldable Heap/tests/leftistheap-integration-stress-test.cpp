#include "gtest/gtest.h"

#include "leftistheap.h"
#include "dijkstra.h"
#include "sort.h"

#include <vector>
#include <random>
#include <algorithm>

#include <ctime>

TEST(Stress, Dijkstra)
{
    std::vector<std::vector<int>> cases
    {
        {10, 20, 20},
        {100, 2000, 200}
    };
    for (int i = 0; i < 1000; ++i)
    {
        auto param = cases[i%cases.size()];
        auto testcase = RandomGraphGenerator::gen(i, param[0], param[1], param[2]);

        Dijkstra d(testcase);
        d.solve<Asserted<LeftistHeap<std::pair<long long, int>>>>(0);
    }
}

TEST(Stress, SortPermutation)
{
    for (int i = 0; i < 20; ++i)
    {
        std::vector<int> perm;

        std::mt19937 gen(i);
        perm.resize(10000 + std::abs((int)gen()) % 500);
        for (size_t i = 0; i < perm.size(); ++i)
            perm[i] = i;
        std::shuffle(perm.begin(), perm.end(), gen);

        HeapSortChecker<Asserted<LeftistHeap<int>>> checker;
        checker.check(perm);
    }
}

TEST(Stress, SortSequence)
{
    for (int i = 0; i < 20; ++i)
    {
        std::vector<int> seq;

        std::mt19937 gen(i);
        seq.resize(10000 + std::abs((int)gen()) % 500);
        for (size_t i = 0; i < seq.size(); ++i)
            seq[i] = gen();

        HeapSortChecker<Asserted<LeftistHeap<int>>> checker;
        checker.check(seq);
    }
}

namespace
{
template<typename T, class Compare = std::less<T> >
class DummyHeap
{
public:
    typedef T Key;

    void push(T key)
    {
        v.push_back(key);
    }
    T takeTop()
    {
        auto it = std::min_element(v.begin(), v.end(), compare);
        T ret = *it;
        v.erase(it);
        return ret;
    }
    bool isEmpty() const
    {
        return v.empty();
    }

    Compare * comparator()
    {
        return &compare;
    }

private:
    std::vector<T> v;
    Compare compare;
};
}

TEST(DummyHeapBenchmark, SortSequence)
{
    for (int i = 0; i < 20; ++i)
    {
        std::vector<int> seq;

        std::mt19937 gen(i);
        int n = 10000 + std::abs((int)gen()) % 500;
        seq.resize(n);
        for (size_t i = 0; i < seq.size(); ++i)
            seq[i] = gen();

        HeapSortChecker<LeftistHeap<int>> leftistChecker;
        clock_t start = clock();
        leftistChecker.check(seq);
        double leftistTime = (double)(clock() - start) / CLOCKS_PER_SEC;

        HeapSortChecker<DummyHeap<int>> dummyChecker;
        start = clock();
        dummyChecker.check(seq);
        double dummyTime = (double)(clock() - start) / CLOCKS_PER_SEC;

        double profit = dummyTime / leftistTime;
        double expectedProfit = n / (log(n) / log(2.)) / 20.;
        EXPECT_GT(profit, expectedProfit);
    }
}
