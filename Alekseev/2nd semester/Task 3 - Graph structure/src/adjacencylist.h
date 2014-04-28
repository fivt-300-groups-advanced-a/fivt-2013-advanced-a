#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <vector>
#include <algorithm>

#include <cassert>

#include "adjacency.h"

namespace graph
{

class AdjacencyList : public Adjacency
{
    typedef std::vector<vertex_t> IdVector;

public:
    template<class FwdIt>
    AdjacencyList(FwdIt begin, FwdIt end):
        AdjacencyList(IdVector(begin, end))
    {}
    explicit AdjacencyList(IdVector &&vertices):
        vertices(prepared(std::move(vertices)))
    {}

    ~AdjacencyList() {}

    virtual std::size_t size() const override
    {
        return vertices.size();
    }

    virtual std::unique_ptr<AdjacencyIterator> makeIterator() const override
    {
        return std::unique_ptr<AdjacencyIterator>(new AdjacencyStdIterator<std::vector<vertex_t>::const_iterator>(
                                                      vertices.cbegin(),
                                                      vertices.cend()));
    }

    virtual bool adjacentTo(vertex_t vertex) const override
    {
        return std::binary_search(vertices.cbegin(), vertices.cend(), vertex);
    }

private:
    IdVector prepared(IdVector &&vert)
    {
        if (!std::is_sorted(vert.begin(), vert.end()))
            std::sort(vert.begin(), vert.end());
//        assert(std::unique(vert.begin(), vert.end()) == vert.end());
        return vert;
    }

    const std::vector<vertex_t> vertices;
};

} // namespace graph

#endif // ADJACENCYLIST_H
