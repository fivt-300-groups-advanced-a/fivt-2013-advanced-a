#include "utils.h"

#include "singleadjacency.h"

class SingleAdjacencyTest : public AdjacencyTest
{
};

TEST_F(SingleAdjacencyTest, Single)
{
    std::unique_ptr<Adjacency> adj(new SingleAdjacency(42));

    checkEquivalence(adj.get(), {42});
}

TEST_F(SingleAdjacencyTest, InvalidDereferenceDeath)
{
    auto it = SingleAdjacency(42).makeIterator();
    it->advance();
    ASSERT_DEATH({it->destination();}, "");
}
