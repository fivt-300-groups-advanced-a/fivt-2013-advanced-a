from copy import copy
import unittest

from dijkstra.dijkstra import Dijkstra
#from test_dijkstra.utils import Graph
from utils import Graph

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


class TestDijkstraImplCorrectness(unittest.TestCase):

    def test_impl_correctness_small(self):
        for graph in small_tests:
            for v in graph.vertices:
                self.assertDictEqual(Dijkstra._impl(graph.get_wrapper(), v, 0, Graph.cmp_path,
                                                    Graph.sum_pe, False)[0],
                                     graph.paths(v),
                                     repr(graph))

    def test_impl_correctness_stress(self):
        for i in range(300):
            graph = Graph.random(i, 80, 400)
            self.assertDictEqual(Dijkstra._impl(graph.get_wrapper(), graph.vertices[0], 0,
                                                Graph.cmp_path, Graph.sum_pe, False)[0],
                                 graph.paths(graph.vertices[0]),
                                 repr(graph))


class TestDijkstraIntegrationSmall(unittest.TestCase):
    def test_dijkstra_adj_lists(self):
        for graph in small_tests:
            lists = {v: list(edges.values()) for v, edges in graph.adj.items()}
            v = graph.vertices[0]
            self.assertDictEqual(Dijkstra(lists, v).paths,
                                 graph.paths(v),
                                 repr(graph))

    def test_dijkstra_adjacency_matrix(self):
        for graph in small_tests:
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

            v = graph.vertices[0]
            self.assertDictEqual(Dijkstra(matrix, v, edge_to=None).paths,
                                 graph.paths(v),
                                 repr(graph))

    def test_dijkstra_strange_fields(self):
        for graph in small_tests:
            for v in graph.vertices:
                self.assertDictEqual(Dijkstra(graph.adj, v, edge_to=Graph.Edge.edge_to, edge_w=Graph.Edge.edge_w).paths,
                                     graph.paths(v),
                                     repr(graph))

    def test_dijkstra_strange_paths(self):
        start_path = ([], 0)
        sum_pev = lambda p, e, v: (p[0] + [v], p[1] + e.weight)
        path_lt = lambda a, b: (a[1], a[0]) < (b[1], b[0])

        for graph in small_tests:
            for v in graph.vertices:
                dij = Dijkstra(graph.adj, v, start_p=start_path, sum_pev=sum_pev, cmp_path_lt=path_lt).paths
                fb = graph.paths(v, start_p=start_path, sum_pev=sum_pev, path_lt=path_lt)
                self.assertDictEqual(dij, fb, repr(graph))


if __name__ == '__main__':
    unittest.main()
