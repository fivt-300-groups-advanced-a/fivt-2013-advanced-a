__author__ = 'Alex'

from Dijkstra import Dijkstra
from Dijkstra import Node
from unittest import TestCase
from queue import PriorityQueue
import random


class Edge:
    def __init__(self, weight, vertex):
        self.weight = weight
        self.vertex = vertex


class Path:
    def __init__(self, path, weight):
        self.path = path
        self.weight = weight

    def add_edge(self, edge):
        ans = self.path[:]
        ans.append(edge.vertex)
        return Path(ans, self.weight + edge.weight)


#TODO write test for bamboo, cycle, stress test with all graphs on N vertexes, path and not default lambda functions
class IntegrationTest(TestCase):
    def test_cycle(self):
        graph = [[Node(1, 1), Node(3, 2)], [Node(1, 2)], [Node(0, 1)]]
        ans = Dijkstra().get_shortest_path(graph, 3, 0, 0)
        self.assertEqual(ans, [0, 1, 2])

    def test_bamboo(self):
        graph = [[Node(1, 1), Node(4, 2)], [Node(2, 2)], []]
        ans = Dijkstra().get_shortest_path(graph, 3, 0, 0)
        self.assertEqual(ans, [0, 1, 3])

    def test_unusual_edges(self):
        graph = [[Edge("a", 1), Edge("c", 2)], [Edge("b", 2)], []]
        ans = Dijkstra()
        list = ans.get_shortest_path(graph, 3, 0, "")
        self.assertEqual(list, ["", "a", "ab"])

    def find_paths_between_all_vertexes(self, graph, size):
        for k in range(size):
            for i in range(size):
                for j in range(size):
                    if not graph[i][k] is None and not graph[k][j] is None:
                        if graph[i][j] is None:
                            graph[i][j] = graph[i][k] + graph[k][j]
                        else:
                            graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j])
        return graph

    def test_stress(self):
        for test_number in range(1000):
            graph_size = 6
            graph = []
            matrix = []
            for i in range(graph_size):
                matrix.append([None] * graph_size)
                graph.append([])
            for i in range(graph_size):
                matrix[i][i] = 0
                for j in range(i + 1, graph_size):
                    edge_weight = random.randint(0, 100)
                    matrix[i][j] = edge_weight
                    matrix[j][i] = edge_weight
                    graph[i].append(Edge(edge_weight, j))
                    graph[j].append(Edge(edge_weight, i))
            self.find_paths_between_all_vertexes(matrix, graph_size)
            for i in range(graph_size):
                ans = Dijkstra().get_shortest_path(graph, graph_size, i, 0)
                self.assertEqual(ans, matrix[i], graph)

    def test_unusual_lambda_functions_and_path(self):
        for test_number in range(1000):
            graph_size = 6
            graph = []
            matrix = []
            for i in range(graph_size):
                matrix.append([None] * graph_size)
                graph.append([])
            for i in range(graph_size):
                matrix[i][i] = 0
                for j in range(i + 1, graph_size):
                    edge_weight = random.randint(0, 100)
                    matrix[i][j] = edge_weight
                    matrix[j][i] = edge_weight
                    graph[i].append(Edge(edge_weight, j))
                    graph[j].append(Edge(edge_weight, i))
            self.find_paths_between_all_vertexes(matrix, graph_size)
            for i in range(graph_size):
                ans = Dijkstra().get_shortest_path(graph, graph_size, i, Path([i], 0),
                                                   compare_weight=lambda path_a, path_b: path_a.weight < path_b.weight,
                                                   adder_to_path=Path.add_edge,
                                                   destination_of_edge=lambda edge: edge.vertex)
                for k in range(graph_size):
                    weight_of_path = 0
                    path_size = len(ans[k].path)
                    self.assertEqual(ans[k].path[0], i)
                    self.assertEqual(ans[k].path[path_size - 1], k)
                    for j in range(path_size - 1):
                        weight_of_path += matrix[ans[k].path[j]][ans[k].path[j + 1]]
                    self.assertEqual(ans[k].weight, weight_of_path)


#TODO test adding to the queue(that i get what i want) and relaxing(comparing with None and etc.)
class UnitTest(TestCase):
    def test_relaxing_of_edges(self):
        self.queue = PriorityQueue()
        ans = Dijkstra()
        ans.distances = [None] * 3
        ans.distances[0] = 0
        ans.distances[1] = 2
        graph = [[Node(1, 1), Node(1, 2)], [], []]
        ans.upgrade_path_through_vertex(graph, 0)
        self.assertEqual(ans.distances, [0, 1, 1])

    def test_queue(self):
        ans = Dijkstra()
        list_of_random_numbers = [None] * 1000
        for i in range(1000):
            list_of_random_numbers[i] = random.randint(0, 1000)
            ans.queue.put(Node(list_of_random_numbers[i], i))
        list_of_random_numbers.sort()
        for i in range(1000):
            edge = ans.queue.get()
            self.assertEqual(edge.weight, list_of_random_numbers[i])