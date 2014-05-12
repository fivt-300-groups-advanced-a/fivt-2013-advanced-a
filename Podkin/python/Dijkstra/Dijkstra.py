__author__ = 'Alex'

from queue import PriorityQueue


class Node:
    def __init__(self, weight, vertex,
                 compare_weight=lambda weight_a, weight_b: weight_a < weight_b):
        """

        :param weight: weight of edge
        :param vertex: vertex that edge is going to
        :param compare_weight: functor for comparing 2 weights, it's true when first smaller than second
        """
        self.weight = weight
        self.vertex = vertex
        self.compare_weight = compare_weight

    def __lt__(self, other):
        return self.compare_weight(self.weight, other.weight)


class Dijkstra:
    def __init__(self):
        self.distances = []
        self.number_of_vertexes = 0
        self.queue = PriorityQueue()

    def upgrade_path_through_vertex(self, graph, vertex,
                                    compare_weight=lambda weight_a, weight_b: weight_a < weight_b,
                                    adder_to_path=lambda path_weight, edge: path_weight + edge.weight,
                                    destination_of_edge=lambda edge: edge.vertex):
        """

        :param graph: list of incident vertexes lists
        :param vertex: starting vertex
        :param compare_weight: functor for comparing 2 weights, it's true when first smaller than second
        :param adder_to_path: functor for adding weight of new edge for current path
        :param destination_of_edge: returning destination of edge
        """
        for edge in graph[vertex]:
            to = destination_of_edge(edge)
            new_weight = adder_to_path(self.distances[vertex], edge)
            if self.distances[to] is None or compare_weight(new_weight, self.distances[to]):
                self.distances[to] = new_weight
                self.queue.put(Node(new_weight, to, compare_weight))

    def init_before_dijkstra(self, number_of_vertexes, start_vertex, starting_weight,
                             compare_weight=lambda weight_a, weight_b: weight_a < weight_b):
        """

        :param number_of_vertexes: number of vertexes in graph
        :param start_vertex: you are looking for paths to all vertexes from this vertex
        :param starting_weight: starting value of distance
        :param compare_weight: functor for comparing 2 weights, it's true when first smaller than second
        """
        self.number_of_vertexes = number_of_vertexes
        self.distances = [None] * self.number_of_vertexes
        self.distances[start_vertex] = starting_weight
        self.queue.put(Node(starting_weight, start_vertex, compare_weight))

    def get_shortest_path(self, graph, number_of_vertexes, start_vertex, starting_weight,
                          compare_weight=lambda weight_a, weight_b: weight_a < weight_b,
                          adder_to_path=lambda path_weight, edge: path_weight + edge.weight,
                          destination_of_edge=lambda edge: edge.vertex):
        """

        :param graph: list of incident vertexes lists
        :param number_of_vertexes: number of vertexes in graph
        :param start_vertex: you are looking for paths to all vertexes from this vertex
        :param starting_weight: starting value of distance
        :param compare_weight: functor for comparing 2 weights, it's true when first smaller than second
        :param adder_to_path: functor for adding weight of new edge for current path
        :param destination_of_edge: returning destination of edge
        :return: list of shortest paths from start_vertex
        """
        self.init_before_dijkstra(number_of_vertexes, start_vertex, starting_weight, compare_weight)
        while not self.queue.empty():
            top = self.queue.get()
            vertex = top.vertex
            weight = top.weight
            if compare_weight(self.distances[vertex], weight) or compare_weight(weight, self.distances[vertex]):
                continue
            self.upgrade_path_through_vertex(graph, vertex, compare_weight, adder_to_path,
                                             destination_of_edge)
        return self.distances