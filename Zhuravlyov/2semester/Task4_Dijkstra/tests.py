__author__ = 'Алексей'

from unittest import TestCase
from dijksta import Dijkstra
from floyd import Floyd
import random
import copy

class Edge:
    def __init__(self, destination, weight):
        self.destination = destination
        self.weight = weight

class BaseReleaseIntegrationTests(TestCase):
    def test_manual_dijkstra(self):
        graph = [[Edge(1, 2)], [Edge(2, 3)], [], []]
        res = Dijkstra(graph, 0)
        dist = res.get_path_values()
        self.assertEqual(dist, [0, 2, 5, None])
        path = res.get_path_to_vertex(2)
        self.assertEqual(path, [0, 1, 2])
        path = res.get_path_to_vertex(3)
        self.assertEqual(path, None)

    def test_manual_floyd(self):
        matrix = [[0, 2, 100, None], [600, 0, 3, None], [10, 5, 0, None], [None, 3, None, 0]]
        Floyd(matrix, 4)
        self.assertEqual(matrix, [[0, 2, 5, None],[13, 0, 3, None],[10, 5, 0, None],[16, 3, 6, 0]])

    def test_stress(self):
        number_of_tests = 10
        max_graph_size = 50
        max_edge_weight = 100
        for test_case in range(number_of_tests):
            graph_size = random.randint(1, max_graph_size)
            matrix = []
            graph = []
            for i in range(graph_size):
                matrix.append([])
                graph.append([])
                for j in range(graph_size):
                    if random.randint(0, 1) == 0:
                        matrix[i].append(None)
                    else:
                        matrix[i].append(random.randint(0, max_edge_weight))
                        graph[i].append(Edge(j, matrix[i][j]))
            shortest_paths = copy.deepcopy(matrix)
            Floyd(shortest_paths, graph_size)
            for i in range(graph_size):
                shortest_paths[i][i] = 0
            for i in range(graph_size):
                res = Dijkstra(graph, i)
                dists = res.get_path_values()
                for j in range(graph_size):
                    self.assertEqual(dists[j], shortest_paths[i][j])
                    path = res.get_path_to_vertex(j)
                    if path is None:
                        self.assertEqual(shortest_paths[i][j], None)
                    else:
                        actual_weight = sum([matrix[path[i]][path[i+1]] for i in range(len(path) - 1)])
                        self.assertEqual(actual_weight, shortest_paths[i][j])