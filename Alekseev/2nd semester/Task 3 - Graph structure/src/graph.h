#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>

#include "adjacency.h"

namespace graph
{

class EdgeListIterator
{
public:
    explicit EdgeListIterator(const EdgeListIterator &it) = delete;
    EdgeListIterator& operator = (const EdgeListIterator &it) = delete;

    EdgeListIterator(EdgeListIterator &&it):
        adjIt(std::move(it.adjIt)),
        endIt(std::move(it.endIt)),
        vIt(std::move(it.vIt))
    {}
    EdgeListIterator& operator = (EdgeListIterator &&it)
    {
        adjIt = std::move(it.adjIt);
        endIt = std::move(it.endIt);
        vIt = std::move(it.vIt);
        return *this;
    }

    vertex_t destination() const
    {
        assert(isValid());
        return vIt->destination();
    }
    vertex_t operator*() const
    {
        return destination();
    }

    bool advance()
    {
        if (!vIt->isValid())
            return false;
        vIt->advance();
        validate();
        return true;
    }
    bool isValid() const
    {
        return adjIt != endIt && vIt->isValid();
    }

private:
    friend class Graph;

    EdgeListIterator(std::vector<std::unique_ptr<Adjacency>>::const_iterator adjIt,
                     std::vector<std::unique_ptr<Adjacency>>::const_iterator endIt):
        adjIt(adjIt),
        endIt(endIt),
        vIt(adjIt != endIt ? (*adjIt)->makeIterator() : nullptr)
    {
        validate();
    }

    bool validate()
    {
        while (adjIt != endIt && !vIt->isValid())
        {
            ++adjIt;
            if (adjIt != endIt)
                vIt = (*adjIt)->makeIterator();
        }
        return adjIt != endIt;
    }

    std::vector<std::unique_ptr<Adjacency>>::const_iterator adjIt, endIt;
    std::unique_ptr<AdjacencyIterator> vIt;
};

class Graph
{
public:
    explicit Graph(const Graph &graph) = delete;
    Graph& operator = (const Graph &graph) = delete;

    Graph() {}

    explicit Graph(Graph &&graph) = default;
//        adj(std::move(graph.adj)),
//        backAdj(std::move(graph.backAdj))
//    {}
    explicit Graph(std::vector<std::unique_ptr<Adjacency>> &&adj):
        adj(std::move(adj))
    {}
    Graph(std::vector<std::unique_ptr<Adjacency>> &&adj,
          std::vector<std::unique_ptr<Adjacency>> &&backAdj):
        adj(std::move(adj)),
        backAdj(std::move(backAdj))
    {}

    Graph& operator = (Graph &&graph) = default;
//    {
//        adj = std::move(graph.adj);
//        backAdj = std::move(graph.backAdj);
//        return *this;
//    }

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

    EdgeListIterator makeEdgeListIterator() const
    {
        return EdgeListIterator(adj.cbegin(), adj.cend());
    }

private:
    std::vector<std::unique_ptr<Adjacency>> adj, backAdj;
};

} // namespace graph

#endif // GRAPH_H
