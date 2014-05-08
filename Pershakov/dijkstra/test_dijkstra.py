from unittest import TestCase
from dijkstra import Dijkstra
from dijkstra import PQEntry
from queue import PriorityQueue
import random
import string

__author__ = 'pershik'


class Edge:
    def __init__(self, to, weight):
        self.to = to
        self.weight = weight


class StringEdge:
    def __init__(self, destination, label):
        self.destination = destination
        self.label = label


class IntegrationTest(TestCase):
    def test_manual_bamboo(self):
        graph = [[Edge(1, 1)], [Edge(2, 2)], [Edge(3, 3)], []]
        solver = Dijkstra()
        res = solver.get_shortest_paths(graph, from_vertex=0, init_path_value=0)
        self.assertEqual(res, [0, 1, 3, 6])
        res = solver.get_shortest_paths(graph, from_vertex=1, init_path_value=0)
        self.assertEqual(res, [None, 0, 2, 5])
        res = solver.get_shortest_paths(graph, from_vertex=2, init_path_value=0)
        self.assertEqual(res, [None, None, 0, 3])
        res = solver.get_shortest_paths(graph, from_vertex=3, init_path_value=0)
        self.assertEqual(res, [None, None, None, 0])

    def test_manual_bidirectional_cycle(self):
        graph = [[Edge(1, 1), Edge(4, 2)], [Edge(2, 3), Edge(0, 3)], [Edge(3, 2), Edge(1, 5)],
                 [Edge(4, 4), Edge(2, 6)], [Edge(0, 2), Edge(3, 4)]]
        solver = Dijkstra()
        res = solver.get_shortest_paths(graph, from_vertex=0, init_path_value=0)
        self.assertEqual(res, [0, 1, 4, 6, 2])
        res = solver.get_shortest_paths(graph, from_vertex=1, init_path_value=0)
        self.assertEqual(res, [3, 0, 3, 5, 5])
        res = solver.get_shortest_paths(graph, from_vertex=2, init_path_value=0)
        self.assertEqual(res, [8, 5, 0, 2, 6])
        res = solver.get_shortest_paths(graph, from_vertex=3, init_path_value=0)
        self.assertEqual(res, [6, 7, 6, 0, 4])
        res = solver.get_shortest_paths(graph, from_vertex=4, init_path_value=0)
        self.assertEqual(res, [2, 3, 6, 4, 0])


class TestRelaxEdge(TestCase):
    def test_default_realisations(self):
        solver = Dijkstra()
        solver.dist = [None, None]
        edge = Edge(1, 0)

        #None case
        solver.dist[0] = 0
        edge.weight = 5
        solver.relax_edge(from_vertex=0, edge=edge)
        self.assertEqual(solver.dist[1], 5, 'None case. Incorrect weight')
        self.assertEqual(solver.pr_queue.qsize(), 1, 'None case. Incorrect pr_queue size')
        top = solver.pr_queue.get()
        if top.prior != solver.dist[1] or top.value != 1:
            self.fail('None case. Incorrect pr_queue')

        #Not None case
        for solver.dist[0] in range(0, 50):
            for i in range(0, 50):
                for edge.weight in range(0, 50):
                    solver.pr_queue = PriorityQueue()
                    solver.dist[1] = i
                    if solver.dist[0] + edge.weight < solver.dist[1]:
                        flag = True
                    else:
                        flag = False
                    solver.relax_edge(from_vertex=0, edge=edge)
                    if solver.dist[0] + edge.weight < solver.dist[1]:
                        self.fail('Incorrect weight')
                    if flag:
                        self.assertEqual(solver.pr_queue.qsize(), 1, 'Incorrect pr_queue size')
                        top = solver.pr_queue.get()
                        if top.prior != solver.dist[1] or top.value != 1:
                            self.fail('Incorrect pr_queue')
                    else:
                        self.assertEqual(solver.pr_queue.qsize(), 0, 'Incorrect pr_queue size')

    def random_string(self):
        return ''.join(random.choice(string.ascii_letters) for i in range(1, 10))

    def test_not_default_realisation(self):
        """
            Edge { destination, label }
            Cost of path - maximal reversed label on it

        """
        solver = Dijkstra()
        solver.dist = [None, None]
        string_edge = StringEdge(1, '')
        cnt_tests = 100
        for test_number in range(0, cnt_tests):
            solver.dist[0] = self.random_string()
            solver.dist[1] = old_value = self.random_string()
            string_edge.label = self.random_string()
            solver.relax_edge(from_vertex=0, edge=string_edge,
                              cmp=lambda weight1, weight2: weight1 > weight2,
                              add_edge=lambda weight, edge: max(weight, edge.label[::-1]),
                              get_destination=lambda edge: edge.destination)
            self.assertEqual(
                max(solver.dist[1], max(solver.dist[1], string_edge.label[::-1])), solver.dist[1])
            if old_value != solver.dist[1]:
                self.assertEqual(solver.pr_queue.qsize(), 1)
                top = solver.pr_queue.get()
                self.assertTrue(top.prior == solver.dist[1] and top.value == 1)
            else:
                self.assertEqual(solver.pr_queue.qsize(), 0)


class TestGetMinVertex(TestCase):
    def test_default_realisation(self):
        solver = Dijkstra()
        size = 3
        cnt_tests = 10
        max_val = 10
        for test_number in range(0, cnt_tests):
            for d1 in range(0, max_val):
                for d2 in range(0, max_val):
                    for d3 in range(0, max_val):
                        solver.dist = [d1, d2, d3]
                        solver.pr_queue = PriorityQueue()
                        for i in range(0, size):
                            solver.pr_queue.put(PQEntry(solver.dist[i], i))
                            solver.pr_queue.put(PQEntry(solver.dist[i] + 2, i))
                        for i in range(0, size):
                            min_vertex = solver.get_min_vertex()
                            pos_min = 0
                            for j in range(0, size):
                                if solver.dist[j] < solver.dist[pos_min]:
                                    pos_min = j
                            self.assertNotEqual(min_vertex, -1)
                            self.assertEqual(solver.dist[min_vertex], solver.dist[pos_min])
                            solver.dist[min_vertex] = max_val + 100
                        min_vertex = solver.get_min_vertex()
                        self.assertEqual(min_vertex, -1)
