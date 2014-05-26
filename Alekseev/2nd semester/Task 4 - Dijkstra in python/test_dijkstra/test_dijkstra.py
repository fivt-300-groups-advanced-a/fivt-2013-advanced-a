from copy import copy
import unittest

from dijkstra.dijkstra import dijkstra
#from test_dijkstra.utils import Graph
from utils import Graph


class TestDijkstraSmall(unittest.TestCase):
    
    small_tests = [
        Graph([(0, 1, 1), (1, 2, 3), (0, 2, 2)]),
        Graph([(0, 0, 1)]),
        Graph([(3, 2, 0), (2, -5, 3)]),
        Graph([('aba', 2, 1), (2, 5, 5), (5, 8, 8), (8, 'aba', 0)]),
        Graph.random(0, 10, 20),
        Graph.random(1, 10, 20),
        Graph.random(2, 10, 20),
        Graph.random(3, 10, 20),
        Graph.random(0, 20, 25),
        Graph.random(1, 20, 25),
        Graph.random(2, 20, 25),
        Graph.random(3, 20, 25),
    ]

    @staticmethod
    def edge_to(e):
        return e.u

    @staticmethod
    def edge_w(e):
        return e.weight

    def test_dijkstra_correctness_small(self):
        for graph in self.small_tests:
            for v in graph.vertices:
                self.assertDictEqual(dijkstra(graph.adj, v, 0, edge_w=self.edge_w),
                                     graph.paths(v, 0),
                                     repr(graph))

#    @unittest.skip('stress is so slow...')
    def test_dijkstra_correctness_stress(self):
        for i in range(300):
            graph = Graph.random(i, 80, 400)
            self.assertDictEqual(dijkstra(graph.adj, graph.vertices[0], 0, edge_w=self.edge_w),
                                 graph.paths(graph.vertices[0], 0),
                                 repr(graph))

    def test_dijkstra_adj_lists(self):
        for graph in self.small_tests:
            lists = {v: list(edges.values()) for v, edges in graph.adj.items()}
            for v in graph.vertices:
                self.assertDictEqual(dijkstra(lists, v, 0, edge_to=self.edge_to, edge_w=self.edge_w),
                                     graph.paths(v, 0),
                                     repr(graph))

    def test_dijkstra_adjacency_matrix(self):
        for graph in self.small_tests:
            ok = True
            for v in graph.vertices:
                if not isinstance(v, int) or v < 0:
                    ok = False
            if not ok:
                continue

            n = max(graph.vertices) + 1
            matrix = [[None] * n for _ in range(n)]
            for e in graph.edge_list:
                matrix[e.v][e.u] = e.weight

            for v in graph.vertices:
                self.assertDictEqual(dijkstra(matrix, v, 0),
                                     graph.paths(v, 0),
                                     repr(graph))

    def test_dijkstra_strange_fields(self):
        edge_to = lambda e: e[0]
        edge_w = lambda e: e[1]

        for graph in self.small_tests:
            adj = copy(graph.adj)
            for k in adj.keys():
                adj[k] = [(b, c) for a, b, c in adj[k].values()]
            for v in graph.vertices:
                self.assertDictEqual(dijkstra(adj, v, 0, edge_to=edge_to, edge_w=edge_w),
                                     graph.paths(v, 0),
                                     repr(graph))

    def test_dijkstra_strange_paths(self):
        start_path = ([], 0)
        sum_pe = lambda p, e: (p[0] + [e.u], p[1] + e.weight)
        path_lt = lambda a, b: (a[1], a[0]) < (b[1], b[0])

        for graph in self.small_tests:
            for v in graph.vertices:
                dij = dijkstra(graph.adj, v, start_path, sum_pe=sum_pe, path_lt=path_lt)
                fb = graph.paths(v, start_path, sum_pe=sum_pe, path_lt=path_lt)
                self.assertDictEqual(dij, fb, repr(graph))


class TestDijkstraDeath(unittest.TestCase):
    def test_dijkstra_negative_loop_death(self):
        self.assertRaises(AssertionError, dijkstra, [{1: -1}, {0: -1}], 0, 0)

    def test_dijkstra_unknown_argument_death(self):
        self.assertRaises(TypeError, dijkstra, [], 0, 0, path_cmp=lambda a, b: a > b)


class TestDijkstraSimplicity(unittest.TestCase):
    def test_dijkstra_matrix_simplicity(self):
        dijkstra([[0, 1, 2],
                  [3, 4, 5],
                  [6, 7, 8]], 0, 0)

# class TestDijkstraInteresting(unittest.TestCase):
#     def

if __name__ == '__main__':
    unittest.main()
