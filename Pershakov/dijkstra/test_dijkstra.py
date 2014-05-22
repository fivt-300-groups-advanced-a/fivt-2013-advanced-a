from unittest import TestCase
from dijkstra import Dijkstra
from dijkstra import PQEntry
from queue import PriorityQueue
import random

__author__ = 'pershik'


class Edge:
    def __init__(self, to, weight):
        self.to = to
        self.weight = weight


class Path:
    def __init__(self, path, weight=0):
        self.weight = weight
        self.path = path


def add_edge_to_path(path, edge):
    pth = path.path[:]
    pth.append(edge.to)
    return Path(pth, edge.weight + path.weight)


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

    def floyd(self, matrix, size,
              cmp=lambda weight1, weight2: weight1 < weight2,
              sum=lambda weight1, weight2: weight1 + weight2):
        for k in range(0, size):
            for i in range(0, size):
                for j in range(0, size):
                    if matrix[i][k] is None or matrix[k][j] is None:
                        continue
                    elif matrix[i][j] is None:
                        matrix[i][j] = sum(matrix[i][k], matrix[k][j])
                    elif cmp(sum(matrix[i][k], matrix[k][j]), matrix[i][j]):
                        matrix[i][j] = sum(matrix[i][k], matrix[k][j])

    def test_default_implementation(self):
        """
        Check algorithm on all undirected graphs with 5 vertexes with random weights
        """
        size = 5
        random.seed(19121994)
        solver = Dijkstra()
        for msk in range(0, 1 << (size * (size - 1) // 2)):
            graph = []
            for i in range(0, size):
                graph.append([])
            matrix = []
            for i in range(0, size):
                matrix.append([None] * size)
            cur = 0
            for i in range(0, size):
                matrix[i][i] = 0
                for j in range(i + 1, size):
                    weight = random.randint(0, 20)
                    if msk & (1 << cur):
                        matrix[i][j] = matrix[j][i] = weight
                        graph[i].append(Edge(j, weight))
                        graph[j].append(Edge(i, weight))
                    cur += 1

            self.floyd(matrix, size)
            for i in range(0, size):
                res = solver.get_shortest_paths(graph, from_vertex=i, init_path_value=0)
                self.assertEqual(res, matrix[i])

    def check_path(self, matrix, from_vertex, to_vertex, path):
        path_len = len(path.path)
        if path.path[0] != from_vertex or path.path[path_len - 1] != to_vertex:
            return False
        weight = 0
        for i in range(0, path_len - 1):
            if matrix[path.path[i]][path.path[i + 1]] is None:
                return False
            weight += matrix[path.path[i]][path.path[i + 1]]
        return weight == path.weight

    def test_paths(self):
        """
        Find shortest paths to every vertex for all undirected graphs.
        Their length are checked in previous test yet.
        """
        size = 5
        random.seed(19121994)
        solver = Dijkstra()
        for msk in range(0, 1 << (size * (size - 1) // 2)):
            graph = []
            for i in range(0, size):
                graph.append([])
            matrix = []
            for i in range(0, size):
                matrix.append([None] * size)
            cur = 0
            for i in range(0, size):
                matrix[i][i] = 0
                for j in range(i + 1, size):
                    weight = random.randint(0, 20)
                    if msk & (1 << cur):
                        matrix[i][j] = matrix[j][i] = weight
                        graph[i].append(Edge(j, weight))
                        graph[j].append(Edge(i, weight))
                    cur += 1

            for i in range(0, size):
                res = solver.get_shortest_paths(graph, from_vertex=i, init_path_value=Path([i]),
                                                cmp=lambda path1, path2: path1.weight < path2.weight,
                                                add_edge=add_edge_to_path,
                                                get_destination=lambda edge: edge.to)
                for j in range(0, size):
                    if not res[j] is None:
                        self.assertTrue(self.check_path(matrix, from_vertex=i, to_vertex=j, path=res[j]))

    def test_minimal_max_edge(self):
        size = 4
        self.inf = 1000000
        random.seed(19121994)
        solver = Dijkstra()
        for msk1 in range(0, 1 << (size * (size - 1) // 2)):
            for msk2 in range(0, 1 << (size * (size - 1) // 2)):
                graph = []
                for i in range(0, size):
                    graph.append([])
                matrix = []
                for i in range(0, size):
                    matrix.append([None] * size)
                cur = 0
                for i in range(0, size):
                    matrix[i][i] = 0
                    for j in range(i + 1, size):
                        weight1 = random.randint(0, 20)
                        weight2 = random.randint(0, 20)
                        if msk1 & (1 << cur):
                            matrix[i][j] = weight1
                            graph[i].append(Edge(j, weight1))
                        if msk2 & (1 << cur):
                            matrix[j][i] = weight2
                            graph[j].append(Edge(i, weight2))
                        cur += 1

                self.floyd(matrix, size,
                           sum=lambda weight1, weight2: max(weight1, weight2))
                for i in range(0, size):
                    res = solver.get_shortest_paths(graph, from_vertex=i, init_path_value=0,
                                                    add_edge=lambda weight, edge: max(weight, edge.weight))
                    self.assertEqual(res, matrix[i])


class TestRelaxEdge(TestCase):
    def test_stress(self):
        solver = Dijkstra()
        solver.dist = [None, None]
        edge = Edge(1, 0)

        #None case
        solver.dist[0] = 0
        edge.weight = 5
        solver._relax_edge(from_vertex=0, edge=edge,
                           cmp=lambda weight1, weight2: weight1 < weight2,
                           add_edge=lambda path_weight, edge: path_weight + edge.weight,
                           get_destination=lambda edge: edge.to)
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
                    solver._relax_edge(from_vertex=0, edge=edge,
                                       cmp=lambda weight1, weight2: weight1 < weight2,
                                       add_edge=lambda path_weight, edge: path_weight + edge.weight,
                                       get_destination=lambda edge: edge.to)
                    if solver.dist[0] + edge.weight < solver.dist[1]:
                        self.fail('Incorrect weight')
                    if flag:
                        self.assertEqual(solver.pr_queue.qsize(), 1, 'Incorrect pr_queue size')
                        top = solver.pr_queue.get()
                        if top.prior != solver.dist[1] or top.value != 1:
                            self.fail('Incorrect pr_queue')
                    else:
                        self.assertEqual(solver.pr_queue.qsize(), 0, 'Incorrect pr_queue size')


class TestGetMinVertex(TestCase):
    def test_stress(self):
        solver = Dijkstra()
        size = 4
        max_val = 10
        for d1 in range(0, max_val):
            for d2 in range(0, max_val):
                for d3 in range(0, max_val):
                    for d4 in range(0, max_val):
                        solver.dist = [d1, d2, d3, d4]
                        solver.pr_queue = PriorityQueue()
                        for i in range(0, size):
                            solver.pr_queue.put(PQEntry(solver.dist[i], i))
                            solver.pr_queue.put(PQEntry(solver.dist[i] + 2, i))
                        for i in range(0, size):
                            min_vertex = solver._get_min_vertex(
                                cmp=lambda weight1, weight2: weight1 < weight2)
                            pos_min = 0
                            for j in range(0, size):
                                if solver.dist[j] < solver.dist[pos_min]:
                                    pos_min = j
                            self.assertNotEqual(min_vertex, -1)
                            self.assertEqual(solver.dist[min_vertex], solver.dist[pos_min])
                            solver.dist[min_vertex] = max_val + 100
                        min_vertex = solver._get_min_vertex(
                            cmp=lambda weight1, weight2: weight1 < weight2)
                        self.assertEqual(min_vertex, -1)


class Time:
    def __init__(self, hours, minutes, prev=None):
        self.hours = hours
        self.minutes = minutes
        self.prev = prev

    def __lt__(self, other):
        return (self.hours < other.hours or
                (self.hours == other.hours and self.minutes < other.minutes))

    def __eq__(self, other):
        return self.hours == other.hours and self.minutes == other.minutes

    def __le__(self, other):
        return self < other or self == other


class Bus:
    def __init__(self, from_vertex, from_time, to_vertex, to_time):
        self.from_vertex = from_vertex
        self.from_time = from_time
        self.to_vertex = to_vertex
        self.to_time = to_time


def use_bus(time, bus):
    if time > bus.from_time:
        return None
    else:
        return Time(bus.to_time.hours, bus.to_time.minutes, bus.from_vertex)


class TestTransportProblem(TestCase):
    def test_manual(self):
        """
            0 -> 1 09:00 09:30
            0 -> 4 09:10 23:59
            0 -> 3 09:30 11:20
            1 -> 2 09:20 09:50
            2 -> 4 11:26 15:40
            3 -> 2 11:21 11:23
        """
        graph = [[Bus(0, Time(9, 0), 1, Time(9, 30)), Bus(0, Time(9, 10), 4, Time(23, 59)),
                  Bus(0, Time(9, 30), 3, Time(11, 20))],
                 [Bus(1, Time(9, 20), 2, Time(9, 50))],
                 [Bus(2, Time(11, 26), 4, Time(15, 40))],
                 [Bus(3, Time(11, 21), 2, Time(11, 23))],
                 []]
        solver = Dijkstra()
        res = solver.get_shortest_paths(graph, 0, Time(8, 0),
                                        add_edge=use_bus,
                                        get_destination=lambda edge: edge.to_vertex)
        self.assertEqual(res, [Time(8, 0), Time(9, 30), Time(11, 23), Time(11, 20), Time(15, 40)])

    def random_time(self):
        return Time(random.randint(9, 23), random.randint(0, 59))

    def random_timetable(self, size):
        graph = []
        for i in range(0, size):
            graph.append([])
        for i in range(0, size):
            for j in range(i + 1, size):
                if random.randint(0, 1) == 1:
                    from_time = self.random_time()
                    to_time = self.random_time()
                    if from_time > to_time:
                        from_time, to_time = to_time, from_time
                    graph[i].append(Bus(i, from_time, j, to_time))
        return graph

    def check_path(self, graph, res, vertex):
        if res[vertex] is None:
            return True
        prev = res[vertex].prev
        for bus in graph[prev]:
            if (bus.to_vertex == vertex and res[prev] <= bus.from_time
                    and res[vertex] == bus.to_time):
                return True
        return False

    def test_stress(self):
        cnt_tests = 1000
        random.seed(19121994)
        for i in range(0, cnt_tests):
            size = random.randint(1, 50)
            graph = self.random_timetable(size)
            solver = Dijkstra()
            res = solver.get_shortest_paths(graph, 0, Time(8, 0),
                                            add_edge=use_bus,
                                            get_destination=lambda edge: edge.to_vertex)
            for j in range(1, size):
                self.assertTrue(self.check_path(graph, res, j))