#ifndef DEFAULTADJACENCYBUILDER_H
#define DEFAULTADJACENCYBUILDER_H

#include <memory>
#include <vector>
#include <climits>
#include <cassert>

#include "adjacency.h"

#include "adjacencybitset.h"
#include "adjacencylist.h"
#include "singleadjacency.h"
#include "emptyadjacency.h"

#ifdef TEST
class DefaultAdjacencyBuilderTest;
#endif

namespace graph
{

class DefaultAdjacencyBuilder
{
public:
    explicit DefaultAdjacencyBuilder(std::size_t V):
        V(V)
    {
        init();
    }

    std::unique_ptr<Adjacency> convertToAdjacency()
    {
        std::unique_ptr<Adjacency> ret;

        if (isCondensed)
            ret.reset(new AdjacencyBitSet(std::move(bitset)));
        else if (list.empty())
            ret.reset(new EmptyAdjacency());
        else if (list.size() == 1)
            ret.reset(new SingleAdjacency(list[0]));
        else
            ret.reset(new AdjacencyList(std::move(list)));

        init();

        return std::move(ret);
    }

    void addEdge(vertex_t dest)
    {
        assert(dest < V);
        if (isCondensed)
        {
            if (bitset[dest])
            {
                expand();
                isUnique = false;
                list.push_back(dest);
            }
            else
                bitset[dest] = 1;
        }
        else
        {
            list.push_back(dest);

            if (condenseWorth() && isUnique)
                tryCondense();
        }
    }

    template<class FwdIt>
    void addEdges(FwdIt begin, FwdIt end)
    {
        for (; begin != end; ++begin)
            addEdge(*begin);
    }

private:
#ifdef TEST
    friend class DefaultAdjacencyBuilderTest;
#endif

    void init()
    {
        list.clear();
        list.shrink_to_fit();
        bitset.clear();
        bitset.shrink_to_fit();
        isCondensed = false;
        isUnique = true;
    }

    bool condenseWorth()
    {
        return list.size() > 1 && list.size() * sizeof(vertex_t) > V / CHAR_BIT;
    }

    void expand()
    {
        assert(isCondensed);
        for (size_t i = 0; i < V; ++i)
            if (bitset[i])
                list.push_back(vertex_t(i));

        isCondensed = false;
        bitset.clear();
        bitset.shrink_to_fit();
    }

    bool tryCondense()
    {
        assert(!isCondensed);

        bitset.reserve(V);
        bitset.resize(V);

        for (vertex_t v : list)
        {
            if (bitset[v])
            {
                isUnique = false;
                bitset.clear();
                bitset.shrink_to_fit();
                return false;
            }
            bitset[v] = 1;
        }

        isCondensed = true;
        bitset.shrink_to_fit();
        list.clear();
        return true;
    }

    std::vector<vertex_t> list;
    std::vector<bool> bitset;

    bool isCondensed, isUnique;

    const std::size_t V;
};

}

#endif // DEFAULTADJACENCYBUILDER_H
