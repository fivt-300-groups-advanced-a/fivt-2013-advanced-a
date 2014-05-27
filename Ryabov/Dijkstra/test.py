#!/bin/python
from graph.dijkstra import dijkstra
import graph.dijkstra
import unittest
import random
import copy


def generateGraph(n, m, edge):
    a = {}
    for i in n:
        a[i] = set()
    for i in range(m):
        a[random.choice(n)].add((random.choice(n), random.choice(edge)))
    return a


class SampleTest(unittest.TestCase):

    def test_sample(self):
        self.assertEqual(dijkstra({0: [(1, 1)], 1: [(0, 1)]}, 0), {0: 0, 1: 1})
        self.assertEqual(
            dijkstra({'0': [('1', 1)], '1': [('0', 1)]}, '0'), {'0': 0, '1': 1})
        self.assertEqual(dijkstra({0: {(1, 1)}, 1: {(0, 1)}}, 0), {0: 0, 1: 1})
        self.assertEqual(dijkstra({0: [(1, 'a'), (2, 'b')], 1: [(0, 'c')], 2: [
                         (1, 'd')]}, 1, init=''), {0: 'c', 1: '', 2: 'cb'})


class StressTest(unittest.TestCase):

    def test_stress1(self):
        dijkstra(
            generateGraph(list(range(0, 100)), 1000, list(range(1, 100))), 0)

    def test_stress4ExistanceCheck(self):
        for tests in range(10):
            g = generateGraph(list(range(0, 100)), 10000, list(range(1, 100)))
            a = graph.dijkstra.patheddijkstra(g, 0)
            for q, qwe in a.items():
                t = 0
                s = 0
                w = graph.dijkstra.getpath(a, q)
                for i in range(1, len(w)):
                    self.assertTrue((w[i], a[w[i]][0] - a[t][0]) in g[t])
                    t = w[i]

    def test_stress5ExistanceCheck(self):
        for tests in range(10):
            g = generateGraph(list(range(0, 100)), 10000, list(range(-1, -100, -1)))
            a = graph.dijkstra.patheddijkstra(g, 0, cmp=lambda way1, way2: way2 < way1)
            for q, qwe in a.items():
                t = 0
                s = 0
                w = graph.dijkstra.getpath(a, q)
                for i in range(1, len(w)):
                    self.assertTrue((w[i], a[w[i]][0] - a[t][0]) in g[t])
                    t = w[i]

    def test_Floyd_neg(self):
        for tests in range(2):
            n = 100
            g = {}
            for i in range(n):
                g[i] = []
            q = [[1e10]* n for i in range(n)]
            for i in range(n):
                q[i][i] = 0
            for i in range(n):
                for j in range(n):
                    x = random.randrange(1, 100)
                    g[i].append((j, -x))
                    q[i][j] = min(q[i][j], x)
            for k in range(n):
                for i in range(n):
                    for j in range(n):
                        q[i][j] = min(q[i][k] + q[k][j], q[i][j])
            a = dijkstra(g, 0, cmp=lambda way1, way2: way2 < way1)
            for i in range(n):
                self.assertEqual(q[0][i], -a[i])


    def test_Floyd(self):
        for tests in range(5):
            n = 100
            g = {}
            for i in range(n):
                g[i] = []
            q = [[1e10]* n for i in range(n)]
            for i in range(n):
                q[i][i] = 0
            for i in range(n):
                for j in range(n):
                    x = random.randrange(1, 100)
                    g[i].append((j, x))
                    q[i][j] = min(q[i][j], x)
            for k in range(n):
                for i in range(n):
                    for j in range(n):
                        q[i][j] = min(q[i][k] + q[k][j], q[i][j])
            a = dijkstra(g, 0)
            for i in range(n):
                self.assertEqual(q[0][i], a[i])


class HandTest(unittest.TestCase):

    def test_hand1(self):
        g = {}
        n = 100
        for i in range(0, n):
            g[i] = [(i + 1, 1)]
        g[n] = []
        self.assertEqual(
            dijkstra(g, 0), dict(enumerate(list(range(0, n + 1)))))

    def test_hand2(self):
        g = {}
        n = 100
        for i in range(1, n):
            g[i] = [(i + 1, -1), (i - 1, -2)]
        g[n] = []
        g[0] = [(1, -1)]
        self.assertEqual(
            dijkstra(g, 0, cmp=lambda way1, way2: way2 < way1), dict(enumerate(list(range(0, -n - 1, -1)))))

    def test_hand3(self):
        g = {}
        n = 100
        for i in range(1, n):
            g[i] = [(1, i + 1)]
        g[n] = []
        g[0] = [(1, 1)]
        self.assertEqual(dijkstra(g, 0, makelen=(
            lambda way, edge, vertex: way + edge[0]), getvertex=(lambda edge: edge[1])), dict(enumerate(list(range(0, n + 1)))))

    def test_hand4(self):
        g = {}
        n = 100
        for i in range(0, n):
            g[i] = [((i + 1) % n, 1), ((i - 1) % n, 1)]
        self.assertEqual(
            dijkstra(g, 0), dict(enumerate([min(i, n - i) for i in range(n)])))


class Problem(unittest.TestCase):
    def test_hand1(self):
        self.assertEqual(dijkstra({0:[(1, 2, 3)], 1:[]}, 0, makelen = lambda way, edge, vertex: 1e9 if way > edge[1] else edge[2]), {0: 0, 1:3})
    def test_hand2(self):
        self.assertEqual(dijkstra({
            0 : [(1, 0, 2, 3), (2, 0, 1, 1)],
            1 : [(3, 1, 2, 4), (3, 1, 3, 10)],
            2 : [(1, 2, 1, 2)],
            3 : []},
            0, makelen= lambda way, edge, vertex: (1e9, -1) if way[0] > edge[2] else (edge[3], edge[1]),
            init=(0, -1)),
            {0: (0, -1), 1: (2, 2), 2: (1, 0), 3: (4, 1)})
    def test_hand3(self):
        self.assertEqual(graph.dijkstra.tableproblem({
            0 : [(1, 2, 3), (2, 1, 1)],
            1 : [(3, 2, 4), (3, 3, 10)],
            2 : [(1, 1, 2)],
            3 : []}, 0), {0: (0, None), 1: (2, 2), 2: (1, 0), 3: (4, 1)})
    def test_hand4(self):
        self.assertEqual(graph.dijkstra.tableproblem({
            0 : [(1, 2, 3), (2, 1, 2)],
            1 : [(3, 2, 4), (3, 3, 10), (2, 3, 4)],
            2 : [(1, 1, 2), (3, 4, 9), (1, 3, 3)],
            3 : []}, 0), {0: (0, None), 1: (3, 0), 2: (2, 0), 3: (9, 2)})
    def test_hand5(self):
        self.assertEqual(graph.dijkstra.tableproblem({
            0 : [(1, 2, 3), (2, 1, 1)],
            1 : [(3, 2, 4), (3, 3, 10), (2, 3, 4)],
            2 : [(1, 1, 2), (3, 4, 9), (1, 3, 3)],
            3 : []}, 0), {0: (0, None), 1: (2, 2), 2: (1, 0), 3: (4, 1)})

unittest.main()
