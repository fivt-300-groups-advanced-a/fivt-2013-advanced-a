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
/*
    virtual bool equals(AdjacencyIterator *that) const
    {
        return false;
    }
*/
};

template<class FwdIter>
class AdjacencyStdIterator : public AdjacencyIterator
{
public:
    AdjacencyStdIterator(FwdIter it, FwdIter end):
        it(it),
        end(end)
    {}

    virtual ~AdjacencyStdIterator() {}

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

private:
    FwdIter it, end;
};

class Adjacency
{
public:
    virtual ~Adjacency() {}

    virtual std::size_t size() const = 0;

    virtual std::unique_ptr<AdjacencyIterator> makeIterator() const = 0;

    virtual bool adjacentTo(vertex_t vertex) const = 0;
};

} // namespace graph

#endif // ADJACENCY_H
