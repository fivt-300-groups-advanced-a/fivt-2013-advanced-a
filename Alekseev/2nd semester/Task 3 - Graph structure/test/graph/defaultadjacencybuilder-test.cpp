#include <random>

#include "gtest/gtest.h"

#include "utils.h"

#include "defaultadjacencybuilder.h"
#include "graph.h"

namespace graph
{
class DefaultAdjacencyBuilderTest: public testing::TestWithParam<DummyGraph>
{
protected:
    static const size_t bitsetCapacityTolerance;
    static const size_t emptyListCapacity;

    virtual void SetUp() override
    {
        const DummyGraph &t = GetParam();
        std::vector<std::unique_ptr<Adjacency>> adj, backAdj;

        for (std::size_t i = 0; i < t.vertexCount(); ++i)
        {
            DefaultAdjacencyBuilder builder(t.vertexCount());

            for (vertex_t dest : t.edgesFrom(i))
                builder.addEdge(dest);

            EXPECT_GE(builder.bitset.size() + bitsetCapacityTolerance, builder.bitset.capacity());

            adj.emplace_back(builder.convertToAdjacency());

            ASSERT_EQ(0, builder.list.size());
            ASSERT_EQ(0, builder.bitset.size());
            EXPECT_GE(emptyListCapacity, builder.list.capacity());
            EXPECT_GE(builder.bitset.size() + bitsetCapacityTolerance, builder.bitset.capacity());

            for (vertex_t dest : t.edgesTo(i))
                builder.addEdge(dest);
            backAdj.emplace_back(std::move(builder.convertToAdjacency()));
        }

        g = Graph(std::move(adj), std::move(backAdj));
    }

    enum AdjacencyType
    {
        EMPTY_ADJACENCY,
        SINGLE_ADJACENCY,
        ADJACENCY_LIST,
        ADJACENCY_BITSET
    };

    static AdjacencyType properAdjacencyType(std::size_t V, const std::vector<vertex_t> &adj)
    {
        if (adj.empty())
            return EMPTY_ADJACENCY;
        if (adj.size() == 1)
            return SINGLE_ADJACENCY;
        if (adj.size() * sizeof(vertex_t) > V / CHAR_BIT)
        {
            std::vector<bool> met(V);
            for (vertex_t x : adj)
            {
                if (met[x])
                    return ADJACENCY_LIST;
                met[x] = 1;
            }
            return ADJACENCY_BITSET;
        }
        return ADJACENCY_LIST;
    }

    static AdjacencyType actualAdjacencyType(const Adjacency *adj)
    {
        if (dynamic_cast<const EmptyAdjacency*>(adj) != nullptr)
            return EMPTY_ADJACENCY;
        if (dynamic_cast<const SingleAdjacency*>(adj) != nullptr)
            return SINGLE_ADJACENCY;
        if (dynamic_cast<const AdjacencyList*>(adj) != nullptr)
            return ADJACENCY_LIST;
        if (dynamic_cast<const AdjacencyBitSet*>(adj) != nullptr)
            return ADJACENCY_BITSET;
        assert(false);
    }

    Graph g;
};

const size_t DefaultAdjacencyBuilderTest::bitsetCapacityTolerance =
        []{std::vector<bool> v; v.reserve(1); return v.capacity(); }();
const size_t DefaultAdjacencyBuilderTest::emptyListCapacity =
        []{std::vector<int> v; return v.capacity(); }();

TEST_P(DefaultAdjacencyBuilderTest, Correctness)
{
    const DummyGraph &t = GetParam();

    for (std::size_t i = 0; i < t.vertexCount(); ++i)
    {
        ASSERT_NO_FATAL_FAILURE(checkEquivalence(g.getEdgesFrom(i), t.edgesFrom(i)));
        ASSERT_NO_FATAL_FAILURE(checkEquivalence(g.getEdgesTo(i), t.edgesTo(i)));
    }
}

TEST_P(DefaultAdjacencyBuilderTest, Optimality)
{
    const DummyGraph &t = GetParam();

    for (size_t i = 0; i < t.vertexCount(); ++i)
    {
        auto proper = properAdjacencyType(t.vertexCount(), t.edgesFrom(i));
        auto actual = actualAdjacencyType(g.getEdgesFrom(i));
        EXPECT_EQ(proper, actual);
    }
}

INSTANTIATE_TEST_CASE_P(Tests,
                        DefaultAdjacencyBuilderTest,
                        testing::Values(
                            DummyGraph(0, {}),
                            DummyGraph(20, {}),

                            DummyGraph(3, {{0, 1}, {1, 2}, {0, 2}, {1, 0}}),
                            DummyGraph(4, {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {2, 0}}),
                            DummyGraph(5, {{0, 1}, {0, 0}, {0, 2}, {0, 3}, {1, 2}, {1, 2}, {1, 3}, {4, 4}}),

                            DummyGraph::random(1, 3, 9),
                            DummyGraph::random(2, 5, 10),
                            DummyGraph::random(3, 5, 10),
                            DummyGraph::random(4, 10, 1000),
                            DummyGraph::random(5, 20, 100),
                            DummyGraph::random(6, 30, 10),
                            DummyGraph::random(7, 40, 1000),
                            DummyGraph::random(8, 50, 2000),
                            DummyGraph::random(9, 100, 100)
                            ));
}
