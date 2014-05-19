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

    def test_floyd(self):
        n = 3
        graph = (((978, 0), (883, 1), (970, 2)), ((869, 0), (57, 1), (93, 2)), ((86, 0), (369, 1), (855, 2)))
        dist = [[10**9] * n for i in range(n)]
        for i in range(n):
            for j in range(n):
                if i == j: dist[i][j] = 0
                else: dist[i][j] = graph[i][j][0]
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k])
        for i in range(n):
            dj = Dijkstra(graph, start=i)
            self.assertEqual(dist[i], dj.getDistsList())
            for j in range(n):
                sm = 0
                for k in range(1, len(dj.getShortestPath(j))):
                    sm += graph[dj.getShortestPath(j)[k-1]][dj.getShortestPath(j)[k]][0]
                self.assertEqual(dist[i][j], sm)

    def test_floyd_stress(self):
        r = Random(2)
        n = 100
        graph = tuple(tuple((r.randint(0, 1000), v) for v in range(n)) for u in range(n))
        dist = [[10**9] * n for i in range(n)]
        for i in range(n):
            for j in range(n):
                if i == j: dist[i][j] = 0
                else: dist[i][j] = graph[i][j][0]
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k])
        for i in range(n):
            dj = Dijkstra(graph, start=i)
            self.assertEqual(dist[i], dj.getDistsList())
            for j in range(n):
                sm = 0
                for k in range(1, len(dj.getShortestPath(j))):
                    sm += graph[dj.getShortestPath(j)[k-1]][dj.getShortestPath(j)[k]][0]
                self.assertEqual(dist[i][j], sm)


class MaxEdgesTest(TestCase):

    def test_manual(self):
        graph = [[(2, 1), (4, 2)], [(3, 2)], [(3, 0)]]
        res = Dijkstra(graph, start=0, path_rec=max, init_id=0)
        dist = res.getDistsList()
        self.assertEqual(dist, [0, 2, 3])
        path = res.getShortestPath(2)
        self.assertEqual(path, [0, 1, 2])

    def test_floyd_stress(self):
        r = Random(2)
        n = 100
        graph = tuple(tuple((r.randint(0, 1000), v) for v in range(n)) for u in range(n))
        dist = [[10**9] * n for i in range(n)]
        for i in range(n):
            for j in range(n):
                if i == j:
                    dist[i][j] = 0
                else:
                    dist[i][j] = graph[i][j][0]
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    dist[j][k] = min(dist[j][k], max(dist[j][i], dist[i][k]))
        for i in range(n):
            dj = Dijkstra(graph, start=i, path_rec=max)
            self.assertEqual(dist[i], dj.getDistsList())
            for j in range(n):
                sm = 0
                for k in range(1, len(dj.getShortestPath(j))):
                    sm = max(sm, graph[dj.getShortestPath(j)[k-1]][dj.getShortestPath(j)[k]][0])
                self.assertEqual(dist[i][j], sm)

from copy import deepcopy

class Ordinal:

    def __init__(self, lst):
        self.lst = list(filter(lambda x: x[1] != 0, (reversed(sorted(lst)))))

    def __ge__(self, other):
        return self.lst >= other.lst

    def __eq__(self, other):
        return self.lst == other.lst

    def __gt__(self, other):
        return self.lst > other.lst

    def __le__(self, other):
        return self.lst <= other.lst

    def __lt__(self, other):
        return self.lst < other.lst

    def __add__(self, other):
        if len(other.lst) == 0: return self
        lst = deepcopy(self.lst)
        while len(lst) > 0 and lst[-1][0] < other.lst[0][0]:
            lst.pop()
        if  len(lst) > 0 and lst[-1][0] == other.lst[0][0]:
            lst[-1] = (lst[-1][0], lst[-1][1] + other.lst[0][1])
        else:
            lst.append(other.lst[0])
        return Ordinal(lst + other.lst[1:])

ord_seed = Random(3)


def rand_ord(max_v):
    return Ordinal([(i, ord_seed.randint(1, max_v)) for i in range(max_v)])


class OrdinalTest(TestCase):

    def test_Comparsion(self):
        a = Ordinal([(1, 2), (0, 1)])  # w * 2 + 1
        b = Ordinal([(1, 1), (0, 1)])  # w + 1
        self.assertGreater(a, b)
        self.assertLess(b, a)
        self.assertEqual(a, a)
        a = Ordinal([(100, 1)])  # w^100
        b = Ordinal([(99, 3000)])  # w^99 * 3000

        self.assertGreater(a, b)
        self.assertLess(b, a)
        a = Ordinal([(2, 1), (1, 1), (0, 1)])  # w^2 + w + 1
        b = Ordinal([(2, 1), (0, 1)])  # w^2 + 1
        self.assertGreater(a, b)
        self.assertLess(b, a)

    def test_Manual(self):
        a = Ordinal([(1, 1), (0, 1)])  # w + 1
        b = Ordinal([(1, 1)])  # w
        c = a + b  # w + 1 + w = w * 2
        d = b + a  # w + w + 1 = w * 2 + 1
        self.assertEqual(c, Ordinal([(1, 2)]))
        self.assertEqual(d, Ordinal([(1, 2), (0, 1)]))
        a = Ordinal([(3, 1), (1, 1), (0, 1)])  # w^3 + w + 1
        b = Ordinal([(2, 1)])  # w^2
        c = a + b  # w^3 + w + 1 + w^2 = w^3 + w^2
        self.assertEqual(c, Ordinal([(3, 1), (2, 1)]))
        a = Ordinal([(0, 0)])  # 0
        b = Ordinal([(3, 1), (2, 2), (1, 3)])  # w^3 + w^2 * 2 + w * 3
        c = a + b
        d = b + a
        self.assertEqual(c, b)
        self.assertEqual(d, b)
        q = [(i, i) for i in reversed(range(1, 99))]
        a = Ordinal(q)  # w^99 * 99 + w^98 * 98 + ... + w
        b = Ordinal([(100, 1)])  # w^100
        c = a + b  # w^99 * 99 + w^98 * 98 + ... + 0 + w^100 = w^100
        self.assertEqual(c, b)


class DijkstraOrdinalTest(TestCase):

    def test_manual(self):
        a = Ordinal([(0, 1)])  # 1
        b = Ordinal([(1, 1)])  # w
        c = Ordinal([(1, 1), (0,1)])  # w + 1
        d = Ordinal([(0, 0)])  # 0
        graph = [[(a, 1), (c, 2)], [(b, 2)], []]
        res = Dijkstra(graph, start=0, init_id=Ordinal([]))
        dist = res.getDistsList()
        self.assertEqual(dist, [d, a, b])
        path = res.getShortestPath(2)
        self.assertEqual(path, [0, 1, 2])

    def test_floyd_stress(self):
        r = Random(2)
        n = 30
        graph = tuple(tuple((rand_ord(100), v) for v in range(n)) for u in range(n))
        dist = [[10**9] * n for i in range(n)]
        for i in range(n):
            for j in range(n):
                if i == j:
                    dist[i][j] = Ordinal([])
                else:
                    dist[i][j] = graph[i][j][0]
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    dist[j][k] = min(dist[j][k], max(dist[j][i], dist[i][k]))
        for i in range(n):
            dj = Dijkstra(graph, start=i, path_rec=max, init_id=Ordinal([]))
            self.assertEqual(dist[i], dj.getDistsList())
            for j in range(n):
                sm = Ordinal([])
                for k in range(1, len(dj.getShortestPath(j))):
                    sm = max(sm, graph[dj.getShortestPath(j)[k-1]][dj.getShortestPath(j)[k]][0])
                self.assertEqual(dist[i][j], sm)

