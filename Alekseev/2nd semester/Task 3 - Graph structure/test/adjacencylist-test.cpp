#include "gtest/gtest.h"

#include "adjacencylist.h"

class AdjacencyListTest : public ::testing::Test
{
protected:
    void checkEquivalence(Adjacency *adj, std::vector<std::size_t> src)
    {
        ASSERT_EQ(src.size(), adj->size());

        sort(src.begin(), src.end());

        std::unique_ptr<AdjacencyIterator> it = adj->makeIterator();

        for (std::size_t value : src)
        {
            ASSERT_TRUE(it->isValid());
            ASSERT_EQ(value, it->destination());
            ASSERT_TRUE(it->advance());
        }
        ASSERT_FALSE(it->isValid());
    }
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

TEST(AdjacencyListDeathTest, RepeatingDests)
{
    std::vector<std::size_t> v = {3, 1, 4, 1, 5};
    ASSERT_DEATH({
                    std::unique_ptr<Adjacency> adj(new AdjacencyList(v.begin(), v.end()));
                 }, "");
}
