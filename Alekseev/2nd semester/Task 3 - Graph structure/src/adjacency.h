#ifndef ADJACENCY_H
#define ADJACENCY_H

#include <memory>

class AdjacencyIterator
{
public:
    virtual ~AdjacencyIterator() {}

    virtual std::size_t destination() const = 0;

    virtual bool advance() = 0;

    virtual bool isValid() const = 0;
};

class Adjacency
{
public:
    virtual ~Adjacency() {}

    virtual std::size_t size() const = 0;

    virtual std::unique_ptr<AdjacencyIterator> makeIterator() const = 0;

    virtual bool isConnectedTo(std::size_t vertex) const = 0;
};

#endif // ADJACENCY_H
