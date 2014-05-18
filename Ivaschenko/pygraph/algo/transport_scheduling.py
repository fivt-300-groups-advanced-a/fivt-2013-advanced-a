__author__ = 'skird'

from algo.dijkstra import Dijkstra


class TransportScheduleGraph:
    def __init__(self, vertex_list):
        """
        :param vertex_list - list of vertices in graph
        """
        self.graph = dict()
        for v in vertex_list:
            self.graph[v] = list()

    @staticmethod
    def __append_edge(path, edge):
        if path[0] is None or edge[1] < path[0][2]:
            return None
        return edge, TransportScheduleGraph.__destination(path[0])

    @staticmethod
    def __compare_routes(first, second) -> bool:
        return first[0][2] < second[0][2]

    @staticmethod
    def __destination(edge):
        return edge[0]

    def add_route(self, from_vertex, to_vertex, time_leave, time_come) -> None:
        """
        :param from_vertex starting point of route
        :param to_vertex ending point of route
        :param time_leave start time of route
        :param time_come end time of route
        :rtype None
        :return None
        """
        if from_vertex not in self.graph:
            raise KeyError("from_vertex does not exist")
        if to_vertex not in self.graph:
            raise KeyError("to_vertex does not exist")
        self.graph[from_vertex].append((to_vertex, time_leave, time_come))

    def find_way(self, from_vertex, to_vertex, start_time=0):
        """
        :param from_vertex
        :param to_vertex
        :param start_time
        :rtype pair of arrival time and route to destination - the list of used transport represented by tuple
         of (interjacent_destination, boarding_time, arrival_time)
        :returns pair of arrival time and list of used transport
        """
        dist = Dijkstra.find_shortest_paths(self.graph, [from_vertex],
                                            initial_path_weight=((from_vertex, None, start_time), None),
                                            comparator=TransportScheduleGraph.__compare_routes,
                                            append_edge=TransportScheduleGraph.__append_edge,
                                            edge_destination=TransportScheduleGraph.__destination)
        if to_vertex not in dist or dist[to_vertex][0] is None:
            return None, None
        route = list()
        current = to_vertex
        while current != from_vertex:
            route.append(dist[current][0])
            current = dist[current][1]
        return dist[to_vertex][0][2], list(reversed(route))