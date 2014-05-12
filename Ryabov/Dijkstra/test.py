#!/bin/python
from graph.dijkstra import dijkstra
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


alph = []
for i in range(ord('a'), ord('z') + 1):
    alph.append(chr(i))


def pather(q, w):
    a = copy.copy(q)
    a[0] += w[1]
    a.append(w[0])
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

    def test_stress2(self):
        g = generateGraph(list(range(0, 100)), 1000, alph)
        dijkstra(
            g, 0, makelen=(lambda a, b: a + b[1]), init='', getvertex=lambda a: a[0])

    def test_stress3ExistanceCheck(self):
        g = generateGraph(list(range(0, 100)), 10000, list(range(1, 100)))
        a = dijkstra(g, 0, makelen=pather, init=[0])
        for q, w in a.items():
            t = 0
            s = 0
            for i in range(1, len(w)):
                self.assertTrue((w[i], a[w[i]][0] - a[t][0]) in g[t])
                t = w[i]


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
            dijkstra(g, 0, key=lambda a: -a), dict(enumerate(list(range(0, -n - 1, -1)))))

    def test_hand3(self):
        g = {}
        n = 100
        for i in range(1, n):
            g[i] = [(1, i + 1)]
        g[n] = []
        g[0] = [(1, 1)]
        self.assertEqual(dijkstra(g, 0, makelen=(
            lambda a, b: a + b[0]), getvertex=(lambda a: a[1])), dict(enumerate(list(range(0, n + 1)))))

    def test_hand4(self):
        g = {}
        n = 100
        for i in range(0, n):
            g[i] = [((i + 1) % n, 1), ((i - 1) % n, 1)]
        self.assertEqual(
            dijkstra(g, 0), dict(enumerate([min(i, n - i) for i in range(n)])))


unittest.main()
