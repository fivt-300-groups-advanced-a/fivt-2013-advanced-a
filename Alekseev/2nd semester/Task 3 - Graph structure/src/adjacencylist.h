#ifndef ADJACENCYLIST_H
#define ADJACENCYLIST_H

#include <vector>
#include <algorithm>

#include <cassert>

#include "adjacency.h"

class AdjacencyListIterator : public AdjacencyIterator
{
    typedef std::vector<std::size_t>::const_iterator vectorIter;

public:
    AdjacencyListIterator(const vectorIter &it,
                          const vectorIter &end):
        it(it),
        end(end)
    {}

    virtual ~AdjacencyListIterator() {}

    virtual std::size_t destination() const override
    {
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
private:
    vectorIter it, end;
};

class AdjacencyList : public Adjacency
{
    typedef std::vector<std::size_t> IdVector;

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

    virtual bool isConnectedTo(std::size_t vertex) const override
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

    const std::vector<std::size_t> vertices;
};

#endif // ADJACENCYLIST_H
