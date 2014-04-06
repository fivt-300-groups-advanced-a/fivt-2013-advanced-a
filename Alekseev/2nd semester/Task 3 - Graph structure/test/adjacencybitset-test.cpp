#include "utils.h"

#include "adjacencybitset.h"

class AdjacencyBitSetTest : public AdjacencyTest
{
};
std::vector<bool> makeBitSet(const std::vector<std::size_t> &v)
{
    std::vector<bool> ret(*max_element(v.begin(), v.end()) + 1, 0);
    for (std::size_t i : v)
        ret[i] = true;
    return ret;
}

TEST_F(AdjacencyBitSetTest, Empty)
{
    std::unique_ptr<Adjacency> adj(new AdjacencyBitSet(std::vector<bool>()));

    checkEquivalence(adj.get(), std::vector<std::size_t>());
}

TEST_F(AdjacencyBitSetTest, CopyCtor)
{
    std::vector<std::size_t> v = {3, 1, 4, 8, 5};
    std::unique_ptr<Adjacency> adj(new AdjacencyBitSet(v.begin(), v.end()));

    checkEquivalence(adj.get(), v);
}

TEST_F(AdjacencyBitSetTest, MoveCtor)
{
    std::vector<std::size_t> v = {3, 1, 4, 8, 5};
    std::unique_ptr<Adjacency> adj(new AdjacencyBitSet(makeBitSet(v)));

    checkEquivalence(adj.get(), v);
}

TEST(AdjacencyBitSetDeathTest, RepeatingDestinations)
{
    std::vector<std::size_t> v = {3, 1, 4, 1, 5};
    ASSERT_DEATH({
                    AdjacencyBitSet(v.begin(), v.end());
                 }, "");
}

TEST(AdjacencyBitSetDeathTest, InvalidIteratorDereference)
{
    ASSERT_DEATH({
                     AdjacencyBitSet(std::vector<bool>()).makeIterator()->destination();
                 }, "");
}
