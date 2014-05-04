#include "utils.h"

#include "adjacencylist.h"

typedef AdjacencyTest AdjacencyListTest;

TEST_F(AdjacencyListTest, Empty)
{
    std::unique_ptr<Adjacency> adj(new AdjacencyList(std::vector<vertex_t>()));

    ASSERT_NO_FATAL_FAILURE(checkEquivalence(adj.get(), std::vector<vertex_t>()));
}

TEST_F(AdjacencyListTest, CopyCtor)
{
    std::vector<vertex_t> v = {3, 1, 4, 8, 5};
    std::unique_ptr<Adjacency> adj(new AdjacencyList(v.begin(), v.end()));

    ASSERT_NO_FATAL_FAILURE(checkEquivalence(adj.get(), v));
}

TEST_F(AdjacencyListTest, MoveCtor)
{
    std::vector<vertex_t> v = {3, 1, 4, 8, 5}, u = v;
    std::unique_ptr<Adjacency> adj(new AdjacencyList(std::move(u)));

    ASSERT_NO_FATAL_FAILURE(checkEquivalence(adj.get(), v));
}

//TEST(AdjacencyListDeathTest, RepeatingDestinations)
//{
//    std::vector<vertex_t> v = {3, 1, 4, 1, 5};
//    ASSERT_DEATH({
//                    AdjacencyList(v.begin(), v.end());
//                 }, "");
//}

TEST(AdjacencyListDeathTest, InvalidIteratorDereference)
{
    ASSERT_DEATH({
                     AdjacencyList(std::vector<vertex_t>()).makeIterator()->destination();
                 }, "");
}
