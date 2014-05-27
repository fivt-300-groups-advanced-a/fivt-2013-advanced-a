from algo.dijkstra import Dijkstra

__author__ = 'skird'


class ShortestPathFinder:
    def __init__(self, vertices_list):
        """
        TODO: doc
        """
        self.__comparator = lambda w1, w2: w1 < w2
        self.__appender = lambda path, edge: path + edge[1]
        self.__get_destination = lambda edge: edge[0]
        self.__start_vertex = 0

        self.__graph = {v: list() for v in vertices_list}

    def __contains__(self, item):
        return item in self.__graph

    def __compare_routes(self, first, second):
        return self.__comparator(first[0], second[0])

    def __append_edge(self, path, edge):
        new_weight = self.__appender(path[0], edge)
        if new_weight is None:
            return None
        return self.__appender(path[0], edge), edge, self.__edge_destination(path[1])

    def __edge_destination(self, edge):
        if edge is None:
            return self.__start_vertex
        return self.__get_destination(edge)

    def add_edge(self, from_vertex, edge):
        """
        TODO: doc
        """
        if from_vertex not in self.__graph:
            raise KeyError('starting vertex not in graph')
        if self.__get_destination(edge) not in self.__graph:
            raise KeyError('ending vertex not in graph')
        self.__graph[from_vertex].append(edge)

    def add_edges(self, edges):
        """
        TODO: doc
        """
        for edge in edges:
            self.add_edge(edge[0], edge[1])

    def erase_edge(self, from_vertex, edge):
        if from_vertex not in self.__graph or edge not in self.__graph[from_vertex]:
            return
        self.__graph[from_vertex].remove(edge)

    def find_shortest_path(self, from_vertex, to_vertex, *,
                           initial_path_weight=0,
                           comparator=lambda w1, w2: w1 < w2,
                           append_edge=lambda path, edge: path + edge[1],
                           edge_destination=lambda edge: edge[0]):
        """
        TODO: doc
        """
        self.__comparator = comparator
        self.__appender = append_edge
        self.__get_destination = edge_destination
        self.__start_vertex = from_vertex
        dist = Dijkstra.find_shortest_paths(self.__graph, [from_vertex],
                                            initial_path_weight=(initial_path_weight, None, None),
                                            comparator=self.__compare_routes, append_edge=self.__append_edge,
                                            edge_destination=self.__edge_destination)
        if to_vertex not in dist:
            return None, None

        route = list()
        current = to_vertex
        while current != from_vertex:
            route.append(dist[current][1])
            current = dist[current][2]
        return dist[to_vertex][0], list(reversed(route))