#include "gtest/gtest.h"

#include "leftistheap.h"
#include "dijkstra.h"

#include <typeinfo>

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
