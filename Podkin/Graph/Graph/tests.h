//
//  tests.h
//  Graph
//
//  Created by Алексей Подкин on 21.04.14.
//  Copyright (c) 2014 Алексей Подкин. All rights reserved.
//

#ifndef Graph_tests_h
#define Graph_tests_h

#include <iostream>
#include "Tarjan.h"
#include <gtest/gtest.h>
#include <time.h>
#include <vector>
#include <memory>
#include "Factory.h"

TEST(GraphStorageTesting, BitSetMethod)
{
    srand(42);
    int number_of_tests = 10, t = 0;
    while (t < number_of_tests)
    {
        std::vector <bool> matrix;
        int number_of_verteces = rand() % 100 + 10;
        matrix.resize(number_of_verteces);
        for (int i = 0; i < number_of_verteces; i++)
        {
            matrix[i] = rand() % 2;
        }
        BitSetMethod list_of_incidents(matrix);
        for (int i = 0; i < number_of_verteces; i++)
        {
            ASSERT_EQ(list_of_incidents.isConnected(i), matrix[i]);
        }
        t++;
    }
}

TEST(GraphStorageTesting, StandartListMethod)
{
    srand(42);
    int number_of_tests = 10, t = 0;
    while (t < number_of_tests)
    {
        std::vector <int> matrix;
        std::vector <bool> bit_set;
        int number_of_verteces = rand() % 100 + 10, number_of_incident_verteces = number_of_verteces / 2;
        matrix.resize(number_of_incident_verteces);
        bit_set.resize(number_of_verteces, 0);
        for (int i = 0; i < number_of_incident_verteces; i++)
        {
            matrix[i] = rand() % number_of_verteces;
            bit_set[matrix[i]] = 1;
        }
        StandartListMethod list_of_incidents = StandartListMethod(matrix, 0);
        for (int i = 0; i < number_of_verteces; i++)
        {
            ASSERT_EQ(list_of_incidents.isConnected(i), bit_set[i]);
        }
        t++;
    }
}

TEST(GraphStorageTesting, FunctionalMethod)
{
    srand(42);
    int number_of_tests = 10, t = 0;
    while (t < number_of_tests)
    {
        int to = rand() % number_of_tests;
        FunctionalGraph list_of_incidents = FunctionalGraph(to);
        for (int i = 0; i < number_of_tests; i++)
        {
            int vertex = rand() % number_of_tests;
            ASSERT_EQ(vertex == to, list_of_incidents.isConnected(vertex));
        }
        t++;
    }
}

Graph graph;
void buildGraph(std::vector<std::vector<int> > &gr, int sz)
{
    std::vector<std::unique_ptr<ListOfIncidentVerteces> > g(sz);
    for (int i = 0; i < (int)gr.size(); i++)
    {
        Factory f(gr[i], sz);
        g[i] = std::move(f.getList());
    }
    graph = std::move(Graph(g));
}

TEST(ManualTest, SCCTEST1)
{
    std::vector<std::vector<int> > edges(4);
    edges[0].push_back(2);
    edges[1].push_back(3);
    edges[2].push_back(0);
    edges[3].push_back(1);
    buildGraph(edges, 4);
    SCCTarjan go(4);
    std::vector<std::vector<int> > ans = go.scc(graph);
    ASSERT_EQ(ans.size(), 2);
}

#endif
