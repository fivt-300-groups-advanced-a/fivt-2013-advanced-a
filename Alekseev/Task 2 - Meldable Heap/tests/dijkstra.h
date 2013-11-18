#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <random>
#include <iostream>

#include "leftistheap.h"

#include "testutils.h"

typedef long long ll;

struct Edge
{
    int from, to;
    ll w;
};

struct Dijkstra
{
    explicit Dijkstra (const std::vector<Edge>& edges)
    {
        int v = 0;
        for (Edge e : edges)
            v = std::max(v, std::max(e.from, e.to));
        adj.reserve(v+1);
        adj.resize(v+1);
        for (Edge e : edges)
            adj[e.from].push_back(e);
    }

    std::vector<ll> solve(int start)
    {
        typedef std::pair<ll, int> pair;

        std::vector<ll> rng(adj.size(), -1LL);
        std::vector<LeftistHeap<pair>::Index> ids(adj.size());

        rng[start] = 0;
        LeftistHeap<pair> heap;
        TestAccess<LeftistHeap<pair>> test(&heap);
        test.assertInvariants();
        ids[start] = heap.emplace(rng[start], start);
        test.assertInvariants();
//        std::cerr << test.printableTraverse() << "###################\n";
        while (!heap.isEmpty())
        {
            pair p = heap.takeTop();
            test.assertInvariants();
//            std::cerr << test.printableTraverse() << "###################\n";
            for (Edge e : adj[p.second])
            {
                if (rng[e.to] == -1)
                {
                    rng[e.to] = p.first + e.w;
                    ids[e.to] = heap.emplace(p.first + e.w, e.to);
                }
                else if (rng[e.to] > p.first + e.w)
                {
                    rng[e.to] = p.first + e.w;
                    ids[e.to] = heap.set(ids[e.to], pair(p.first + e.w, e.to));
                }
                test.assertInvariants();
//                std::cerr << test.printableTraverse() << "###################\n";
            }
        }
        return rng;
    }

private:
    std::vector<std::vector<Edge>> adj;
};

class RandomGraphGenerator
{
public:
    static std::vector<Edge> gen(int seed, int V, int E, ll maxW)
    {
        std::mt19937 gen(seed);
        std::vector<Edge> ret;
        for (int i = 0; i < E; ++i)
            ret.push_back({std::abs(int(gen())) % V, std::abs(int(gen())) % V, std::abs(ll(gen())) % maxW});
        return ret;
    }
};

#endif // DIJKSTRA_H
