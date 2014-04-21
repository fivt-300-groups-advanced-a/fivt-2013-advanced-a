//
//  Tarjan.h
//  Graph
//
//  Created by Алексей Подкин on 21.04.14.
//  Copyright (c) 2014 Алексей Подкин. All rights reserved.
//

#ifndef Graph_Tarjan_h
#define Graph_Tarjan_h

#include "GraphStorage.h"
#include <vector>
#include <stack>
#include <limits.h>

class SCCTarjan
{
public:
    SCCTarjan(const Graph &gr, int n)
    {
        graph = gr;
        graph_lenght = n;
    }
    
    std::vector<std::vector<int> > scc()
    {
        time = 0;
        low_link.resize(graph_lenght);
        used.resize(graph_lenght);
        for (int i = 0; i < graph_lenght; i++)
        {
            if (!used[i])
            {
                dfs(i);
            }
        }
        return components;
    }
private:
    int time;
    int graph_lenght;
    Graph graph;
    std::vector<int> low_link;
    std::vector<bool> used;
    std::stack<int> stack;
    std::vector <std::vector<int> > components;
    
    void dfs(int vertex)
    {
        low_link[vertex] = time++;
        used[vertex] = true;
        stack.push(vertex);
        bool is_root = true;
        const ListOfIncidentVerteces &list = graph.getIncidentVerteces(vertex);
        auto it = std::move(list.getIterator());
        do
        {
            int to = it->getIncidentVertex();
            if (!used[to])
            {
                dfs(to);
            }
            if (low_link[vertex] > low_link[to]) {
                low_link[vertex] = low_link[to];
                is_root = false;
            }
        }
        while (it->nextVertexWithEdge() != false);
        if (is_root)
        {
            std::vector<int> component;
            while (true)
            {
                int t = stack.top();
                stack.pop();
                component.push_back(t);
                low_link[t] = INT_MAX;
                if (t == vertex)
                    break;
            }
            components.push_back(component);
        }
    }
};

#endif
