#include <iostream>
#include <queue>
#include <gtest/gtest.h>
#include <utility>
#include <ctime>
#include <map>
#include "graph.h"

using namespace std;

TEST(asd, sdf)
{
    Graph g;
    g.print();
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    testing :: InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
