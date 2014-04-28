#include "utils.h"

#include "singleadjacency.h"

typedef AdjacencyTest SingleAdjacencyTest;

TEST_F(SingleAdjacencyTest, Single)
{
    std::unique_ptr<Adjacency> adj(new SingleAdjacency(42));

    ASSERT_NO_FATAL_FAILURE(checkEquivalence(adj.get(), {42}));
}

TEST_F(SingleAdjacencyTest, DeathOnInvalidDereference)
{
    auto it = SingleAdjacency(42).makeIterator();
    it->advance();
    ASSERT_DEATH({it->destination();}, "");
}
