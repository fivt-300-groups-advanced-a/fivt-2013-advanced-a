__author__ = 'Алексей'

from unittest import TestCase
from dijkstra import Dijkstra
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
        self.assertEqual(matrix, [[0, 2, 5, None], [13, 0, 3, None], [10, 5, 0, None], [16, 3, 6, 0]])

    def form_random_graph_with_matrix(self, graph, matrix, graph_size, max_edge_weight, diagonal_value,
                                      generator=lambda max_val: random.randint(0, max_val)):
        for i in range(graph_size):
            matrix.append([])
            graph.append([])
            for j in range(graph_size):
                if random.randint(0, 1) == 0:
                    matrix[i].append(None)
                else:
                    matrix[i].append(generator(max_edge_weight))
                    graph[i].append(Edge(j, matrix[i][j]))
        for i in range(graph_size):
            matrix[i][i] = diagonal_value

    def test_stress(self):
        number_of_tests = 10
        max_graph_size = 30
        max_edge_weight = 100
        for test_case in range(number_of_tests):
            graph_size = random.randint(1, max_graph_size)
            matrix = []
            graph = []
            self.form_random_graph_with_matrix(graph, matrix, graph_size, max_edge_weight, 0)
            shortest_paths = copy.deepcopy(matrix)
            Floyd(shortest_paths, graph_size)
            for i in range(graph_size):
                res = Dijkstra(graph, i)
                dists = res.get_path_values()
                for j in range(graph_size):
                    self.assertEqual(dists[j], shortest_paths[i][j])
                    path = res.get_path_to_vertex(j)
                    if path is None:
                        self.assertEqual(shortest_paths[i][j], None)
                    else:
                        actual_weight = sum([matrix[path[i]][path[i + 1]] for i in range(len(path) - 1)])
                        self.assertEqual(actual_weight, shortest_paths[i][j])


class MinimumMaxEdgeReleaseIntegrationTests(TestCase):
    def test_manual_dijkstra(self):
        graph = [[Edge(1, 2), Edge(2, 4)], [Edge(2, 3)], [Edge(0, 3)]]
        res = Dijkstra(graph, 0, sum_path_edge=lambda p, e: max(p, e.weight))
        dist = res.get_path_values()
        self.assertEqual(dist, [0, 2, 3])
        path = res.get_path_to_vertex(2)
        self.assertEqual(path, [0, 1, 2])

    def test_manual_floyd(self):
        matrix = [[0, 1, 3], [4, 0, None], [None, 2, 0]]
        Floyd(matrix, 3, operation=lambda a, b: max(a, b))
        self.assertEqual([[0, 1, 3], [4, 0, 4], [4, 2, 0]], matrix)

    def test_stress(self):
        number_of_tests = 10
        max_graph_size = 30
        max_edge_weight = 1000
        for test_case in range(number_of_tests):
            graph_size = random.randint(1, max_graph_size)
            matrix = []
            graph = []
            BaseReleaseIntegrationTests().form_random_graph_with_matrix(graph, matrix, graph_size, max_edge_weight, 0)
            shortest_paths = copy.deepcopy(matrix)
            Floyd(shortest_paths, graph_size, operation=lambda a, b: max(a, b))
            for i in range(graph_size):
                res = Dijkstra(graph, i, sum_path_edge=lambda p, e: max(p, e.weight))
                dists = res.get_path_values()
                for j in range(graph_size):
                    self.assertEqual(dists[j], shortest_paths[i][j])
                    path = res.get_path_to_vertex(j)
                    if i != j:
                        if path is None:
                            self.assertEqual(shortest_paths[i][j], None)
                        else:
                            actual_weight = max([matrix[path[i]][path[i + 1]] for i in range(len(path) - 1)])
                            self.assertEqual(actual_weight, shortest_paths[i][j])
                    else:
                        self.assertEqual(path, [i])


from ordinals import Ordinal
from ordinals import ordinal_cmp
from ordinals import ordinal_sum
from ordinals import get_random_ordinal


class OrdinalFunctorsTests(TestCase):
    def test_sum(self):
        a = Ordinal([(1, 1), (0, 1)])  # w + 1
        b = Ordinal([(1, 1)])  # w
        c = ordinal_sum(a, b)  # w + 1 + w = w * 2
        d = ordinal_sum(b, a)  # w + w + 1 = w * 2 + 1
        self.assertEqual(c.list, [(1, 2)])
        self.assertEqual(d.list, [(1, 2), (0, 1)])
        a = Ordinal([(3, 1), (1, 1), (0, 1)])  # w^3 + w + 1
        b = Ordinal([(2, 1)])  # w^2
        c = ordinal_sum(a, b)  # w^3 + w + 1 + w^2 = w^3 + w^2
        self.assertEqual(c.list, [(3, 1), (2, 1)])
        a = Ordinal([(0, 0)])  # 0
        b = Ordinal([(3, 1), (2, 2), (1, 3)])  # w^3 + w^2 * 2 + w * 3
        c = ordinal_sum(a, b)
        d = ordinal_sum(b, a)
        self.assertEqual(c.list, b.list)
        self.assertEqual(d.list, b.list)
        q = [(i, i) for i in range(1, 99)]
        q.reverse()
        a = Ordinal(q)  # w^99 * 99 + w^98 * 98 + ... + w
        b = Ordinal([(100, 1)])  # w^100
        c = ordinal_sum(a, b)  # w^99 * 99 + w^98 * 98 + ... + 0 + w^100 = w^100
        self.assertEqual(c.list, b.list)

    def test_cmp(self):
        a = Ordinal([(1, 2), (0, 1)])  # w * 2 + 1
        b = Ordinal([(1, 1), (0, 1)])  # w + 1
        self.assertEqual(ordinal_cmp(a, b), False)
        self.assertEqual(ordinal_cmp(b, a), True)
        self.assertEqual(ordinal_cmp(a, a, ), False)
        a = Ordinal([(100, 1)])  # w^100
        b = Ordinal([(99, 3000)])  # w^99 * 3000
        self.assertEqual(ordinal_cmp(a, b), False)
        self.assertEqual(ordinal_cmp(b, a), True)
        a = Ordinal([(2, 1), (1, 1), (0, 1)])  # w^2 + w + 1
        b = Ordinal([(2, 1), (0, 1)])  # w^2 + 1
        self.assertEqual(ordinal_cmp(a, b), False)
        self.assertEqual(ordinal_cmp(b, a), True)

    def test_get_random_ordinal(self):
        a = get_random_ordinal(100)
        for i in range(len(a.list) - 1):
            self.assertTrue(a.list[i][0] > a.list[i + 1][0])
            self.assertTrue(a.list[i][1] > 0)
            self.assertTrue(a.list[i + 1][1] > 0)


class EdgeOrdinalsReleaseIntegrationTests(TestCase):
    def test_manual_dijkstra(self):
        a = Ordinal([(0, 1)])  # 1
        b = Ordinal([(1, 1)])  # w
        c = Ordinal([(1, 1), (0, 1)])  # w + 1
        d = Ordinal([(0, 0)])  # 0
        graph = [[Edge(1, a), Edge(2, c)], [Edge(2, b)], []]
        res = Dijkstra(graph, 0, start_value=d, cmp=ordinal_cmp, sum_path_edge=lambda p, e: ordinal_sum(p, e.weight))
        dist = res.get_path_values()
        self.assertEqual(dist, [d, a, b])
        path = res.get_path_to_vertex(2)
        self.assertEqual(path, [0, 1, 2])

    def test_manual_floyd(self):
        a = Ordinal([(0, 1)])  # 1
        b = Ordinal([(1, 1)])  # w
        c = Ordinal([(1, 1), (0, 1)])  # w + 1
        d = Ordinal([(0, 0)])  # 0
        matrix = [[d, a, c], [None, d, b], [None, None, d]]
        Floyd(matrix, 3, operation=ordinal_sum, cmp=ordinal_cmp)
        self.assertEqual(matrix, [[d, a, b], [None, d, b], [None, None, d]])

    def test_stress(self):
        number_of_tests = 10
        max_graph_size = 10
        max_edge_weight = 10
        for test_case in range(number_of_tests):
            graph_size = random.randint(1, max_graph_size)
            matrix = []
            graph = []
            BaseReleaseIntegrationTests().form_random_graph_with_matrix(graph, matrix, graph_size,
                                                                        max_edge_weight, Ordinal([(0, 0)]),
                                                                        generator=get_random_ordinal)
            shortest_paths = copy.deepcopy(matrix)
            Floyd(shortest_paths, graph_size, operation=ordinal_sum, cmp=ordinal_cmp)
            for i in range(graph_size):
                res = Dijkstra(graph, i, start_value=Ordinal([(0, 0)]), cmp=ordinal_cmp,
                               sum_path_edge=lambda p, e: ordinal_sum(p, e.weight))
                dists = res.get_path_values()
                for j in range(graph_size):
                    self.assertEqual(dists[j], shortest_paths[i][j])
                    path = res.get_path_to_vertex(j)
                    if i != j:
                        if path is None:
                            self.assertEqual(shortest_paths[i][j], None)
                        else:
                            weights = [matrix[path[i]][path[i + 1]] for i in range(len(path) - 1)]
                            result = Ordinal([(0, 0)])
                            for ordinal in weights:
                                result = ordinal_sum(result, ordinal)
                            self.assertEqual(result, shortest_paths[i][j])
                    else:
                        self.assertEqual(path, [i])


from transport import PublicTransport


class PublicTransportTest(TestCase):
    def test_manual_first(self):
        problem = PublicTransport(4, 10 ** 9)
        problem.add_edge(0, 1, 9, 10)
        problem.add_edge(1, 2, 11, 12)
        problem.add_edge(1, 2, 8, 9)
        problem.add_edge(0, 2, 20, 30)
        problem.add_edge(0, 3, 7, 30)
        problem.solve(0, 9)
        times = problem.get_times()
        self.assertEqual(times, [9, 10, 12, None])
        path = problem.get_path_to_vertex(2)
        self.assertEqual(path, [0, 1, 2])

    def test_manual_second(self):
        problem = PublicTransport(6, 10 ** 9)
        problem.add_edge(1, 2, 5, 10)
        problem.add_edge(2, 4, 10, 15)
        problem.add_edge(5, 4, 0, 17)
        problem.add_edge(4, 3, 17, 20)
        problem.add_edge(3, 2, 20, 35)
        problem.add_edge(1, 3, 2, 40)
        problem.add_edge(3, 4, 40, 45)
        problem.solve(1, 0)
        times = problem.get_times()
        self.assertEqual(times, [None, 0, 10, 20, 15, None])
        path = problem.get_path_to_vertex(3)
        self.assertEqual(path, [1, 2, 4, 3])


    def test_manual_third(self):
        problem = PublicTransport(6, 10 ** 9)
        problem.add_edge(1, 3, 1, 2)
        problem.add_edge(3, 4, 2, 10)
        problem.add_edge(4, 5, 10, 20)
        problem.add_edge(5, 4, 10, 15)
        problem.add_edge(4, 2, 15, 40)
        problem.solve(1, 1)
        times = problem.get_times()
        self.assertEqual(times, [None, 1, 40, 2, 10, 20])
        path = problem.get_path_to_vertex(2)
        self.assertEqual(path, [1, 3, 4, 2])