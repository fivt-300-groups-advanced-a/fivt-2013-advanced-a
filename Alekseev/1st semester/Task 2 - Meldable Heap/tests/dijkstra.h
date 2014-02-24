#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <random>
#include <iostream>

#include "testutils.h"

typedef long long ll;

struct Edge
{
    int from, to;
    ll w;
};

class Dijkstra
{
public:
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

    template<class HeapOfPairLLint>
    std::vector<ll> solve(int start)
    {
        typedef std::pair<ll, int> Pair;
        typedef HeapOfPairLLint Heap;

        std::vector<ll> rng(adj.size(), -1LL);
        std::vector<typename Heap::Index> ids(adj.size());

        Heap heap;

        rng[start] = 0;
        ids[start] = heap.emplace(rng[start], start);
        while (!heap.isEmpty())
        {
            Pair p = heap.takeTop();
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
                    ids[e.to] = heap.set(ids[e.to], Pair(p.first + e.w, e.to));
                }
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
