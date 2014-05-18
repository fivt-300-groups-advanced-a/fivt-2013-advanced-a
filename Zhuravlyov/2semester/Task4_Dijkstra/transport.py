__author__ = 'Алексей'

from dijkstra import Dijkstra


class PublicTransport:
    class Edge:
        def __init__(self, time_depart, to_vertex, time_arrive):
            self.time_depart = time_depart
            self.time_arrive = time_arrive
            self.destination = to_vertex

    def __init__(self, graphSize, inf_time, cmp_time = lambda t1, t2: t1 < t2):
        """
        :param graphSize: size of Graph
        :param inf_time: Time which is more then all other for cmp_time
        :param cmp_time: compares two times
        """
        self.graph = []
        self.inf_time = inf_time
        self.cmp_time = cmp_time
        for i in range(graphSize):
            self.graph.append([])

    def __add_edge_to_path__(self, path, edge):
        if self.cmp_time(edge.time_depart, path):
            return self.inf_time
        else:
            return edge.time_arrive

    def add_edge(self, from_vertex, to_vertex, time_depart, time_arrive):
        """
        :param from_vertex: Number of departure vertex
        :param to_vertex: Number of arrival vertex
        :param time_depart: Time of department
        :param time_arrive: Time of arrival
        """
        self.graph[from_vertex].append(self.Edge(time_depart, to_vertex, time_arrive))

    def solve(self, start_vertex, start_time):
        """
        :param start_vertex: Vertex, where you start moving
        :param start_time: Time when you appear there
        """
        self.result = Dijkstra(self.graph, start_vertex, start_time, self.cmp_time, self.__add_edge_to_path__)

    def get_times(self):
        """
        :return: list of shortest times to get to all vertices, None if impossible to get
        """
        res = self.result.get_path_values()
        for i in range(len(res)):
            if res[i] == self.inf_time:
                res[i] = None
        return res

    def get_path_to_vertex(self, finish_vertex):
        """
        :param finish_vertex: number of finish vertex. It's time must be non-None in get_times method
        :return: list of vertices you need to visit from start vertex to finish vertex in right order.
        """
        return self.result.get_path_to_vertex(finish_vertex)