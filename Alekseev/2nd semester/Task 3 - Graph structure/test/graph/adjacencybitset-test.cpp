#include "utils.h"

#include <algorithm>

#include "adjacencybitset.h"

class AdjacencyBitSetTest : public AdjacencyTest
{
protected:
    std::vector<bool> makeBitSet(const std::vector<vertex_t> &v)
    {
        std::vector<bool> ret(*std::max_element(v.begin(), v.end()) + 1, 0);
        for (vertex_t i : v)
            ret[i] = true;
        return ret;
    }
};

TEST_F(AdjacencyBitSetTest, Empty)
{
    std::unique_ptr<Adjacency> adj(new AdjacencyBitSet(std::vector<bool>()));

    ASSERT_NO_FATAL_FAILURE(checkEquivalence(adj.get(), std::vector<vertex_t>()));
}

TEST_F(AdjacencyBitSetTest, CopyCtor)
{
    std::vector<vertex_t> v = {3, 1, 4, 8, 5};
    std::unique_ptr<Adjacency> adj(new AdjacencyBitSet(v.begin(), v.end()));

    ASSERT_NO_FATAL_FAILURE(checkEquivalence(adj.get(), v));
}

TEST_F(AdjacencyBitSetTest, MoveCtor)
{
    std::vector<vertex_t> v = {3, 1, 4, 8, 5};
    std::unique_ptr<Adjacency> adj(new AdjacencyBitSet(makeBitSet(v)));

    ASSERT_NO_FATAL_FAILURE(checkEquivalence(adj.get(), v));
}

TEST(AdjacencyBitSetDeathTest, RepeatingDestinations)
{
    std::vector<vertex_t> v = {3, 1, 4, 1, 5};
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

//TEST_F(AdjacencyBitSetTest, OutOfRangeDeathTest)
//{
//    ASSERT_DEATH({
//                     AdjacencyBitSet(std::vector<bool>(5, 3)).adjacentTo(10);
//                 }, "");
//}
