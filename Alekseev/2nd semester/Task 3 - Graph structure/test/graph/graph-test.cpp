#include <vector>

#include "utils.h"

#include "graph.h"
#include "defaultadjacencybuilder.h"
#include "adjacencylist.h"

using namespace graph;

class GraphTest : public ::testing::TestWithParam<DummyGraph>
{
};

TEST_P(GraphTest, EdgeList)
{
    DummyGraph d = GetParam();
    std::vector<vertex_t> expected;
    std::vector<std::unique_ptr<Adjacency>> adj;
    for (size_t i = 0; i < d.vertexCount(); ++i)
    {
        std::vector<vertex_t> here = d.edgesFrom(i);
        std::sort(here.begin(), here.end());
        expected.insert(expected.end(), here.begin(), here.end());

        DefaultAdjacencyBuilder builder(d.vertexCount());
        builder.addEdges(here.begin(), here.end());
        adj.emplace_back(builder.convertToAdjacency());
    }

    Graph g(std::move(adj));

    EdgeListIterator it = g.makeEdgeListIterator();
    for (vertex_t x : expected)
    {
        ASSERT_TRUE(it.isValid());
        EXPECT_EQ(x, it.destination());
        ASSERT_TRUE(it.advance());
    }
    ASSERT_FALSE(it.isValid());
}

INSTANTIATE_TEST_CASE_P(Tests, GraphTest,
                        testing::Values(
                            DummyGraph(),
                            DummyGraph(5, {}),
                            DummyGraph(4, {{2, 3}, {2, 3}, {2, 1}, {1, 1}, {3, 1}}),
                            DummyGraph(4, {{0, 3}, {0, 3}, {0, 1}, {1, 1}, {3, 1}}),
                            DummyGraph::random(0, 10, 30),
                            DummyGraph::random(1, 5, 80)
                            ));
