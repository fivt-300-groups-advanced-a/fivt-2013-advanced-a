#include "gtest/gtest.h"

#include <vector>

#include "../src/MaxExpression.h"
#include "dummy.h"

TEST(Manual, Dummy)
{
    std::vector< std::pair<std::vector<int>, int> > testcases =
    {
        {{0, 0, 0}, 0},
        {{1, 1, 1, 0, 2}, 5},
        {{3, 1, 4}, 12},
        {{3,-1, 4}, 6}
    };

    for (auto test : testcases)
        EXPECT_EQ(test.second, dummyMaxExpression(test.first.begin(), test.first.end()));
}

TEST(Manual, Model)
{
    std::vector< std::pair<std::vector<int>, int> > testcases =
    {
        {{0, 0, 0}, 0},
        {{1, 1, 1, 0, 2}, 5},
        {{3, 1, 4}, 12},
        {{3,-1, 4}, 6}
    };

    for (auto test : testcases)
        EXPECT_EQ(test.second, maxExpression(test.first.begin(), test.first.end()));
}
