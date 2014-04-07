#include <gtest/gtest.h>
#include <cstdio>
#include <utility>
#include <list>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <vector>
#include <algorithm>
#include <ctime>
#include <iostream>
#include "graph.h"

TEST(handmade, vectorInt) { 
    std::unique_ptr<Graph> G(new Graph(std::unique_ptr<ListFactory>(new Const<List<std::vector<int> > >()), 4));
    G->addEdge(1, 2);
    G->addEdge(0, 2);
    G->addEdge(1, 3);
    EXPECT_FALSE(G->isConnected(0, 1));
    EXPECT_TRUE(G->isConnected(0, 2));
    EXPECT_TRUE(G->isConnected(1, 3));
    EXPECT_TRUE(G->isConnected(1, 2));
    EXPECT_FALSE(G->isConnected(2, 3));
}


TEST(handmade, vectorBool) { 
    std::unique_ptr<Graph> G(new Graph(std::unique_ptr<ListFactory>(new Const<List<std::vector<bool> > >()), 4));
    G->addEdge(1, 2);
    G->addEdge(0, 2);
    G->addEdge(1, 3);
    EXPECT_FALSE(G->isConnected(0, 1));
    EXPECT_TRUE(G->isConnected(0, 2));
    EXPECT_TRUE(G->isConnected(1, 3));
    EXPECT_TRUE(G->isConnected(1, 2));
    EXPECT_FALSE(G->isConnected(2, 3));
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
