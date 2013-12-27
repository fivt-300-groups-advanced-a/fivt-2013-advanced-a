#include "gtest/gtest.h"

#include <random>

#include <limits>

#include "SegmentAssignmentTree.h"

#include "dummystructure.h"

TEST(Assignment, SmallManual)
{
    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};
//    std::size_t n = data.size();

    SegmentAssignmentTree<int> tree(data.begin(), data.end(),
                                            std::numeric_limits<int>::min(),
                                            std::numeric_limits<int>::max(),
                                            0);
    DummyStructure<int> dummy(data.begin(), data.end());

    ASSERT_EQ(dummy.sum(0, 7), tree.get(0, 7).sum);
    ASSERT_EQ(dummy.sum(1, 6), tree.get(1, 6).sum);

    std::vector< std::vector< std::vector<int> > > tests =
    {
        {{0, 7, 1}, {0, 7}},
        {{3, 4, 1}, {0, 7}},
        {{4, 6, 2}, {3, 6}}
    };
    for (auto test : tests)
    {
        dummy.assign(test[0][0], test[0][1], test[0][2]);
        tree.assign(test[0][0], test[0][1], test[0][2]);

        auto res = tree.get(test[1][0], test[1][1]);
        ASSERT_EQ(dummy.sum(test[1][0], test[1][1]), res.sum);
        ASSERT_EQ(dummy.min(test[1][0], test[1][1]), res.min);
        ASSERT_EQ(dummy.max(test[1][0], test[1][1]), res.max);
    }
}

TEST(Assignment, StressAssignGet)
{
    for (int seed = 0; seed < 100; ++seed)
    {
        std::mt19937 gen(seed);

        std::size_t n = std::abs((int)gen()) % 100;

        DummyStructure<int> dummy(n, 0);
        std::vector<int> init(n, 0);
        SegmentAssignmentTree<int> tree(init.begin(), init.end(),
                                                std::numeric_limits<int>::min() / 2,
                                                std::numeric_limits<int>::max() / 2,
                                                0);

        for (int i = 0; i < 100; ++i)
        {
            std::size_t l = std::labs(gen()) % n;
            std::size_t r = l + std::labs(gen()) % (n - l);
            int value = gen() % 1000;

            dummy.assign(l, r, value);
            tree.assign(l, r, value);
        }

        for (int i = 0; i < 100; ++i)
        {
            std::size_t l = std::labs(gen()) % n;
            std::size_t r = l + std::labs(gen()) % (n - l);

            auto res = tree.get(l, r);
            ASSERT_EQ(dummy.sum(l, r), res.sum);
            ASSERT_EQ(dummy.min(l, r), res.min);
            ASSERT_EQ(dummy.max(l, r), res.max);
        }
    }
}
