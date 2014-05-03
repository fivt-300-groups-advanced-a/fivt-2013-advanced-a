#ifndef UTILS_H
#define UTILS_H

#include <vector>

#include "gtest/gtest.h"

#include "adjacency.h"

using namespace graph;

class AdjacencyTest : public ::testing::Test
{
};

void checkEquivalence(const Adjacency *adj, std::vector<vertex_t> src);

class DummyGraph
{
public:
    DummyGraph()
    {
    }

    DummyGraph(std::size_t V, const std::vector<std::pair<vertex_t, vertex_t>> &edges);

    static DummyGraph random(int seed, std::size_t V, std::size_t E);

    std::size_t vertexCount() const
    {
        return V;
    }
    std::size_t edgesCount() const
    {
        return edgeList.size();
    }

    const std::vector<std::vector<vertex_t> > &edgesFrom() const
    {
        return adj;
    }
    const std::vector<vertex_t> &edgesFrom(vertex_t from) const
    {
        return adj[from];
    }
    const std::vector<std::vector<vertex_t> > &edgesTo() const
    {
        return backAdj;
    }
    const std::vector<vertex_t> &edgesTo(vertex_t to) const
    {
        return backAdj[to];
    }
    const std::vector<std::pair<vertex_t, vertex_t>> &edges() const
    {
        return edgeList;
    }

private:
    std::size_t V;
    std::vector<std::vector<vertex_t>> adj, backAdj;
    std::vector<std::pair<vertex_t, vertex_t>> edgeList;
};

std::ostream& operator << (std::ostream &out, const DummyGraph &t);


#endif // UTILS_H
