#include "gtest/gtest.h"

#include "emptyadjacency.h"

using namespace graph;

TEST(EmptyAdjacency, Empty)
{
    std::unique_ptr<Adjacency> adj(new EmptyAdjacency);

    ASSERT_EQ(0, adj->size());

    std::unique_ptr<AdjacencyIterator> it(adj->makeIterator());
    ASSERT_NE(nullptr, it.get());

    ASSERT_FALSE(it->isValid());
    ASSERT_FALSE(it->advance());

    ASSERT_DEATH({it->destination();}, "");
}
