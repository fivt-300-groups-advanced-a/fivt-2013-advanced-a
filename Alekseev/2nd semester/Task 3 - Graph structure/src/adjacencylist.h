#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <vector>
#include <algorithm>

#include <cassert>

#include "adjacency.h"

namespace graph
{

class AdjacencyListIterator : public AdjacencyIterator
{
    typedef std::vector<vertex_t>::const_iterator vectorIter;

public:
    virtual ~AdjacencyListIterator() {}

    virtual vertex_t destination() const override
    {
        assert(isValid());
        return *it;
    }

    virtual bool advance() override
    {
        if (!isValid())
            return false;
        ++it;
        return true;
    }

    virtual bool isValid() const override
    {
        return it != end;
    }

protected:
    friend class AdjacencyList;

    AdjacencyListIterator(const vectorIter &it,
                          const vectorIter &end):
        it(it),
        end(end)
    {}

private:
    vectorIter it, end;
};

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
        return std::unique_ptr<AdjacencyIterator>(new AdjacencyListIterator(
                                                      vertices.cbegin(),
                                                      vertices.cend()));
    }

    virtual bool isConnectedTo(vertex_t vertex) const override
    {
        return std::binary_search(vertices.cbegin(), vertices.cend(), vertex);
    }

private:
    IdVector prepared(IdVector &&vert)
    {
        sort(vert.begin(), vert.end());
        assert(std::unique(vert.begin(), vert.end()) == vert.end());
        return std::move(vert);
    }

    const std::vector<vertex_t> vertices;
};

} // namespace graph

#endif // ADJACENCYLIST_H
