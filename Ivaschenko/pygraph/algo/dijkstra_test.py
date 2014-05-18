__author__ = 'skird'

from algo.dijkstra import Dijkstra

import itertools
import unittest
import random


class DijkstraIntegrationTest(unittest.TestCase):
    def setUp(self):
        pass

    def __make_weighted_list_graph(self, n, edges) -> list:
        graph = [list() for _ in range(n)]
        for edge in edges:
            graph[edge[0]].append((edge[1], edge[2]))
        return graph

    def __make_weighted_dict_graph(self, n, edges) -> dict:
        graph = dict()
        for i in range(n):
            graph[i] = list()
        for edge in edges:
            graph[edge[0]].append((edge[1], edge[2]))
        return graph

    def __make_dict_graph(self, n, edges) -> dict:
        graph = dict()
        for i in range(n):
            graph[i] = list()
        for edge in edges:
            graph[edge[0]].append(edge[1])
        return graph

    def __make_list_graph(self, n, edges) -> list:
        graph = [list() for _ in range(n)]
        for edge in edges:
            graph[edge[0]].append(edge[1])
        return graph
    
    def test_with_integer_weights(self) -> None:
        tests = list()
        answers = list()

        tests.append(self.__make_weighted_list_graph(3, {(0, 1, 2), (0, 2, 5), (1, 2, 2)}))
        answers.append({0: 0, 1: 2, 2: 4})

        tests.append(self.__make_weighted_dict_graph(5, {(0, 1, 1), (1, 2, 1), (2, 3, 1), (3, 4, 1), (0, 2, 3),
                                                         (2, 4, 3), (1, 4, 4)}))
        answers.append({0: 0, 1: 1, 2: 2, 3: 3, 4: 4})

        tests.append(self.__make_weighted_list_graph(10, {}))
        answers.append(dict({0: 0}))

        for (index, test) in enumerate(tests):
            ret = Dijkstra.find_shortest_paths(test, [0])
            self.assertDictEqual(ret, answers[index])

    def test_with_no_weights(self) -> None:
        tests = list()
        answers = list()

        tests.append((self.__make_list_graph(3, {(0, 1), (0, 2), (1, 2)}), [0]))
        answers.append({0: 0, 1: 1, 2: 1})

        tests.append((self.__make_dict_graph(5, {(0, 1), (1, 2), (1, 3), (3, 1), (3, 0), (3, 4), (4, 2)}), [0]))
        answers.append({0: 0, 1: 1, 2: 2, 3: 2, 4: 3})

        tests.append((self.__make_dict_graph(5, {(0, 1), (1, 2), (1, 3), (3, 1), (3, 0), (3, 4), (4, 2)}), [0, 4]))
        answers.append({0: 0, 1: 1, 2: 1, 3: 2, 4: 0})

        tests.append((self.__make_list_graph(3, {}), [1, 2]))
        answers.append(dict({1: 0, 2: 0}))

        for (index, test) in enumerate(tests):
            ret = Dijkstra.find_shortest_paths(test[0], test[1],
                                               append_edge=lambda path, edge: path + 1,
                                               edge_destination=lambda edge: edge)
            self.assertDictEqual(ret, answers[index])


class DijkstraStressTest(unittest.TestCase):
    def setUp(self):
        pass

    def __make_graph_from_bit_field(self, size, bit_field) -> list:
        graph = [list() for _ in range(size)]
        for i in range(size):
            for j in range(size):
                if bit_field[i * size + j] == 1:
                    graph[i].append(j)
        return graph

    def __make_random_weighted_graph_from_bit_field(self, size, bit_field) -> list:
        graph = [list() for _ in range(size)]
        for i in range(size):
            for j in range(size):
                if bit_field[i * size + j] == 1:
                    graph[i].append((j, random.randrange(100)))
        return graph

    def __generate_random_weighted_graph(self, size) -> list:
        max_weight = 100
        p = random.random()
        graph = [list() for _ in range(size)]
        for i in range(size):
            for j in range(size):
                if random.random() < p:
                    graph[i].append((j, random.randrange(max_weight)))
        return graph

    def __dummy_shortest_paths_unweighted(self, graph, from_vertex) -> dict:
        n = len(graph)
        dist = [[n for _ in range(n)] for _ in range(n)]

        for i in range(n):
            for j in graph[i]:
                dist[i][j] = 1
            dist[i][i] = 0

        for k in range(n):
            for i in range(n):
                for j in range(n):
                    if dist[i][j] > dist[i][k] + dist[k][j]:
                        dist[i][j] = dist[i][k] + dist[k][j]

        answer = dict()
        for i in range(n):
            if dist[from_vertex][i] < n:
                answer[i] = dist[from_vertex][i]
        return answer

    def __dummy_shortest_paths_weighted(self, graph, from_vertex) -> dict:
        n = len(graph)
        dist = [[None for _ in range(n)] for _ in range(n)]
        for i in range(n):
            for edge in graph[i]:
                dist[i][edge[0]] = edge[1]
            dist[i][i] = 0

        for k in range(n):
            for i in range(n):
                for j in range(n):
                    if dist[i][k] is not None and dist[k][j] is not None and \
                       (dist[i][j] is None or dist[i][j] > dist[i][k] + dist[k][j]):
                        dist[i][j] = dist[i][k] + dist[k][j]

        answer = dict()
        for i in range(n):
            if dist[from_vertex][i] is not None:
                answer[i] = dist[from_vertex][i]
        return answer

    def test_on_all_small_graphs(self) -> None:
        graph_size = 3
        for bit_field in itertools.product({0, 1}, repeat=graph_size ** 2):
            test = self.__make_graph_from_bit_field(graph_size, bit_field)
            ret = Dijkstra.find_shortest_paths(test, [0], append_edge=lambda path, edge: path + 1,
                                               edge_destination=lambda edge: edge)
            self.assertDictEqual(ret, self.__dummy_shortest_paths_unweighted(test, 0))

        for bit_field in itertools.product({0, 1}, repeat=graph_size ** 2):
            test = self.__make_random_weighted_graph_from_bit_field(graph_size, bit_field)
            ret = Dijkstra.find_shortest_paths(test, [0])
            self.assertDictEqual(ret, self.__dummy_shortest_paths_weighted(test, 0))

    def test_on_random_graphs(self) -> None:
        test_number = 30
        graph_size = 50
        for it in range(test_number):
            size = random.randrange(1, graph_size + 1)
            test = self.__generate_random_weighted_graph(size)
            ret = Dijkstra.find_shortest_paths(test, [0], append_edge=lambda path, edge: path + edge[1],
                                               edge_destination=lambda edge: edge[0])
            self.assertDictEqual(ret, self.__dummy_shortest_paths_weighted(test, 0))


# access to private methods of Dijkstra:
# noinspection PyProtectedMember
class DijkstraRelaxEdgeTest(unittest.TestCase):
    def setUp(self) -> None:
        pass

    def test_on_simple_graph(self) -> None:
        gray = list()
        dist = {0: 0}
        compare = lambda a, b: a < b
        destination = lambda edge: edge
        append_edge = lambda path, edge: path + 1

        Dijkstra._Dijkstra__relax_edge(0, 1, gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [1])
        self.assertDictEqual(dist, {0: 0, 1: 1})

        Dijkstra._Dijkstra__relax_edge(1, 0, gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [1])
        self.assertDictEqual(dist, {0: 0, 1: 1})

        Dijkstra._Dijkstra__relax_edge(1, 2, gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [1, 2])
        self.assertDictEqual(dist, {0: 0, 1: 1, 2: 2})

        Dijkstra._Dijkstra__relax_edge(0, 2, gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [1, 2])
        self.assertDictEqual(dist, {0: 0, 1: 1, 2: 1})

    def test_on_timetable_graph(self) -> None:

        class Distance:
            def __init__(self, value):
                self.dist = value

            def __lt__(self, other):
                assert isinstance(other, Distance)
                return (self.dist is not None) and (other.dist is None or self.dist < other.dist)

            def __repr__(self):
                return str(self.dist) if self.dist is not None else "Infinity"

            def __eq__(self, other):
                return self.dist == other.dist

        gray = list()
        dist = {0: Distance(0)}
        compare = lambda a, b: a < b
        destination = lambda edge: edge[0]

        def append_edge(path, edge):
            assert isinstance(path, Distance)
            if path.dist is None or path.dist > edge[1]:
                return Distance(None)
            return Distance(edge[2])

        Dijkstra._Dijkstra__relax_edge(0, (1, 0, 1), gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [1])
        self.assertDictEqual(dist, {0: Distance(0), 1: Distance(1)})

        Dijkstra._Dijkstra__relax_edge(0, (2, -1, 1), gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [1, 2])
        self.assertDictEqual(dist, {0: Distance(0), 1: Distance(1), 2: Distance(None)})

        Dijkstra._Dijkstra__relax_edge(1, (2, 3, 4), gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [1, 2])
        self.assertDictEqual(dist, {0: Distance(0), 1: Distance(1), 2: Distance(4)})

        Dijkstra._Dijkstra__relax_edge(0, (2, 2, 2), gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [1, 2])
        self.assertDictEqual(dist, {0: Distance(0), 1: Distance(1), 2: Distance(2)})

    def test_on_max_edge_graph(self) -> None:
        gray = [0, 1, 2, 3]
        dist = {0: -5, 1: 100, 2: 100, 3: 1}

        compare = lambda x, y: x < y
        destination = lambda edge: edge[0]
        append_edge = lambda path, edge: max(path, edge[1])

        Dijkstra._Dijkstra__relax_edge(0, (1, 5), gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [0, 1, 2, 3])
        self.assertDictEqual(dist, {0: -5, 1: 5, 2: 100, 3: 1})

        Dijkstra._Dijkstra__relax_edge(1, (2, 101), gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [0, 1, 2, 3])
        self.assertDictEqual(dist, {0: -5, 1: 5, 2: 100, 3: 1})

        Dijkstra._Dijkstra__relax_edge(1, (2, 3), gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [0, 1, 2, 3])
        self.assertDictEqual(dist, {0: -5, 1: 5, 2: 5, 3: 1})

        Dijkstra._Dijkstra__relax_edge(0, (2, 3), gray, dist, compare, destination, append_edge)
        self.assertListEqual(gray, [0, 1, 2, 3])
        self.assertDictEqual(dist, {0: -5, 1: 5, 2: 3, 3: 1})


# access to private methods of Dijkstra:
# noinspection PyProtectedMember
class DijkstraFindNearestTest(unittest.TestCase):
    def setUp(self) -> None:
        pass

    def test_on_simple_graph(self) -> None:
        gray = [0, 2, 7, 3]
        dist = {0: 34, 1: 3, 2: 49, 3: -1, 4: 0,  7: 12, 100: 35}
        self.assertEqual(Dijkstra._Dijkstra__find_nearest(gray, dist, lambda x, y: x < y), 3)
        self.assertEqual(Dijkstra._Dijkstra__find_nearest(gray, dist, lambda x, y: x > y), 2)

    def test_with_custom_labels(self) -> None:
        gray = ["Andrey", "Leo", "Vasily", "Vladimir"]
        dist = {"Andrey": "vampire", "Leo": "elf", "Vasily": "gray unicorn", "Vladimir": "orc", "Masha": "nobody"}
        self.assertEqual(Dijkstra._Dijkstra__find_nearest(gray, dist, lambda x, y: x < y), "Leo")
        self.assertEqual(Dijkstra._Dijkstra__find_nearest(gray, dist, lambda x, y: x > y), "Andrey")
        self.assertEqual(Dijkstra._Dijkstra__find_nearest(gray, dist, lambda x, y: len(x) > len(y)), "Vasily")
