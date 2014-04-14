#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>

#include "adjacency.h"

namespace graph
{

class Graph
{
public:

    explicit Graph(const Graph &graph) = delete;
    Graph& operator = (const Graph &graph) = delete;

    explicit Graph(Graph &&graph):
        adj(std::move(graph.adj)),
        backAdj(std::move(graph.backAdj))
    {}
    explicit Graph(std::vector<std::unique_ptr<Adjacency>> &&adj):
        adj(std::move(adj))
    {}
    Graph(std::vector<std::unique_ptr<Adjacency>> &&adj,
          std::vector<std::unique_ptr<Adjacency>> &&backAdj):
        adj(std::forward(adj)),
        backAdj(std::forward(backAdj))
    {}

    Graph& operator = (Graph &&graph)
    {
        adj = std::move(graph.adj);
        backAdj = std::move(graph.backAdj);
        return *this;
    }

    std::size_t vertexCount() const
    {
        return adj.size();
    }

    const Adjacency *getEdgesFrom(vertex_t vertex) const
    {
        return adj[vertex].get();
    }
    const Adjacency *getEdgesTo(vertex_t vertex) const
    {
        return backAdj[vertex].get();
    }

private:
    std::vector<std::unique_ptr<Adjacency>> adj, backAdj;
};

} // namespace graph

#endif // GRAPH_H
