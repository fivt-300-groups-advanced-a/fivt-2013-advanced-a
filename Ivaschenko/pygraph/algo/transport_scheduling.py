__author__ = 'skird'

from algo.shortestpathfinder import ShortestPathFinder


class TransportScheduleGraph:
    def __init__(self, vertices_list):
        """
        :param vertices_list - list of vertices of graph
        """
        self.__finder = ShortestPathFinder(vertices_list)

    @staticmethod
    def __append_edge(path, edge):
        if path > edge[1]:
            return None
        return edge[2]

    def add_route(self, from_vertex, to_vertex, time_leave, time_come) -> None:
        """
        :param from_vertex starting point of route
        :param to_vertex ending point of route
        :param time_leave start time of route
        :param time_come end time of route
        :rtype None
        :return None
        """
        if from_vertex not in self.__finder:
            raise KeyError("from_vertex does not exist")
        if to_vertex not in self.__finder:
            raise KeyError("to_vertex does not exist")
        self.__finder.add_edge(from_vertex, (to_vertex, time_leave, time_come))

    def find_way(self, from_vertex, to_vertex, start_time=0):
        """
        :param from_vertex
        :param to_vertex
        :param start_time
        :rtype pair of arrival time and route to destination - the list of used transport represented by tuple
         of (interjacent_destination, boarding_time, arrival_time)
        :returns pair of arrival time and list of used transport
        """
        return self.__finder.find_shortest_path(from_vertex, to_vertex,
                                                initial_path_weight=start_time,
                                                append_edge=TransportScheduleGraph.__append_edge)