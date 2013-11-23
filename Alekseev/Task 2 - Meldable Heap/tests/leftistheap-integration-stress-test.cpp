#include "gtest/gtest.h"

#include "leftistheap.h"
#include "dijkstra.h"

TEST(Stress, Dijkstra)
{
    std::vector<std::vector<int>> cases
    {
        {10, 20, 20},
        {100, 2000, 200}
    };
    for (int i = 0; i < 1000; ++i)
    {
        auto testcase = RandomGraphGenerator::gen(i, cases[i%cases.size()][0], cases[i%cases.size()][1], cases[i%cases.size()][2]);

        Dijkstra d(testcase);
        d.solve(0);
    }
}
