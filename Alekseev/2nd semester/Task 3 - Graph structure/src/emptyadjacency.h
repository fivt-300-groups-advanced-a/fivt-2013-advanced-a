#ifndef EMPTYADJACENCY_H
#define EMPTYADJACENCY_H

#include "adjacency.h"

class EmptyAdjacencyIterator : public AdjacencyIterator
{
public:
    virtual ~EmptyAdjacencyIterator() {}

    virtual destination() const override
    {
        assert(false);
    }

    virtual bool advance() override
    {
        return false;
    }

    virtual bool isValid() const override
    {
        return false;
    }
};

class EmptyAdjacency : public Adjacency
{
public:
    virtual ~EmptyAdjacency() {}

    virtual std::size_t size() const override
    {
        return 0;
    }

    virtual std::unique_ptr<AdjacencyIterator> makeIterator() const override
    {
        return std::unique_ptr<AdjacencyIterator>(new EmptyAdjacencyIterator());
    }

    virtual bool isConnectedTo(std::size_t vertex) const override
    {
        return false;
    }
};

#endif // EMPTYADJACENCY_H
