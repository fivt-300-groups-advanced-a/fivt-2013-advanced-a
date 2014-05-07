from unittest import TestCase
from dijkstra import Dijkstra
from queue import PriorityQueue

__author__ = 'pershik'


class Edge:
    def __init__(self, to, weight):
        self.to = to
        self.weight = weight


class IntegrationTest(TestCase):
    def test_manual_bamboo(self):
        graph = [[Edge(1, 1)], [Edge(2, 2)], [Edge(3, 3)], []]
        solver = Dijkstra()
        res = solver.get_shortest_paths(graph, from_vertex=0, init_path_value=0)
        if res != [0, 1, 3, 6]:
            self.fail()
        res = solver.get_shortest_paths(graph, from_vertex=1, init_path_value=0)
        if res != [None, 0, 2, 5]:
            self.fail()
        res = solver.get_shortest_paths(graph, from_vertex=2, init_path_value=0)
        if res != [None, None, 0, 3]:
            self.fail()
        res = solver.get_shortest_paths(graph, from_vertex=3, init_path_value=0)
        if res != [None, None, None, 0]:
            self.fail()

    def test_manual_bidirectional_cycle(self):
        graph = [[Edge(1, 1), Edge(4, 2)], [Edge(2, 3), Edge(0, 3)], [Edge(3, 2), Edge(1, 5)],
                 [Edge(4, 4), Edge(2, 6)], [Edge(0, 2), Edge(3, 4)]]
        solver = Dijkstra()
        res = solver.get_shortest_paths(graph, from_vertex=0, init_path_value=0)
        if res != [0, 1, 4, 6, 2]:
            self.fail()


class TestRelaxEdge(TestCase):
    def test_default_realisations(self):
        solver = Dijkstra()
        solver.dist = [None, None]
        edge = Edge(1, 0)

        #None case
        solver.dist[0] = 0
        edge.weight = 5
        solver.relax_edge(from_vertex=0, edge=edge)
        if solver.dist[1] != 5:
            self.fail('None case. Incorrect weight')
        if solver.pr_queue.qsize() != 1:
            self.fail('None case. Incorrect pr_queue size')
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
                        if solver.pr_queue.qsize() != 1:
                            self.fail('Incorrect pr_queue size')
                        top = solver.pr_queue.get()
                        if top.prior != solver.dist[1] or top.value != 1:
                            self.fail('Incorrect pr_queue')
                    else:
                        if solver.pr_queue.qsize() != 0:
                            self.fail('Incorrect pr_queue size')