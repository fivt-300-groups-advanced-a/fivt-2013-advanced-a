#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>

#include "adjacency.h"

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
        adj(std::move(adj)),
        backAdj(std::move(backAdj))
    {}

    Graph& operator = (Graph &&graph)
    {
        adj = std::move(graph.adj);
        backAdj = std::move(graph.backAdj);
        return *this;
    }

    std::size_t size() const
    {
        return adj.size();
    }

    const Adjacency *getEdgesFrom(std::size_t vertex) const
    {
        return adj[vertex].get();
    }
    const Adjacency *getEdgesTo(std::size_t vertex) const
    {
        return backAdj[vertex].get();
    }

private:
    std::vector<std::unique_ptr<Adjacency>> adj, backAdj;
};

#endif // GRAPH_H
