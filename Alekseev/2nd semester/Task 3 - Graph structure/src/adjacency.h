#ifndef ADJACENCY_H
#define ADJACENCY_H

#include <memory>

namespace graph
{

typedef std::size_t vertex_t;

class AdjacencyIterator
{
public:
    virtual ~AdjacencyIterator() {}

    virtual graph::vertex_t destination() const = 0;

    virtual bool advance() = 0;

    virtual bool isValid() const = 0;
};

class Adjacency
{
public:
    virtual ~Adjacency() {}

    virtual std::size_t size() const = 0;

    virtual std::unique_ptr<AdjacencyIterator> makeIterator() const = 0;

    virtual bool isConnectedTo(vertex_t vertex) const = 0;
};

} // namespace graph

#endif // ADJACENCY_H
