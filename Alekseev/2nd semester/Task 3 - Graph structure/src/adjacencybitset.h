#ifndef ADJACENCYBITSET_H
#define ADJACENCYBITSET_H

#include <vector>
#include <algorithm>

#include "adjacency.h"

namespace graph
{

class AdjacencyBitSetIterator : public AdjacencyIterator
{
public:
    virtual ~AdjacencyBitSetIterator() {}

    virtual vertex_t destination() const override
    {
        assert(isValid());
        return pos;
    }
    virtual bool advance() override
    {
        if (!isValid())
            return false;
        pos = findPopBit(pos + 1);
        return true;
    }
    virtual bool isValid() const override
    {
        return pos < bits.size();
    }

protected:
    friend class AdjacencyBitSet;

    AdjacencyBitSetIterator(std::size_t pos,
                            const std::vector<bool> &bits):
        bits(bits),
        pos(findPopBit(pos))
    {}

    std::size_t findPopBit(std::size_t start)
    {
        while (start < bits.size() && !bits[start])
            ++start;
        return start;
    }

private:
    const std::vector<bool> &bits;
    std::size_t pos;
};

class AdjacencyBitSet : public Adjacency
{
public:
    template<class FwdIter>
    AdjacencyBitSet(FwdIter begin, FwdIter end):
        bits(makeBitSet(begin, end)),
        size_(std::distance(begin, end))
    {}
    explicit AdjacencyBitSet(std::vector<bool> &&bits):
        bits(bits),
        size_(std::count(this->bits.cbegin(), this->bits.cend(), true))
    {}

    virtual ~AdjacencyBitSet()
    {}

    virtual std::size_t size() const override
    {
        return size_;
    }

    virtual std::unique_ptr<AdjacencyIterator> makeIterator() const override
    {
        return std::unique_ptr<AdjacencyIterator>(new AdjacencyBitSetIterator(0, bits));
    }

    virtual bool adjacentTo(vertex_t vertex) const override
    {
//        assert(vertex < bits.size());
        return vertex < bits.size() && bits[vertex];
    }

private:
    template<class FwdIter>
    std::vector<bool> makeBitSet(FwdIter begin, FwdIter end)
    {
        std::vector<bool> ret;
        for (; begin != end; ++begin)
        {
            if (ret.size() <= *begin)
                ret.resize(*begin + 1, 0);
            assert(!ret[*begin]);
            ret[*begin] = true;
        }
        ret.shrink_to_fit();
        return ret;
    }

    const std::vector<bool> bits;
    const std::size_t size_;
};

} // namespace graph

#endif // ADJACENCYBITSET_H
