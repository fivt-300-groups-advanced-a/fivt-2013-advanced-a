#include "utils.h"

#include <algorithm>

void checkEquivalence(const graph::Adjacency *adj, std::vector<vertex_t> src)
{
    ASSERT_EQ(src.size(), adj->size());

    std::sort(src.begin(), src.end());

    std::unique_ptr<graph::AdjacencyIterator> it = adj->makeIterator();
    ASSERT_NE(nullptr, it.get());

    for (std::size_t value : src)
    {
        ASSERT_TRUE(it->isValid());
        ASSERT_EQ(value, it->destination());
        ASSERT_TRUE(it->advance());
    }
    ASSERT_FALSE(it->isValid());
}

DummyGraph::DummyGraph(std::size_t V, const std::vector<std::pair<graph::vertex_t, graph::vertex_t> > &edges):
    V(V),
    edgeList(edges)
{
    adj.resize(V);
    backAdj.resize(V);
    for (const auto &e : edges)
    {
        adj[e.first].push_back(e.second);
        backAdj[e.second].push_back(e.first);
    }
}

DummyGraph DummyGraph::random(int seed, std::size_t V, std::size_t E)
{
    std::vector<std::pair<vertex_t, vertex_t>> edges(E);
    std::mt19937 gen(seed);
    for (size_t i = 0; i < E; ++i)
        edges[i] = std::make_pair(gen() % V, gen() % V);

    return DummyGraph(V, edges);
}

std::ostream& operator << (std::ostream &out, const DummyGraph &t)
{
    out << t.vertexCount() << " " << t.edges().size();
    for (const std::pair<vertex_t, vertex_t> &e : t.edges())
        out << " {" << e.first << ", " << e.second << "}";
    return out;
}
