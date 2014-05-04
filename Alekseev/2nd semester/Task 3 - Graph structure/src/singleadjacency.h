#ifndef SINGLEADJACENCY_H
#define SINGLEADJACENCY_H

#include "adjacency.h"

namespace graph
{

class SingleAdjacencyIterator : public AdjacencyIterator
{
public:
    virtual ~SingleAdjacencyIterator() {}

    virtual vertex_t destination() const override
    {
        assert(valid);
        return dest;
    }

    virtual bool advance() override
    {
        if (!valid)
            return false;
        valid = false;
        return true;
    }

    virtual bool isValid() const override
    {
        return valid;
    }

protected:
    friend class SingleAdjacency;

    SingleAdjacencyIterator(vertex_t dest):
        dest(dest),
        valid(true)
    {}

private:
    vertex_t dest;
    bool valid;
};

class SingleAdjacency : public Adjacency
{
public:
    explicit SingleAdjacency(vertex_t dest):
        dest(dest)
    {}

    virtual ~SingleAdjacency() {}

    virtual std::size_t size() const override
    {
        return 1;
    }

    virtual std::unique_ptr<AdjacencyIterator> makeIterator() const override
    {
        return std::unique_ptr<AdjacencyIterator>(new SingleAdjacencyIterator(dest));
    }

    virtual bool adjacentTo(vertex_t vertex) const override
    {
        return vertex == dest;
    }

private:
    vertex_t dest;
};

} // namespace graph

#endif // SINGLEADJACENCY_H
