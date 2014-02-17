#include "gtest/gtest.h"

#include "dummy.h"
#include "../src/MaxExpression.h"

#include <vector>
#include <random>

struct TestCase
{
    TestCase(int seed, int minSize, int maxSize)
    {
        std::mt19937 gen(seed);

        int n = minSize + gen() % (maxSize - minSize + 1);

        seq.reserve(n);
        seq.resize(n);

        for (int& x : seq)
            x = (gen()&1 ? 1 : -1) * int(gen() % 8);

        ans = dummyMaxExpression(seq.begin(), seq.end());
    }

    std::vector<int> seq;
    int ans;
};

TEST(Stress, Stress)
{
    for (int i = 0; i < 200; ++i)
    {
        TestCase test(i, 1, 15);

        ASSERT_EQ(test.ans, maxExpression(test.seq.begin(), test.seq.end())) << "failed with seed" << i;
    }
}
