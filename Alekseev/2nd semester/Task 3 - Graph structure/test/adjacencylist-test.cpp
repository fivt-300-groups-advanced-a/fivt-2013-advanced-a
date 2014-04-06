#include "utils.h"

#include "adjacencylist.h"

class AdjacencyListTest : public AdjacencyTest
{
};

TEST_F(AdjacencyListTest, Empty)
{
    std::unique_ptr<Adjacency> adj(new AdjacencyList(std::vector<std::size_t>()));

    checkEquivalence(adj.get(), std::vector<std::size_t>());
}

TEST_F(AdjacencyListTest, CopyCtor)
{
    std::vector<std::size_t> v = {3, 1, 4, 8, 5};
    std::unique_ptr<Adjacency> adj(new AdjacencyList(v.begin(), v.end()));

    checkEquivalence(adj.get(), v);
}

TEST_F(AdjacencyListTest, MoveCtor)
{
    std::vector<std::size_t> v = {3, 1, 4, 8, 5}, u = v;
    std::unique_ptr<Adjacency> adj(new AdjacencyList(std::move(u)));

    checkEquivalence(adj.get(), v);
}

TEST(AdjacencyListDeathTest, RepeatingDestinations)
{
    std::vector<std::size_t> v = {3, 1, 4, 1, 5};
    ASSERT_DEATH({
                    AdjacencyList(v.begin(), v.end());
                 }, "");
}

TEST(AdjacencyListDeathTest, InvalidIteratorDereference)
{
    ASSERT_DEATH({
                     AdjacencyList(std::vector<std::size_t>()).makeIterator()->destination();
                 }, "");
}
