#!/bin/env python3
from dijkstra import Dijkstra
from unittest import TestCase, main
from random import Random

class IntegrationTest(TestCase):
    def test_cycle(self):
        graph = [[(1, 1), (1, 3)], [(1, 2), (1, 0)], [(1, 3), (1, 1)], [(1, 0), (1, 2)]]
        a = Dijkstra(graph, start=0)
        self.assertEqual(a.getShortestPath(3), [0, 3])
        self.assertEqual(a.getDistance(2), 2)
    def test_bamboo(self):
        graph = [[(1, 1)], [(4, 2)], [(2, 3)], []]
        ans = Dijkstra(graph)
        self.assertEqual(ans.getDistsList(), [0, 1, 5, 7])
    def test_str_edges(self):
        graph = [[("a", 1), ("b", 2)], [("c", 3)], [("c", 3)], []]
        self.assertEqual("ac", Dijkstra(graph, start=0, init_id="").getDistance(3))
    def floyd_stress(self):
        r = Random(2)
        n = 100
        graph = [[(r.randint(0, 1000), v) for v in range(n)] for u in range(n)]
        dist = [[10**9] * n] * n
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k])
        for i in range(n):
            self.assertEqual(dist[i], Dijkstra(graph, start=i).getDistsList())


    def floyd_str_stress(self):
        r = Random(2)
        n = 100
        graph = [[(r.choice('qwertyuiopasdfghjklzxcvbnm'), v) for v in range(n)] for u in range(n)]
        dist = [[10**9] * n] * n
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k])
        for i in range(n):
            self.assertEqual(dist[i], Dijkstra(graph, start=i).getDistsList())
