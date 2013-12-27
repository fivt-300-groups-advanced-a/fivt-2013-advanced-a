#include "gtest/gtest.h"

#include "SegmentTree.h"

#include <cmath>

#include <random>
#include <iostream>

namespace
{
typedef int Result;
typedef int Meta;

struct Function
{
    Function(int* counter): counter(counter) {}
    Result operator() (Result, Result)
    {
        if (counter)
            ++*counter;
        return 0;
    }

private:
    int* counter;
};

struct Update
{
    explicit Update(int* counter): counter(counter) {}
    void operator() (Meta&, Meta, std::size_t, std::size_t)
    {
        if (counter)
            ++*counter;
    }

private:
    int* counter;
};

struct Apply
{
    explicit Apply(int* counter): counter(counter) {}
    void operator() (Result&, Meta, std::size_t, std::size_t)
    {
        if (counter)
            ++*counter;
    }

private:
    int* counter;
};
}

class InternalCallsTest : public testing::Test
{
protected:
    static const std::size_t n;
    static const std::size_t queryCount;
    static const int expected;

    InternalCallsTest():
        tree(n, 0,
             Function(&funcCount),
             Update(&updateCount),
             Apply(&applyCount))
    {}

    virtual void SetUp()
    {
        funcCount = updateCount = applyCount = 0;
    }

    virtual void TearDown()
    {
        std::cout << "During " << queryCount << " queries:\n";
        std::cout << "Function called: " << funcCount <<
                     " Update called: " << updateCount <<
                     " Apply called: " << applyCount << std::endl;

        EXPECT_LT(funcCount, expected);
        EXPECT_LT(updateCount, expected);
        EXPECT_LT(applyCount, expected);
    }

    int funcCount, updateCount, applyCount;
    SegmentTree<Result, Meta, Function, Update, Apply> tree;
};

const std::size_t InternalCallsTest::n = 1<<20;
const std::size_t InternalCallsTest::queryCount = 1<<17;
const int InternalCallsTest::expected = InternalCallsTest::queryCount * (log(InternalCallsTest::n) / log(2)) * 9;

TEST_F(InternalCallsTest, RandomUpdates)
{
    const int seed = 42;
    std::mt19937 gen(seed);

    for (std::size_t i = 0; i < queryCount; ++i)
    {
        std::size_t l = std::labs(gen()) % n;
        std::size_t r = l + std::labs(gen()) % (n - l);

        tree.update(l, r, 0);
    }
}

TEST_F(InternalCallsTest, LocalUpdates)
{
    const int seed = 42;
    std::mt19937 gen(seed);

    for (std::size_t i = 0; i < queryCount; ++i)
    {
        std::size_t l = std::labs(gen()) % n;
        std::size_t r = l + std::labs(gen()) % std::min((std::size_t)5, n - l);

        tree.update(l, r, 0);
    }
}
