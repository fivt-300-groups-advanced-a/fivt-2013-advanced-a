#include <vector>
#include <iostream>

#include "gtest/gtest.h"
#include "utils.h"

TEST(Random, FirstVariant)
{
    std::vector< std::vector<int> > params =
    {
        {10, 20, 26},
        {10, 20, 26},
        {10, 20, 26},
        {10, 20, 2},
        {10, 20, 2},
        {1000, 2000, 10},
        {1000, 2000, 10},
        {1000, 2000, 10},
        {1000, 2000, 50},
        {1000, 2000, 50},
        {1000, 2000, 50},
        {1000, 2000, 50},
        {1000, 2000, 5000},
        {1000, 2000, 1000000},
        {1000, 2000, 2}
    };

    for (int i = 0; i < (int)params.size(); ++i)
        TestCase::random(i, params[i][0], params[i][1], params[i][2]).checkPrescription();
}
TEST(Random, Special)
{
    TestCase t = TestCase::random(1, 10000, 10000, 2);
    std::cerr << calcPartial(t.src.begin(), t.src.end(),
                             t.dest.begin(), t.dest.end(), std::equal_to<int>()).back() << "\n";
}

TEST(Random, SecondVariant)
{
    std::vector< std::vector<int> > params =
    {
        {10, 20, 26, 5, 10},
        {10, 20, 26, 10, 15},
        {10, 20, 26, 15, 25},
        {10, 20, 2, 5, 10},
        {10, 20, 2, 15, 25},
        {1000, 2000, 10, 1000, 2000},
        {1000, 2000, 10, 500, 1500},
        {1000, 2000, 10, 3000, 5000},
        {1000, 2000, 50, 3000, 5000},
        {1000, 2000, 50, 2000, 4000},
        {1000, 2000, 50, 3000, 5000},
        {1000, 2000, 50, 3000, 5000},
        {1000, 5000, 5000, 3000, 5000},
        {1000, 2000, 1000000, 3000, 5000},
        {1000, 2000, 2, 3000, 7000}
    };

    for (int i = 0; i < (int)params.size(); ++i)
        TestCase::random2(i, params[i][0], params[i][1],
                params[i][2], params[i][3], params[i][4]).checkPrescription();
}

TEST(Random, Stress)
{
    for (int i = 0; i < 500; ++i)
        TestCase::random(i, 5, 100, 30).checkPrescription();
}
