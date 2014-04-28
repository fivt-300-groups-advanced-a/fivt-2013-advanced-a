#ifndef ALGO_H
#define ALGO_H

#include "graph.h"
#include "defaultadjacencybuilder.h"

namespace graph
{

namespace StrongConnectivity
{

typedef int ComponentId;

namespace Implementation
{

struct VertexInfo
{
    ComponentId color;
    unsigned int inTime, upTime;
};

void findComponentsTarjan(vertex_t v,
                          const Graph& g,
                          std::vector<VertexInfo>& info,
                          std::vector<vertex_t>& stack,
                          unsigned int& time)
{
    info[v].inTime = info[v].upTime = ++time;
    for (auto it = g.getEdgesFrom(v)->makeIterator(); it->isValid(); it->advance())
    {
        vertex_t u = it->destination();
        if (info[i].inTime)
        {
            info[v].upTime = min(info[v].inTime, info[u].inTime);
            continue;
        }
        findComponentsTarjan(u, g, info, stack);
        info[v].upTime = min(info[v].upTime, info[u].upTime);
    }
}

} // namespace graph::StrongConnectivity::Implementation

std::vector<ComponentId> findComponents(const Graph &g)
{
    std::vector<ComponentId> ret(g.vertexCount(), -1);
    if (g.vertexCount() == 0)
        return ret;

    std::vector<vertex_t> stack;
    std::vector<Implementation::VertexInfo> info(g.vertexCount(), {-1, 0, 0});
    unsigned int time = 0;

    for (vertex_t i = 0; i < g.vertexCount(); ++i)
        if (!info[i].inTime)
            Implementation::findComponentsTarjan(i, g, info, stack, time);

    return ret;
}

struct Condensation
{
    Condensation(Graph &&g, const std::vector<ComponentId> &color, const std::vector<vertex_t> &representer):
        g(std::move(g)), color(color), representer(representer)
    {}

    const Graph g;
    const std::vector<ComponentId> color;
    const std::vector<vertex_t> representer;
};

} // namespace graph::StrongConnectivity

} // namespace graph

#endif // ALGO_H
