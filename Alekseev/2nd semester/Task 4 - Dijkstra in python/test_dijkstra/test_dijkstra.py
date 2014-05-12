from copy import copy
import unittest

from dijkstra.dijkstra import dijkstra
import utils


__author__ = 'moskupols'


class TestDijkstra(unittest.TestCase):

    small_tests = [
        utils.Graph([(0, 1, 1), (1, 2, 3), (0, 2, 2)]),
        utils.Graph([(0, 0, 1)]),
        utils.Graph([(3, 2, 0), (2, -5, 3)]),
        utils.Graph([('aba', 2, 1), (2, 5, 5), (5, 8, 8), (8, 'aba', 0)]),
        utils.Graph.random(0, 10, 20),
        utils.Graph.random(1, 10, 20),
        utils.Graph.random(2, 10, 20),
        utils.Graph.random(3, 10, 20),
        utils.Graph.random(0, 20, 25),
        utils.Graph.random(1, 20, 25),
        utils.Graph.random(2, 20, 25),
        utils.Graph.random(3, 20, 25),
    ]

    def test_dijkstra_correctness_small(self):
        for graph in self.small_tests:
            for v in graph.vertices:
                self.assertEqual(dijkstra(graph.adj, v, 0),
                                 utils.ford_bellman(graph, v, 0),
                                 repr(graph))

#    @unittest.skip('stress is so slow...')
    def test_dijkstra_correctness_stress(self):
        for i in range(100):
            graph = utils.Graph.random(i, 80, 400)
            self.assertEqual(dijkstra(graph.adj, graph.vertices[0], 0),
                             utils.ford_bellman(graph, graph.vertices[0], 0),
                             repr(graph))

    def test_dijkstra_adj_lists(self):
        for graph in self.small_tests:
            lists = {source: list(edges.values()) for source, edges in graph.adj.items()}
            for v in graph.vertices:
                self.assertEqual(dijkstra(lists, v, 0),
                                 utils.ford_bellman(graph, v, 0),
                                 repr(graph))

    def test_dijkstra_strange_fields(self):
        edge_to = lambda e: e[0]
        edge_w = lambda e: e[1]

        for graph in self.small_tests:
            adj = copy(graph.adj)
            for k in adj.keys():
                adj[k] = [(b, c) for a, b, c in adj[k].values()]
            for v in graph.vertices:
                self.assertEqual(dijkstra(adj, v, 0, edge_to=edge_to, edge_w=edge_w),
                                 utils.ford_bellman(graph, v, 0),
                                 repr(graph))

    def test_dijkstra_strange_paths(self):
        start_path = ([], 0)
        sum_pe = lambda p, e: (p[0] + [e.to], p[1] + e.weight)
        cmp_path = lambda a, b: (a[1], a[0]) < (b[1], b[0])

        for graph in self.small_tests:
            for v in graph.vertices:
                dij = dijkstra(graph.adj, v, start_path, sum_pe=sum_pe, cmp_path=cmp_path)
                fb = utils.ford_bellman(graph, v, start_path, sum_pe=sum_pe, cmp_path=cmp_path)
                self.assertEqual(dij, fb, repr(graph))


if __name__ == '__main__':
    unittest.main()
